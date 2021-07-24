/**************************************Library****************************************/

#include "UartLib.h"

/***********************************Use function**************************************/

void UART_Init(void)
{
	USART1_Init();
	//USART2_Init();
	
}

/*************************************USART1******************************************/

void USART1_Init(void)
{
			RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;    //Enable clock for port A
	
			GPIOA->MODER |= GPIO_MODER_MODE9_1;    // PA9- pp mode, alternate function
			GPIOA->OTYPER &= ~GPIO_OTYPER_OT9;
			GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED9_0;
			GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR9;
	
			GPIOA->MODER |= GPIO_MODER_MODE10_1;    // PA10- pp mode, alternate function
			GPIOA->OTYPER &= ~GPIO_OTYPER_OT10;
			GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED10_0;
			GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR10;

			GPIOA->AFR[1] |= 0x00000770;  //AF init for usart in pa9 and pa10  - AFRH
	
			USART1->BRR = 0x2496;     // Speed Calculated with equation :     (Fclk + Baudrate/2) / Baudrate;   or div = Fclk / (8*(2|1)*Baudrate)		
	
			USART1->CR1 |= USART_CR1_UE;
			USART1->CR1 |= USART_CR1_TE;
			USART1->CR1 |= USART_CR1_RE;
	
			USART1->CR1 |= USART_CR1_RXNEIE;   // Enable interrupt mode for receive -RX
			NVIC_EnableIRQ(USART1_IRQn);
	
}

void USART1_Send(char chr)
{
			while(!(USART1->SR & USART_SR_TC));
			USART1->DR = chr;
	
}

void USART1_Send_Data(uint8_t data)
{
			while(!(USART1->SR & USART_SR_TC));
			USART1->DR = data;
	
}

void USART1_Send_String(char* str)
{
			uint8_t i = 0;
			while(str[i])
	{
			USART1_Send(str[i++]);
	}
}

/*************************************USART2******************************************/

void USART2_Init(void)
{
			RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;    //Enable clock for port A
	
			GPIOA->MODER |= GPIO_MODER_MODE2_1;    // PA2- pp mode, alternate function
			GPIOA->OTYPER &= ~GPIO_OTYPER_OT2;
			GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED2_0;
			GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR2;
	
			GPIOA->MODER |= GPIO_MODER_MODE3_1;    // PA3- pp mode, alternate function
			GPIOA->OTYPER &= ~GPIO_OTYPER_OT3;
			GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED3_0;
			GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR3;

			GPIOA->AFR[0] |= 0x00007700;  //AF init for usart in pa2 and pa3  - AFRL
	
			USART2->BRR = 0x2496;     // Speed Calculated with equation :     (Fclk + Baudrate/2) / Baudrate;   or div = Fclk / (8*(2|1)*Baudrate)		
	
			USART2->CR1 |= USART_CR1_UE;
			USART2->CR1 |= USART_CR1_TE;
			USART2->CR1 |= USART_CR1_RE;
	
			USART2->CR1 |= USART_CR1_RXNEIE;   // Enable interrupt mode for receive -RX
			NVIC_EnableIRQ(USART2_IRQn);
	
}

void USART2_Send(char chr)
{
			while(!(USART2->SR & USART_SR_TC));
			USART2->DR = chr;
	
}

void USART2_Send_Data(uint8_t data)
{
			while(!(USART2->SR & USART_SR_TC));
			USART2->DR = data;
	
}

void USART2_Send_String(char* str)
{
			uint8_t i = 0;
			while(str[i])
	{
			USART2_Send(str[i++]);
	}
}



