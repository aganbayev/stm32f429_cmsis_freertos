
#include "stm32f429xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


//-------------------------------------------------------------
void RCC_init (void);
void GPIO_Init (void);
void vTaskLed1 (void *argument);
void vTaskLed2 (void *argument);

//-------------------------------------------------------------


int main(void)
{

	RCC_init();
	GPIO_Init();
	
	xTaskCreate(vTaskLed1, "LED1", 128, NULL, 2, NULL );
	xTaskCreate(vTaskLed2, "LED2", 128, NULL, 1, NULL );
	
  vTaskStartScheduler();
	
	while(1)
	{

	}
}

//---------------------------------------------------------------
void RCC_init (void)
{
	RCC->CR |= ((uint32_t) RCC_CR_HSEON); // Enable HSE;
		while (!(RCC->CR & RCC_CR_HSERDY));   // Ready start HSE;
	
	FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY;   // Clock flash memory; 
	
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;      // AHB = SYSCLK /1;
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;     // APB1 = HCLK/2;
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;     // APB2 = HCLK/2;
	
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLM;    // CLEAR THIS BITS FOR CONFIGURING;
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLN;
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLP;    // 00- DIVISION P TO 2
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLSRC;
	
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE; // Source HSE = 8 Mhz;
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLM_3;     // HSE / 8 = 1 Mhz;
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLN_3;     // Set prescaler to 180 - 1 * 180 Mhz;
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLN_5;
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLN_6;
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLN_8;
	
	RCC->CR |= RCC_CR_PLLON;    // Enable PLL;
		while((RCC->CR & RCC_CR_PLLRDY) == 0) {}          // Wait till PLL is ready;
	
	RCC->CFGR &= RCC_CFGR_SW;   // Clear SW bits;
	RCC->CFGR |= RCC_CFGR_SW_PLL;   // select PLL as SYSCLK;
		while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_1) {}
			
}

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
			vTaskDelay(1000);
			GPIOG-> ODR &= ~GPIO_ODR_OD13;
			vTaskDelay(1000);
	}
}

void vTaskLed2 (void *argument)
{
	while(1)
	{
			GPIOG-> ODR &= ~GPIO_ODR_OD14;
			vTaskDelay(1000);
			GPIOG-> ODR |= GPIO_ODR_OD14_Msk;
			vTaskDelay(1000);
	}
}
