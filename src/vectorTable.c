#include <stddef.h>
#include "uart.h"

void _reset_handler(void);

void (*__interupt__vectors__[]) (void) __attribute__((section(".vector_table"))) =
{

	_reset_handler,		//Reset 		Reset 																						0x0000 0004
	_reset_handler,		//NMI 			Non maskable interrupt. The RCC clock security system (CSS) is linked to the NMI vector. 	0x0000 0008
	_reset_handler,		//HardFault 	All classes of fault 																		0x0000 000C
	NULL,				//Reserved		-																							0x0000_0010
	NULL,				//Reserved		-																							0x0000_0014
	NULL,				//Reserved		-																							0x0000_0018
	NULL,				//Reserved		-																							0x0000_001c
	NULL,				//Reserved		-																							0x0000_0020
	NULL,				//Reserved		-																							0x0000_0024
	NULL,				//Reserved		-																							0x0000_0028
	_reset_handler,		//SVCall 		System service call via SWI instruction 													0x0000 002C
	NULL,				//Reserved		-																							0x0000_0030
	NULL,				//Reserved		-																							0x0000_0034
	_reset_handler,		//PendSV 		Pendable request for system service 														0x0000 0038 
	NULL,				//SysTick 		System tick timer 																			0x0000 003C
	NULL,				//WWDG 			Window watchdog interrupt 																	0x0000 0040
	NULL,				//Reserved 		- 																							0x0000 0044
	NULL,				//RTC 			RTC interrupts (combined EXTI lines 17, 19 and 20) 											0x0000 0048
	NULL,				//FLASH 		Flash global interrupt 																		0x0000 004C
	NULL,				//RCC 			RCC global interrupts 																		0x0000 0050
	NULL,				//EXTI0_1 		EXTI Line[1:0] interrupts 																	0x0000 0054
	NULL,				//EXTI2_3 		EXTI Line[3:2] interrupts 																	0x0000 0058
	NULL,				//EXTI4_15 		EXTI Line[15:4] interrupts 																	0x0000 005C
	NULL,				//Reserved 		- 																							0x0000 0060
	NULL,				//DMA_CH1 		DMA channel 1 interrupt 																	0x0000 0064
	NULL,				//DMA_CH2_3 		DMA channel 2 and 3 interrupts 															0x0000 0068
	NULL,				//DMA_CH4_5 		DMA channel 4 and 5 interrupts 															0x0000 006C
	NULL,				//ADC 			ADC interrupts 																				0x0000 0070
	NULL,				//TIM1_BRK_UP_ 		TRG_COM TIM1 break, update, trigger and commutation interrupt 							0x0000 0074
	NULL,				//TIM1_CC 		TIM1 capture compare interrupt 																0x0000 0078
	NULL,				//Reserved 		- 																							0x0000 007C
	NULL,				//TIM3 			TIM3 global interrupt 																		0x0000 0080
	NULL,				//TIM6 			TIM6 global interrupt 																		0x0000 0084
	NULL,				//Reserved 		- 																							0x0000 0088
	NULL,				//TIM14 		TIM14 global interrupt 																		0x0000 008C
	NULL,				//TIM15 		TIM15 global interrupt 																		0x0000 0090
	NULL,				//TIM16 		TIM16 global interrupt 																		0x0000 0094
	NULL,				//TIM17 		TIM17 global interrupt 																		0x0000 0098
	NULL,				//I2C1 			I 2 C1 global interrupt 																	0x0000 009C
	NULL,				//I2C2 			I 2C2 global interrupt 																		0x0000 00A0
	NULL,				//SPI1 			SPI1 global interrupt 																		0x0000 00A4
	NULL,				//SPI2 			SPI2 global interrupt 																		0x0000 00A8
	uart1_INTERRUPT,				//USART1 		USART1 global interrupt 																	0x0000 00AC
	NULL,				//USART2 		USART2 global interrupt 																	0x0000 00B0
	NULL,				//USART3_4_5_6 		USART3, USART4, USART5, USART6 global interrupts 										0x0000 00B4
	NULL,				//Reserved 		- 																							0x0000 00B8
	NULL,				//USB 			USB global interrupt (combined with EXTI line 18) 											0x0000 00BC
};
