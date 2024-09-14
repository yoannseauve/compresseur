#include <stddef.h>
#include <stdint.h>
#include "stm32f030x6.h"
#include "init.h"
#include "uart.h"
#include "mini_printf.h"
#include "adc.h"

void main (void)
{
	clkSetup();
	gpioSetup();
	uart1Setup();
	adc_init();


	volatile int i;
	char* uartRx;
	

	for(;;)
	{
		GPIOB->BSRR = GPIO_BSRR_BS_1; // set PB1
		for (i=0; i<0x0FFFF; i++);
		GPIOB->BSRR = GPIO_BSRR_BR_1; // reset PB1
		for (i=0; i<0x0FFFF; i++);

	mini_printf("> %d\n", adc_read_temp());
	}

}
