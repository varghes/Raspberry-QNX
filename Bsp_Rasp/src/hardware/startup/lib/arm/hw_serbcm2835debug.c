/*
 * hw_serbcm2835debug.c
 * QNX - Raspberry Pi
 * by Varghese Alphonse
 *
 */



#include "startup.h"
#include <arm/bcm2835.h>

static void
parse_line(unsigned channel, const char *line, unsigned *baud, unsigned *clk)
{
	/*
	 * Get device base address and register stride
	 */
	if (*line != '.' && *line != '\0') {
		//dbg_device[channel].base = strtoul(line, (char **)&line, 16);
		dbg_device[channel].base = BCM2835_UART0_BASE; //fix me hardcoded
		if (*line == '^')
			dbg_device[channel].shift = strtoul(line+1, (char **)&line, 0);
	}

	/*
	 * Get baud rate
	 */
	if (*line == '.')
		++line;
	if (*line != '.' && *line != '\0')
		*baud = strtoul(line, (char **)&line, 0);

	/*
	 * Get clock rate
	 */
	if (*line == '.')
		++line;
	if (*line != '.' && *line != '\0')
		*clk = strtoul(line, (char **)&line, 0);
}


/*
 * Initialise one of the serial ports
 * buad rates fixed to default- 115200
 * so options are disabled
 */
void
init_bcm2835_debug(unsigned channel, const char *init, const char *defaults)
{
	unsigned	baud, clk, base;
	//buad rate will be added later
	parse_line(channel, defaults, &baud, &clk);
	parse_line(channel, init, &baud, &clk);
	base = dbg_device[channel].base;
	// Disable UART0.

		out32(base + BCM2835_UART0_CR, 0x00000000); // clear control register
		out32(base+ BCM2835_UART0_ICR, 0x7FF);//clear all interrupts
		out32(base + BCM2835_UART0_IBRD, 1);  // Baurd Rate divisior BAUDDIV = (FUARTCLK/(16 Baud rate))
		out32(base + BCM2835_UART0_FBRD, 40);// Fractional Baud rate divisor
		out32(base + BCM2835_UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));  // Line control register
		                                 //Word len:8         eps -parity
		  //
	//	out32(base + BCM2835_UART0_IMSC, (1 << 1) | (1 << 4)| (1 << 5) | (1 << 6) |
		//                       (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));
		     //1-cts interrupt 4-RX intr 5- tx intr 6-receive timeout 7-framing error 8- parrity error 9-break error
		     //10-overrun error  - mask all

		out32(base + BCM2835_UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 6) |
				                       (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));
		out32(base + BCM2835_UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));
		                                // enable UART, TX & RX

}

/*
 * Send a character
 */
void
put_bcm2835(int data)
{
	paddr_t base = dbg_device[0].base;

	// Wait for UART to become ready to transmit.
		while ( in32(base + BCM2835_UART0_FR) & BCM2835_DBGU_IE_SR_TXRDY ) { }
		out32(base + BCM2835_UART0_DR, (unsigned)data);

}
