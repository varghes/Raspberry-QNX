/*
 * $QNXLicenseC:
 * Copyright 2009, QNX Software Systems.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"). You
 * may not reproduce, modify or distribute this software except in
 * compliance with the License. You may obtain a copy of the License
 * at: http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OF ANY KIND, either express or implied.
 *
 * This file may contain contributions from others, either as
 * contributors under the License or as licensors under other terms.
 * Please review this entire file for other proprietary rights or license
 * notices, as well as the QNX Development Suite License Guide at
 * http://licensing.qnx.com/license-guide/ for other information.
 * $
 */

#include "startup.h"
#include <arm/ep93xx_private.h>
#include <hw/hwinfo_ep93xx.h>
#include <pic_ep93xx.h>
#include <drvr/hwinfo.h>


/*
 * =============================================================================
 *                                M A C R O S
 * =============================================================================
*/


/*
 * =============================================================================
 *                   F O R W A R D   D E C L A R A T I O N S
 * =============================================================================
*/


/*
 * =============================================================================
 *                               A P I 's
 * =============================================================================
*/


/* reserve 2 uint32_t's for VIC1 and VIC2 non-vectored interrupt masks. By
 * default all of the interrupts are non-vectored. This can be overridden with
 * the ep93xx_set_vector_priority() routine.
*/
static uint32_t nonvectored_int_masks[] = {~0, ~0};
long non_vectored_int_mask = sizeof(nonvectored_int_masks);

/*
 * We have 5 interrupt info entries.
 * The 1st is for the 64 internal interrupts of the EP9301.
 * The 2nd is for the 16 interruptible pins of GPIO ports A and B. This is a
 * cascaded entry off of the general GPIO interrupt 59.
 * The 3rd, 4th and 5th are for port F. These are cascaded entries off of the
 * 3 dedicated interrupts for each of the 3 port F pins. The same callout is
 * used for each. Port F is implemented as a cascade to allow the handling of
 * the GPIO mask and interrupt clear similar to how Ports A and B are handled
 * and to allow all of the GPIO ports to be logically grouped together, Port F
 * relative interrupt priorities can be independently raised or lowered using
 * the hardware prioritization mechanism of the VIC's facilitated by the
 * ep93xx_set_vector_priority() routine.
*/
static struct startup_intrinfo intrs[] =
{
	{
		.vector_base = 0,		/* will be filled in by ep93xx_config_pic() */
		.num_vectors = 64,
		.cascade_vector = _NTO_INTR_SPARE,
		.cpu_intr_base = 0,
		.cpu_intr_stride = 0,
		.flags = 0,
		.id = {.genflags = INTR_GENFLAG_LOAD_SYSPAGE, .size = 0, .rtn = &interrupt_id_ep93xx},
		.eoi = {.genflags = INTR_GENFLAG_LOAD_SYSPAGE | INTR_GENFLAG_LOAD_INTRMASK, .size = 0, .rtn = &interrupt_eoi_ep93xx},
		.mask = &interrupt_mask_ep93xx,
		.unmask = &interrupt_unmask_ep93xx,
		.config = &interrupt_config_ep93xx,
		.patch_data = nonvectored_int_masks,
	},
	{	/* GPIO controller (Ports A and B) */
		.vector_base = 0,		/* will be filled in by ep93xx_config_pic() */
		.num_vectors = 16,		/* Port A, pins 0 - 7 and port B, pins 0 - 7 */
		.cascade_vector = 59,
		.cpu_intr_base = 0,
		.cpu_intr_stride = 0,
		.flags = 0,
		.id = {.genflags = 0, .size = 0, .rtn = &interrupt_id_ep93xx_gpio_ab},
		.eoi = {.genflags = INTR_GENFLAG_LOAD_INTRMASK, .size = 0, .rtn = &interrupt_eoi_ep93xx_gpio_ab},
		.mask = &interrupt_mask_ep93xx_gpio_ab,
		.unmask = &interrupt_unmask_ep93xx_gpio_ab,
		.config = &interrupt_config_ep93xx_gpio_ab,
		.patch_data = NULL,
	},
	{	/* GPIO controller (Port F) */
		.vector_base = 0,		/* will be filled in after ep93xx_config_pic() */
		.num_vectors = 1,
		.cascade_vector = 20,	/* port F, pin 0 */
		.cpu_intr_base = 0,
		.cpu_intr_stride = 0,
		.flags = 0,
		.id = {.genflags = 0, .size = 0, .rtn = &interrupt_id_ep93xx_gpio_f},
		.eoi = {.genflags = INTR_GENFLAG_LOAD_INTRMASK, .size = 0, .rtn = &interrupt_eoi_ep93xx_gpio_f},
		.mask = &interrupt_mask_ep93xx_gpio_f,
		.unmask = &interrupt_unmask_ep93xx_gpio_f,
		.config = &interrupt_config_ep93xx_gpio_f,
		.patch_data = NULL,
	},
	{	/* GPIO controller (Port F) */
		.vector_base = 0,		/* will be filled in after ep93xx_config_pic() */
		.num_vectors = 1,
		.cascade_vector = 21,	/* port F, pin 1 */
		.cpu_intr_base = 0,
		.cpu_intr_stride = 0,
		.flags = 0,
		.id = {.genflags = 0, .size = 0, .rtn = &interrupt_id_ep93xx_gpio_f},
		.eoi = {.genflags = INTR_GENFLAG_LOAD_INTRMASK, .size = 0, .rtn = &interrupt_eoi_ep93xx_gpio_f},
		.mask = &interrupt_mask_ep93xx_gpio_f,
		.unmask = &interrupt_unmask_ep93xx_gpio_f,
		.config = &interrupt_config_ep93xx_gpio_f,
		.patch_data = NULL,
	},
	{	/* GPIO controller (Port F) */
		.vector_base = 0,		/* will be filled in after ep93xx_config_pic() */
		.num_vectors = 1,
		.cascade_vector = 22,	/* port F, pin 2 */
		.cpu_intr_base = 0,
		.cpu_intr_stride = 0,
		.flags = 0,
		.id = {.genflags = 0, .size = 0, .rtn = &interrupt_id_ep93xx_gpio_f},
		.eoi = {.genflags = INTR_GENFLAG_LOAD_INTRMASK, .size = 0, .rtn = &interrupt_eoi_ep93xx_gpio_f},
		.mask = &interrupt_mask_ep93xx_gpio_f,
		.unmask = &interrupt_unmask_ep93xx_gpio_f,
		.config = &interrupt_config_ep93xx_gpio_f,
		.patch_data = NULL,
	},
};


