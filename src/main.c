#include <stdint.h>
#include "stm32f030x6.h"
#include "init.h"
#include "uart.h"
#include "mini_printf.h"

void main (void)
{
	clkSetup();
	gpioSetup();
	uart1Setup();

	volatile int i;

	for(;;)
	{
		GPIOB->BSRR = GPIO_BSRR_BS_1; // set PB1
		for (i=0; i<0x0FFF; i++);
		GPIOB->BSRR = GPIO_BSRR_BR_1; // reset PB1
		for (i=0; i<0x0FFF; i++);
		mini_printf("tataplop");
	}

}
