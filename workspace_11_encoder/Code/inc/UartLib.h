/**************************************Library****************************************/

#include "stm32f429xx.h"

/***********************************Use function**************************************/
void UART_Init(void);

/*************************************USART1******************************************/
void USART1_Init(void);
void USART1_Send(char chr);
void USART1_Send_Data(uint8_t data);
void USART1_Send_String(char* str);

/*************************************USART2******************************************/

void USART2_Init(void);
void USART2_Send(char chr);
void USART2_Send_Data(uint8_t data);
void USART2_Send_String(char* str);
