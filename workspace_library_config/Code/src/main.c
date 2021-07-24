/**************************************Library****************************************/

#include "main.h"

/**************************************Main****************************************/

int main(void)
{

	RCC_Init();
	GPIO_Init();
	UART_Init();
	PWM_Init();
	
	xTaskCreate(vTaskButton1, "Button", 128, NULL, 1, NULL );
	xTaskCreate(vTaskLed1, "LED", 128, NULL, 1, NULL );
	
  vTaskStartScheduler();
	
	while(1)
	{

	}
}

/**************************************Tasks****************************************/

void vTaskButton1 (void *argument)
{
	while(1)
	{
		if((GPIOA->IDR & GPIO_IDR_ID0) != 0)   // button push 
		{
				USART1_Send_String("Button\r\n");
		}
				vTaskDelay(200);
	}
}

void vTaskLed1 (void *argument)
{
	while(1)
	{
			GPIOG-> ODR ^= GPIO_ODR_OD14;
			vTaskDelay(1000);
			GPIOG-> ODR ^= GPIO_ODR_OD14;
			GPIOG-> ODR ^= GPIO_ODR_OD13;
			vTaskDelay(1000);
			GPIOG-> ODR ^= GPIO_ODR_OD13;

	}
}

/**************************************Handlers****************************************/

void USART1_IRQHandler(void)
{
		if((USART1->SR & USART_SR_RXNE) && (USART1->CR1 & USART_CR1_RXNEIE))
		{
			USART1->SR &= ~USART_SR_RXNE;
			if(USART1->DR == '1')
			{
				USART1_Send_String("that is one\r\n");
			}
		  if(USART1->DR == '2')
			{
				USART1_Send_String("that is two\r\n");
			}	
		}
}
