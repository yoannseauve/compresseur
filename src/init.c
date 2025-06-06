#include "stm32f030x6.h"

void clkSetup()
{

	RCC->CFGR3 |= RCC_CFGR3_USART1SW_0 | RCC_CFGR3_USART1SW_1; //HSI (8MHz) set as uart1 clk
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN    //enable GPIO port A clk
		| RCC_AHBENR_GPIOBEN;    //enable GPIO port B clk
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; //enable TIM3 clk
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN | RCC_APB2ENR_ADC1EN; //enable USART1 and ADC1 clk
	RCC->CR2 |= RCC_CR2_HSI14ON; //Start HSI14 RC oscillator
	while ((RCC->CR2 & RCC_CR2_HSI14RDY) == 0);

}

void gpioSetup()
{
	GPIOA->BSRR = GPIO_BSRR_BS_7; //PA7 output set to 1 (will be configured as open drain)

	GPIOB->MODER |= GPIO_MODER_MODER1_0; // PB1 set as output

	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR1_0 | GPIO_OSPEEDER_OSPEEDR1_1; // PB1 set to high speed
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR7_0 | GPIO_OSPEEDER_OSPEEDR7_1 // PA7 (triac triger) set to high speed
		| GPIO_OSPEEDER_OSPEEDR9_0 | GPIO_OSPEEDER_OSPEEDR9_1 // PA9 (UART1TX) set to high speed
		| GPIO_OSPEEDER_OSPEEDR10_0 | GPIO_OSPEEDER_OSPEEDR10_1;	// PA10 (UART1RX) set to high speed

	GPIOA->MODER |= GPIO_MODER_MODER6_1 //PA6 set to alternate function (TIM3)
		| GPIO_MODER_MODER7_0 //PA7 set as output (triac triger)
		| GPIO_MODER_MODER9_1 //PA9 set to alternate function (UART1TX)
		| GPIO_MODER_MODER10_1; //PA10 set to alternate function (UART1RX)
		
	GPIOA->MODER &= ~(GPIO_MODER_MODER6_0) //PA6 set to alternate function (TIM3)
		& ~(GPIO_MODER_MODER7_1) //PA7 set as output (triac triger)
		& ~(GPIO_MODER_MODER9_0) //PA9 set to alternate function (UART1TX)
		& ~(GPIO_MODER_MODER10_0); //PA10 set to alternate function (UART1RX)

	GPIOA->OTYPER &= GPIO_OTYPER_OT_7; //PA7 set to open drain (triac triger)

	GPIOA->AFR[1] |= 0x01<<GPIO_AFRH_AFSEL9_Pos //set PA9 to AF1 (UART1TX)
		| 0x01<<GPIO_AFRH_AFSEL10_Pos;	//set PA10 to AF1 (UART1RX)
	GPIOA->AFR[1] &= (0x01<<GPIO_AFRH_AFSEL9_Pos | ~GPIO_AFRH_AFSEL9_Msk) //set PA9 to AF1 (UART1TX)
		&(0x01<<GPIO_AFRH_AFSEL10_Pos | ~GPIO_AFRH_AFSEL10_Msk);	//set PA10 to AF1 (UART1RX)
	GPIOA->AFR[0] |=0x01<<GPIO_AFRL_AFSEL6_Pos;   //set PA6 to AF1 (TIM3_CH1)
	GPIOA->AFR[0] &= (0x01<<GPIO_AFRL_AFSEL6_Pos | ~GPIO_AFRL_AFSEL6_Msk);  //set PA6 to AF1 (TIM3_CH1)

	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR10_0 //activates pull-up on PA10 (UART1RX)
		| GPIO_PUPDR_PUPDR6_1; //activate pull-down on PA6 (TIM3-CH1)
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR10_1) //activates pull-up on PA10 (UART1RX)
		& ~(GPIO_PUPDR_PUPDR6_0); //activate pull-down on PA6 (TIM3-CH1)

	GPIOA->MODER &= ~(GPIO_MODER_MODER0_0 | GPIO_MODER_MODER0_1); //set PA0 as floating analog input
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR0_0 | GPIO_PUPDR_PUPDR0_1);
}
