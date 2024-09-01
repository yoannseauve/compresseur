#include "stm32f030x6.h"
#include "uart.h"

static char uartRxBuff[UART_RX_BUFF_SIZE];
static unsigned int uartRxBuffWriteIndex = 0;
static unsigned int uartRxBuffReadIndex = 0;

static char uartTxBuff[UART_TX_BUFF_SIZE];
static unsigned int uartTxBuffWriteIndex = 0;
static unsigned int uartTxBuffReadIndex = 0;

void uart1Setup()
{
	//#define BRR_VAL 129 //calculated value
	#define BRR_VAL 140  //corrected fro osciloscop measure value
	USART1->CR1 &= ~(USART_CR1_UE); //Disable uart
	USART1->ICR = 0xFFFFFFFF; //Clear all pending interrupt flags

	USART1->BRR |= (BRR_VAL & 0xFFF0) | ((BRR_VAL & 0x000F) >> 1); // baudrate set to 115200 assuming 8MHz clk and OSER8 set to 1

	USART1->CR1 |= USART_CR1_OVER8 //set oversampling to 8
		| USART_CR1_UE  // Uart enabled
		| USART_CR1_TE;  //transmit enable

	NVIC_SetPriority(USART1_IRQn, 0x03); //uart1 interrupt at low priority
	NVIC_EnableIRQ(USART1_IRQn); //uart1 interrupt enable	
}

void uart1PutC(char c)
{
	if (uartTxBuffWriteIndex == (UART_TX_BUFF_SIZE - 1)) //write pointer to the end of buffer
		while (uartTxBuffReadIndex == 0); //whait for space in the TX buffer
	else
		while (uartTxBuffReadIndex == (uartTxBuffWriteIndex + 1)); //whait for space in the TX buffer

	uartTxBuff[uartTxBuffWriteIndex++] = c;
	if (uartTxBuffWriteIndex == UART_TX_BUFF_SIZE)
		uartTxBuffWriteIndex = 0;

	if (!(USART1->CR1 & USART_CR1_TXEIE)) // if interrupt on TXE is disabled
	{
		if (uartTxBuffWriteIndex != uartTxBuffReadIndex) //still data to send
		{
			USART1->TDR = uartTxBuff[uartTxBuffReadIndex++];
			if (uartTxBuffReadIndex == UART_TX_BUFF_SIZE)
				uartTxBuffReadIndex = 0;
		}
		USART1->CR1 |= USART_CR1_TXEIE; //enable interrupt on TXE
		USART1->ICR = USART_ICR_TCCF; //reset Transmission complete fral
	}

}

void uart1_INTERRUPT()
{
	if (USART1->ISR & USART_ISR_TXE) //TXE (tx buffer empty) interrupt
	{
		if (uartTxBuffWriteIndex != uartTxBuffReadIndex) //still data to send
		{
			USART1->TDR = uartTxBuff[uartTxBuffReadIndex++];
			if (uartTxBuffReadIndex == UART_TX_BUFF_SIZE)
				uartTxBuffReadIndex = 0;
		}
		else 
			USART1->CR1 &= ~USART_CR1_TXEIE; //disable interrupt on TXE
	}
}
