#include <stddef.h>
#include "stm32f030x6.h"
#include "uart.h"

static char uartTxBuff[UART_TX_BUFF_SIZE];
static unsigned int uartTxBuffWriteIndex = 0;
static unsigned int uartTxBuffReadIndex = 0;

struct uartRxData uartRxData = {{"", ""}, {0, 0}, 0, 0, -1};

void uart1Setup()
{
	//#define BRR_VAL 129 //calculated value
#define BRR_VAL 140  //corrected fro osciloscop measure value
	USART1->CR1 &= ~(USART_CR1_UE); //Disable uart
	USART1->ICR = 0xFFFFFFFF; //Clear all pending interrupt flags

	USART1->BRR |= (BRR_VAL & 0xFFF0) | ((BRR_VAL & 0x000F) >> 1); // baudrate set to 115200 assuming 8MHz clk and OSER8 set to 1

	USART1->CR1 |= USART_CR1_OVER8 //set oversampling to 8
		| USART_CR1_UE  // Uart enabled
		| USART_CR1_TE  // transmit enable
		| USART_CR1_RE  // receive enable
		| USART_CR1_RXNEIE; // Interrupt on receive eneable

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
		USART1->ICR = USART_ICR_TCCF; //reset Transmission complete flag
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

	if (USART1->ISR & USART_ISR_RXNE) //RXNE (rx buffer not empty) interrupt
	{
		char data = USART1->RDR;
		uartRxData.buff[uartRxData.buffToWriteNum][uartRxData.buffWriteIndex++] = data;
		if (uartRxData.buffWriteIndex >= UART_RX_BUFF_SIZE || data == '\r' || data == '\n')
		{
			if(uartRxData.buffWriteIndex == 1) //'\r' or '\n' is the first and only character, ignore the line
			{
				uartRxData.buffWriteIndex = 0;
				return;
			}
			uartRxData.buff[uartRxData.buffToWriteNum][uartRxData.buffWriteIndex] = '\0';
			uartRxData.buffToReadNum = uartRxData.buffToWriteNum;
			uartRxData.dataSize[uartRxData.buffToWriteNum] = uartRxData.buffWriteIndex;
			uartRxData.buffToWriteNum = uartRxData.buffToWriteNum ? 0 : 1;
			uartRxData.buffWriteIndex = 0;
		}
	}
}

char* uartBufferToRead(unsigned int * const dataSize)
{
	int buffToReadNum = uartRxData.buffToReadNum;

	if(buffToReadNum == -1)
	{
		if (dataSize != NULL)
			*dataSize = 0;
		return NULL;
	}

	if (dataSize != NULL)
		*dataSize = uartRxData.dataSize[buffToReadNum];

	return  uartRxData.buff[buffToReadNum];
}

void uartBufferTreated()
{
	if (uartRxData.buffToReadNum == uartRxData.buffToWriteNum)  //the other buff is already full
	{
		uartRxData.buffToReadNum = uartRxData.buffToWriteNum ? 0 : 1;
	}
	else
		uartRxData.buffToReadNum = -1;
}
