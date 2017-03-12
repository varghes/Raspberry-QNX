
#include <stdlib.h>
#include <stdio.h>
#include <hw/inout.h>
#include <sys/mman.h>
#include <sys/neutrino.h>
#include <stdint.h>
#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/neutrino.h>
#include <hw/inout.h>
#include <sys/slog.h>
#include <sys/slogcodes.h>


//#define BCM2835_DEBUG_LED
#define GPIO_BASE       0x20200000
#define GPIO_REG_SIZE   0x30
#define LED_GPFSEL_GREEN      0x10
#define LED_GPFSEL_RED      0x0C
#define LED_GPFBIT_GREEN      21
#define LED_GPFBIT_RED      15
#define LED_GPSET       0x20
#define LED_GPCLR       0x2c
#define LED_GPIO_BIT_GREEN    15
#define LED_GPIO_BIT_RED    3
uintptr_t gpio;

int main(int argc, char *argv[]) {
	uintptr_t  gpio_base;
	unsigned int val;

	printf("LED Test-Red\n");
	ThreadCtl (_NTO_TCTL_IO,NULL);
	gpio_base = mmap_device_io(GPIO_REG_SIZE, GPIO_BASE);

	if(gpio_base == MAP_DEVICE_FAILED)
	{
			perror("Can't map device I/O");
			return 0;
	}
	val=in32(gpio_base+LED_GPFSEL_RED);
	val&=~(7<<LED_GPFBIT_RED);
	val|=1<<LED_GPFBIT_RED;
	out32(gpio_base+LED_GPFSEL_RED,val);

	while(1)
	{
		delay(1000);
		out32(gpio_base+LED_GPCLR,1<<LED_GPIO_BIT_RED);
		delay(1000);
		out32(gpio_base+LED_GPSET,1 << LED_GPIO_BIT_RED);
	}

	munmap_device_io(gpio_base, GPIO_REG_SIZE);
	return EXIT_SUCCESS;
}