/*******************************************************************************
 * ep9301_config_pic
 *
 * This routine will initialize the 2 Vectored Interrupt Controllers in the
 * Cirrus Logic EP93xx family of processors.
 *
 * In order to handle the number of interrupts within the EP93xx, 2 separate
 * interrupt controllers are cascaded together to form 1 larger controller.
 * There is not much documentation on how this cascading of interrupts works as
 * it is not a true cascade. There is some logic which allows the second
 * controller to be daisy chained through the first controller which is actually
 * connected to the processor. There is no documentation on how prioritization
 * works for non vectored interrupts or the prioritization of the second
 * controller relative to the first. The only remote suggestion that they are
 * handled as 32 priority interrupts starting with controller 1 followed by
 * controller 2 is on page 118 of the User Guide which says ...
 *
 * 		"Vector Control Registers. The 32 VICxVectCntl0 through VICxVectCnt15
 * 		 registers select the interrupt source for the vectored interrupt."
 *
 * My interpretation is that vector 0 through 15 on controller 1 followed by
 * vector 0 through 15 on controller 2 is the prioritization.
 *
 * This still leaves the problem that we don't know which controller to look
 * at first without looking at the status register for each ... which implies
 * that the prioritization is completely under software control. In other words,
 * software must decide to either read controller 1 or 2 status first, effectively
 * prioritizing one set of 32 interrupts over the other 32. Then, because some of
 * the interrupts are not vectored, we must choose bits in the status register.
 * However, if the vector address read when none of the interrupts programmed as
 * vectored interrupts is the default vector, then the sequence can flow as
 * follows (I will prioritize controller 1 interrupts over controller 2)
 *
 * 		if (vic1_status != 0)
 * 		{
 * 			if (vic1_vector != default vector)
 * 				id = vect_table[vic1_vector]
 * 			else
 * 				id = first bit set in vic1_status
 * 		}
 * 		else if (vic2_status != 0)
 * 		{
 * 			if (vic1_vector != default vector)
 * 				id = vect_table[vic2_vector]
 * 			else
 * 				id = first bit set in vic2_status
 * 		}
 * 		else
 * 			spurious interrupt
 *
 * 		return id
 *
 * This is all greatly complicated by the fact that only 1/2 of the interrupts
 * in each controller are vectored and prioritized not to mention the
 * prioritization between IRQ and FIQ interrupts.
 * To properly support prioritized interrupts and decouple them from the assigned
 * internal id's is going to require some more thought so for now I am not going
 * to use the hardware prioritization of the vectored interrupts and instead
 * simply do a first bit set prioritization of interrupts as follows
 *
 * 			VIC1, 31 -> 0
 * 			VIC2, 31 -> 0
 *
 * implying that in the case of multiple occurring interrupts, VIC1, interrupt
 * 31 (documented interrupt source 31) is the highest in the system and VIC2,
 * interrupt 0 (documented interrupt source 32) is the lowest.
 *
 * Additionally, FIQ is not going to be used for now.
 *
 * Returns: the number of interrupt vectors assigned
 *
 * Implementation Note:
 *
*/
uint32_t ep9301_config_pic(uint32_t os_vector_base, uint32_t pic_vector_base, vector_tbl_t *reg_vectors)
{
	uintptr_t base = startup_io_map((EP93xx_VIC_CTRL2_BASE - EP93xx_VIC_CTRL1_BASE) + EP93xx_VIC_CTRL2_SIZE, EP93xx_VIC_CTRL1_BASE);
	unsigned hwi_off;
	unsigned i;

	/*
	 * check the peripheral ID registers. We will assert on the truth of valid_pic_id() since a change
	 * may suggest a difference in the device behaviour which may need to be reflected in a
	 * code change
	*/
	ASSERT(valid_pic_id());
	ASSERT(pic_vector_base == os_vector_base);	// re-mapping not supported (yet)

	reg_vectors->start = pic_vector_base;
	reg_vectors->num = 0;

	/* set all the vector bases */
	intrs[0].vector_base = os_vector_base;
	ASSERT(intrs[0].num_vectors > 0);
	reg_vectors->num += intrs[0].num_vectors;
	for (i=1; i<NUM_ELTS(intrs); i++)
	{
		intrs[i].vector_base = intrs[i - 1].vector_base + intrs[i - 1].num_vectors;
		ASSERT(intrs[i].num_vectors > 0);
		reg_vectors->num += intrs[i].num_vectors;
	}
	/* add the interrupt info section */
	add_interrupt_array(intrs, sizeof(intrs));

	/* disable and clear all interrupts. Note that we do not check EP93xx_VIC_INT_RAW because
	 * some devices do actually have interrupts asserted and this is reflected in the RAW register(s) */
	out32(VIC1(EP93xx_VIC_INT_CLEAR), 0xFFFFFFFFU);
	out32(VIC1(EP93xx_VIC_SWINT_CLEAR), 0xFFFFFFFFU);
	ASSERT(in32(VIC1(EP93xx_VIC_INT_ENABLE)) == 0);
	ASSERT(in32(VIC1(EP93xx_VIC_SWINT_ENABLE)) == 0);
	ASSERT(in32(VIC1(EP93xx_VIC_IRQ_STATUS)) == 0);
	ASSERT(in32(VIC1(EP93xx_VIC_FIQ_STATUS)) == 0);

	out32(VIC2(EP93xx_VIC_INT_CLEAR), 0xFFFFFFFFU);
	out32(VIC2(EP93xx_VIC_SWINT_CLEAR), 0xFFFFFFFFU);
	ASSERT(in32(VIC2(EP93xx_VIC_INT_ENABLE)) == 0);
	ASSERT(in32(VIC2(EP93xx_VIC_SWINT_ENABLE)) == 0);
	ASSERT(in32(VIC2(EP93xx_VIC_IRQ_STATUS)) == 0);
	ASSERT(in32(VIC2(EP93xx_VIC_FIQ_STATUS)) == 0);

	/* set privileged access to interrupt controller registers */
	out32(VIC1(EP93xx_VIC_PROTECTION), 0x0);
	out32(VIC2(EP93xx_VIC_PROTECTION), 0x0);

	/* by default, all interrupts will generate an IRQ (FIQ not used) */
	out32(VIC1(EP93xx_VIC_INT_SELECT), 0);
	out32(VIC2(EP93xx_VIC_INT_SELECT), 0);

	/* place HWI_ILLEGAL_VECTOR into the default vector(s) */
	out32(VIC1(EP93xx_VIC_VEC_ADDR_DFLT), HWI_ILLEGAL_VECTOR);
	out32(VIC2(EP93xx_VIC_VEC_ADDR_DFLT), HWI_ILLEGAL_VECTOR);

	/* disable the use of vectored interrupts for now */
	in32(VIC1(EP93xx_VIC_VEC_ADDR_CURR));
	out32(VIC1(EP93xx_VIC_VEC_ADDR_CURR), 0);
	in32(VIC2(EP93xx_VIC_VEC_ADDR_CURR));
	out32(VIC2(EP93xx_VIC_VEC_ADDR_CURR), 0);
	for (i=0; i<16; i++)
	{
		out32(VIC1(EP93xx_VIC_VEC_ADDR(i)), HWI_ILLEGAL_VECTOR);
		out32(VIC1(EP93xx_VIC_VEC_CTRL(i)), 0);
		out32(VIC2(EP93xx_VIC_VEC_ADDR(i)), HWI_ILLEGAL_VECTOR);
		out32(VIC2(EP93xx_VIC_VEC_CTRL(i)), 0);
	}

	/* TIMER1 - interrupt 4 */
	hwitag_set_avail_ivec(hwi_find_device(EP93xx_HWI_TIMER, 0), -1, 4);

	/* TIMER2 - interrupt 5 */
	hwitag_set_avail_ivec(hwi_find_device(EP93xx_HWI_TIMER, 1), -1, 5);

	/* TIMER3 - interrupt 51 */
	hwitag_set_avail_ivec(hwi_find_device(EP93xx_HWI_TIMER, 2), -1, 51);

	/* WATCHDOG TIMER - interrupt 36 */
	hwitag_set_avail_ivec(hwi_find_device(EP93xx_HWI_WDOG, 0), -1, 36);

	/* DMA channels - Note that the order of vector assignment in the HWINFO section will be
	 * memory to memory 0 - 17
	 * memory to memory 1 - 18
	 * memory to peripheral 0 - 7
	 * memory to peripheral 1 - 8
	 * memory to peripheral 2 - 9
	 * memory to peripheral 3 - 10
	 * memory to peripheral 4 - 11
	 * memory to peripheral 5 - 12
	 * memory to peripheral 6 - 13
	 * memory to peripheral 7 - 14
	 * memory to peripheral 8 - 15
	 * memory to peripheral 9 - 16
	*/
	if ((hwi_off = hwi_find_device(EP93xx_HWI_DMA, 0)) != HWI_NULL_OFF)
	{
		hwitag_set_ivec(hwi_off, 0, 17);
		hwitag_set_ivec(hwi_off, 1, 18);
		hwitag_set_ivec(hwi_off, 2, 7);
		hwitag_set_ivec(hwi_off, 3, 8);
		hwitag_set_ivec(hwi_off, 4, 9);
		hwitag_set_ivec(hwi_off, 5, 10);
		hwitag_set_ivec(hwi_off, 6, 11);
		hwitag_set_ivec(hwi_off, 7, 12);
		hwitag_set_ivec(hwi_off, 8, 13);
		hwitag_set_ivec(hwi_off, 9, 14);
		hwitag_set_ivec(hwi_off, 10, 15);
		hwitag_set_ivec(hwi_off, 11, 16);
	}

	/* GPIO
	 * 19 of the EP9301 GPIO ports are interruptible and are handled via a
	 * cascaded interrupt entry. In order for devices specifically connected to
	 * those GPIO pins to have a driver InterruptAttach() we provide the assigned
	 * vector ranges with 3 irqrange tags corresponding to the PORTA, PORTB and
	 * PORTF interrupting port pins respectively. Within the range tag assignments,
	 * the 'irq' field corresponds to the LSb (pin 0 of the port) and 'irq' + 'num' - 1
	 * corresponds to the MSb (pin 7 of the port).
	*/
	if ((hwi_off = hwi_find_device(EP93xx_HWI_GPIO, 0)) != HWI_NULL_OFF)
	{
		const unsigned num_pAB_vectors = intrs[1].num_vectors / 2;
		const unsigned num_pF_vectors = intrs[2].num_vectors + intrs[3].num_vectors + intrs[4].num_vectors;
		hwitag_set_ivecrange(hwi_off, 0, intrs[1].vector_base, num_pAB_vectors);	// PORT A
		hwitag_set_ivecrange(hwi_off, 1, intrs[1].vector_base + num_pAB_vectors, num_pAB_vectors);	// PORT B
		hwitag_set_ivecrange(hwi_off, 2, intrs[2].vector_base, num_pF_vectors);	// PORT F
	}

	/* UART1 - Note that the order of vector assignment in the HWINFO section will be
	 * combined - 52
	 * Rx - 23
	 * Tx - 24
	*/
	if ((hwi_off = hwi_find_device(EP93xx_HWI_UART, 0)) != HWI_NULL_OFF)
	{
		hwitag_set_ivec(hwi_off, 0, 52);
		hwitag_set_ivec(hwi_off, 1, 23);
		hwitag_set_ivec(hwi_off, 2, 24);
	}

	/* UART2 - Note that the order of vector assignment in the HWINFO section will be
	 * combined - 54
	 * Rx - 25
	 * Tx - 26
	*/
	if ((hwi_off = hwi_find_device(EP93xx_HWI_UART, 1)) != HWI_NULL_OFF)
	{
		hwitag_set_ivec(hwi_off, 0, 54);
		hwitag_set_ivec(hwi_off, 1, 25);
		hwitag_set_ivec(hwi_off, 2, 26);
	}

	/* EXTERNAL IRQ - 2 interrupts 32, 33 and 40 for IQR0, IRQ1 and IRQ2 respectively */
	if ((hwi_off = hwi_find_device(EP93xx_HWI_EXT_IRQ, 0)) != HWI_NULL_OFF)
	{
		hwitag_set_ivec(hwi_off, 0, 32);
		hwitag_set_ivec(hwi_off, 1, 33);
		hwitag_set_ivec(hwi_off, 2, 40);
	}

	/* RTC - Note that the order of vector assignment in the HWINFO section will be
	 * rtc - 37
	 * 64 Hz clock - 35
	 * 1 Hz clock - 42 (edge sensitive)
	*/
	if ((hwi_off = hwi_find_device(EP93xx_HWI_RTC, 0)) != HWI_NULL_OFF)
	{
		hwitag_set_ivec(hwi_off, 0, 37);
		hwitag_set_ivec(hwi_off, 1, 35);
		hwitag_set_ivec(hwi_off, 2, 42);
	}

	/* IrDA Device - 38 */
	if ((hwi_off = hwi_find_device(EP93xx_HWI_IrDA, 0)) != HWI_NULL_OFF) {
		hwitag_set_avail_ivec(hwi_off, 0, 38);
	}

	/* Ethernet Device */
	if ((hwi_off = hwi_find_device(EP93xx_HWI_ENET, 0)) != HWI_NULL_OFF) {
		hwitag_set_avail_ivec(hwi_off, 0, 39);
	}

	/* SSP - Note that the order of vector assignment in the HWINFO section will be
	 * combined - 53
	 * Rx - 45
	 * Tx - 46
	*/
	if ((hwi_off = hwi_find_bus(EP93xx_HWI_SSP, 0)) != HWI_NULL_OFF)
	{
		hwitag_set_ivec(hwi_off, 0, 53);
		hwitag_set_ivec(hwi_off, 1, 45);
		hwitag_set_ivec(hwi_off, 2, 46);
	}

	/* USB host controller - 56 */
	if ((hwi_off = hwi_find_bus(EP93xx_HWI_USB, 0)) != HWI_NULL_OFF) {
		hwitag_set_avail_ivec(hwi_off, 0, 56);
	}

	/* I2S device - 60 */
	if ((hwi_off = hwi_find_device(EP93xx_HWI_I2S, 0)) != HWI_NULL_OFF) {
		hwitag_set_avail_ivec(hwi_off, 0, 60);
	}

	/* ACC (AC97) device - 6 */
	if ((hwi_off = hwi_find_device(EP93xx_HWI_AUDIO, 0)) != HWI_NULL_OFF) {
		hwitag_set_avail_ivec(hwi_off, 0, 6);
	}

	startup_io_unmap(base);

	return reg_vectors->num;
}

