
#include "stm32f429xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


//-------------------------------------------------------------
void GPIO_Init (void);
void vTaskLed1 (void *argument);


//-------------------------------------------------------------


int main(void)
{

	GPIO_Init();
	
	xTaskCreate(vTaskLed1, "LED", 4096, NULL, 1, NULL );
	
  vTaskStartScheduler();
	
	while(1)
	{

	}
}

//---------------------------------------------------------------

void GPIO_Init (void)
{
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN; 
			GPIOG->MODER |= GPIO_MODER_MODE13_0;
			GPIOG->OTYPER |= 0x00000000;
			GPIOG->OSPEEDR |= GPIO_OSPEEDR_OSPEED13_0;
			GPIOG-> PUPDR &= GPIO_PUPDR_PUPDR13;
			
			GPIOG->MODER |= GPIO_MODER_MODE14_0;
			GPIOG->OSPEEDR |= GPIO_OSPEEDR_OSPEED14_0;
			GPIOG-> PUPDR &= GPIO_PUPDR_PUPDR14;
}


void vTaskLed1 (void *argument)
{
	while(1)
	{
			GPIOG-> ODR |= GPIO_ODR_OD13_Msk;
			GPIOG-> ODR &= ~GPIO_ODR_OD14;
			vTaskDelay(1000);
			GPIOG-> ODR |= GPIO_ODR_OD14_Msk;
			GPIOG-> ODR &= ~GPIO_ODR_OD13;
			vTaskDelay(1000);
	}
}
