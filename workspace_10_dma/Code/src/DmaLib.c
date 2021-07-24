/**************************************Library****************************************/

#include "DmaLib.h"

/***********************************Use function**************************************/
char buffer[] = "I am DMA!\r\n";

void DMA_Init(void)
{
		DMA2uart1_Init();
	
}

/************************************DMA-USART1***************************************/

void DMA2uart1_Init(void)
{
			RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;    //Enable clock for DMA1
	
			DMA2_Stream7-> PAR= (uint32_t)&USART1->DR;
			DMA2_Stream7-> M0AR = (uint32_t)buffer;
			DMA2_Stream7-> NDTR =  128;														// Size buffer
	
			DMA2_Stream7->CR |= DMA_SxCR_CHSEL_2;
	
			DMA2_Stream7->CR  &= ~DMA_SxCR_CIRC;								// Disenable cycle mode
			DMA2_Stream7->CR  &= ~DMA_SxCR_PINC;								// Disenable increment pointer periphery
			
			DMA2_Stream7->CR  &= ~DMA_SxCR_PSIZE;								// Size data periphery - 8 bit
			DMA2_Stream7->CR  &= ~DMA_SxCR_MSIZE; 							// Size data memory - 8 bit
			DMA2_Stream7->CR  |=  DMA_SxCR_DIR_0;									// Read: memory -> periphery
			DMA2_Stream7->CR  |=  DMA_SxCR_MINC;								// Enable increment pointer memory
	
			USART1->CR3         |=  USART_CR3_DMAT;								// Enable DMA for USART1
}
	
void WriteDMAuart1 (void)
{
	
	DMA2_Stream7->CR  &= ~DMA_SxCR_EN;      
  DMA2_Stream7->NDTR =  (sizeof(buffer)-1);      
  DMA2->HIFCR |=  DMA_HIFCR_CTCIF7; 							// Status flag end of exchange
  DMA2_Stream7->CR  |=  DMA_SxCR_EN;
	
}
