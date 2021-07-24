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
	Encoder_Init();

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
		if(TIM3->CNT <= 50)
		{
			GPIOG->BSRR |= GPIO_BSRR_BS13;
		} 
		else
		{
			GPIOG->BSRR |= GPIO_BSRR_BR13;
		}
		
		if(TIM3->CNT > 50)
		{
			GPIOG->BSRR |= GPIO_BSRR_BS14;
		} 
		else
		{
			GPIOG->BSRR |= GPIO_BSRR_BR14;
		}
		
		vTaskDelay(1000);
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
