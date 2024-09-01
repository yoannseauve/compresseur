#ifndef __UART_h__
#define __UART_h__

#define UART_TX_BUFF_SIZE 30
#define UART_RX_BUFF_SIZE 30

void uart1_INTERRUPT();
void uart1Setup();
void uart1PutC(char C);

#endif
