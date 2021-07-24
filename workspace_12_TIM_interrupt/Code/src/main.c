/****************************************Library*****************************************/

#include "main.h"


/*****************************************Main*******************************************/

int main(void)
{
	RCC_Init();
	GPIO_Init();
	UART_Init();
	//PWM_Init();
	//ADC_Init();
	//DAC_Init();
	//DMA_Init();
	//Encoder_Init();
	Tim2_Init();

	xTaskCreate(vTaskEncoder, "Encoder", 128, NULL, 1, NULL );
	
  vTaskStartScheduler();
	
	while(1)
	{

	}
}

/****************************************Tasks******************************************/


void vTaskEncoder (void *argument)
{

	while(1)
	{
			
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

void TIM2_IRQHandler(void)
{
		TIM2->SR &= ~TIM_SR_UIF;
		
		GPIOG->ODR ^= ~(GPIO_ODR_OD13);
		GPIOG->ODR ^= ~(GPIO_ODR_OD14);
		
}

