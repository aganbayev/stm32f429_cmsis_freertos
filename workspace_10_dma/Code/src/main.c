/****************************************Library*****************************************/

#include "main.h"


/*****************************************Main*******************************************/

int main(void)
{
	RCC_Init();
	GPIO_Init();
	UART_Init();
	PWM_Init();
	ADC_Init();
	DAC_Init();
	DMA_Init();

	xTaskCreate(vTaskDMA, "DMA", 128, NULL, 1, NULL );
	xTaskCreate(vTaskDMA2, "DMA2", 128, NULL, 1, NULL );
	
	/*
	uint32_t CountRemove = 0;
	FlashUnlock();
	if(FlashReadData(0x081E0000) == 0x00000005) {FlashAllErase();}
	else {
	CountRemove = FlashReadData(0x081E0000);
	CountRemove += 1;
	FlashSectorErase();
	FlashWriteData(0x081E0000, CountRemove);
	}
	FlashLock();
	*/
	
  vTaskStartScheduler();
	
	while(1)
	{

	}
}

/****************************************Tasks******************************************/


void vTaskDMA (void *argument)
{

	while(1)
	{
		WriteDMAuart1();
		vTaskDelay(1000);
	}  
}

void vTaskDMA2 (void *argument)
{

	while(1)
	{
		//WriteDMAuart1();
		vTaskDelay(100);
	}
}
/**************************************Handlers****************************************/

void USART1_IRQHandler(void)
{
		if((USART1->SR & USART_SR_RXNE) && (USART1->CR1 & USART_CR1_RXNEIE))
		{
			USART1->SR &= ~USART_SR_RXNE;
			
		}
}