/*
 * ep93xx_set_vector_priority
 *
 * Set the priority of <vector> to <priority>.
 * The routine is currently constrained (via ASSERTS) to deal only with the
 * EP93xx defined vectored interrupts therefore the combination of vector and
 * priority must make sense for the hardware arrangement
 *
 * Returns: EOK on success or errno
*/
#define VECTORED_INT_ENABLE		(1U << 5)
#include <errno.h>
int ep93xx_set_vector_priority(unsigned vector, unsigned priority)
{
	if (vector < 32)		// VIC1
	{
		ASSERT(priority < 16);
		out32(VIC1(EP93xx_VIC_VEC_ADDR(priority)), vector);
		out32(VIC1(EP93xx_VIC_VEC_CTRL(priority)), vector | VECTORED_INT_ENABLE);
		nonvectored_int_masks[0] &= ~(1U << vector);
		return EOK;
	}
	else if (vector < 64)	// VIC2
	{
		/* normalize <vector> and <priority> to VIC2 */
		priority -= 16;
		vector -= 32;
		ASSERT(priority < 16);
		out32(VIC2(EP93xx_VIC_VEC_ADDR(priority)), vector);
		out32(VIC2(EP93xx_VIC_VEC_CTRL(priority)), vector | VECTORED_INT_ENABLE);
		nonvectored_int_masks[1] &= ~(1U << vector);
		return EOK;
	}
	return EINVAL;
}

/*
 * =============================================================================
 *                     S U P P O R T   R O U T I N E S
 * =============================================================================
*/




__SRCVERSION("pic_85xx.c $Rev: 170330 $");
