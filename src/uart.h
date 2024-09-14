#ifndef __UART_h__
#define __UART_h__

#define UART_TX_BUFF_SIZE 30
#define UART_RX_BUFF_SIZE 30

struct uartRxData {
    char buff[2][UART_RX_BUFF_SIZE+1];
    unsigned int dataSize[2];
    unsigned char buffToWriteNum;
    unsigned int buffWriteIndex;
    int buffToReadNum;
};
//Reade available data from buff[buffToWriteNum];

void uart1_INTERRUPT();
void uart1Setup();
void uart1PutC(char C);

char* uartBufferToRead(unsigned int * const dataSize);
void uartBufferTreated();

#endif
