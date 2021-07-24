
#include "stm32f429xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


//-------------------------------------------------------------
void RCC_Init (void);
void GPIO_Init (void);
void UART_Init(void);
void USART1_Send(char chr);
void USART1_Send_String(char* str);
void vTaskButton1 (void *argument);
void vTaskLed1 (void *argument);

//-------------------------------------------------------------


int main(void)
{

	RCC_Init();
	GPIO_Init();
	UART_Init();
	
	xTaskCreate(vTaskButton1, "Button", 128, NULL, 1, NULL );
	xTaskCreate(vTaskLed1, "LED", 128, NULL, 1, NULL );
	
  vTaskStartScheduler();
	
	while(1)
	{

	}
}

//---------------------------------------------------------------

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

//----------------------------------------------------------------

void RCC_Init (void)
{
	RCC->CR |= ((uint32_t) RCC_CR_HSEON); // Enable HSE;
		while (!(RCC->CR & RCC_CR_HSERDY));   // Ready start HSE;
	
	FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY;   // Clock flash memory; 
			
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;      // AHB = SYSCLK /1;
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;     // APB1 = HCLK/2;
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;     // APB2 = HCLK/2;
	
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLM;    // CLEAR THIS BITS FOR CONFIGURING;
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLN;
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLP;    // 00- DIVISION P TO 2   --- 180 MHz / 2 = 90 AHB
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLSRC;
	
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE; // Source HSE = 8 Mhz;
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLM_3;     // HSE / 8 = 1 Mhz;
	
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLN_2;     // Set prescaler to 180 - 1 * 180 Mhz;
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLN_4;
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLN_5;
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLN_7;

	
	RCC->CR |= RCC_CR_PLLON;    // Enable PLL;
		while((RCC->CR & RCC_CR_PLLRDY) == 0) {}          // Wait till PLL is ready;
	
	RCC->CFGR &= RCC_CFGR_SW;   // Clear SW bits;
	RCC->CFGR |= RCC_CFGR_SW_PLL;   // select PLL as SYSCLK;
		while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_1) {}
			
}

void GPIO_Init (void)
{
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN; // Clock on 
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	
			GPIOG->MODER |= GPIO_MODER_MODE13_0;
			GPIOG->OTYPER |= 0x00000000;
			GPIOG->OSPEEDR |= GPIO_OSPEEDR_OSPEED13_0;
			GPIOG->PUPDR &= ~GPIO_PUPDR_PUPDR13;
			
			GPIOG->MODER |= GPIO_MODER_MODE14_0;
			GPIOG->OSPEEDR |= GPIO_OSPEEDR_OSPEED14_0;
			GPIOG->PUPDR &= ~GPIO_PUPDR_PUPDR14;
	
			GPIOA->MODER &= ~GPIO_MODER_MODE0;
			GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR0;
	
			
}

void UART_Init(void)
{
			RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	
			GPIOA->MODER |= GPIO_MODER_MODE9_1;    // PA9- pp mode, alternate function
			GPIOA->OTYPER &= ~GPIO_OTYPER_OT9;
			GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED9_0;
			GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR9;
	
			GPIOA->MODER |= GPIO_MODER_MODE10_1;    // PA10- pp mode, alternate function
			GPIOA->OTYPER &= ~GPIO_OTYPER_OT10;
			GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED10_0;
			GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR10;

			GPIOA->AFR[1] |= 0x00000770;  //AF init for usart in pa9 and pa10
	
			//GPIOA->MODER &= ~GPIO_MODER_MODE10;    // PA10 - input mode with floating point
			//GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR10;
	
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

void USART1_Send_String(char* str)
{
			uint8_t i = 0;
			while(str[i])
	{
			USART1_Send(str[i++]);
	}
}

void USART1_IRQHandler(void)
{
		if((USART1->SR & USART_SR_RXNE) && (USART1->CR1 & USART_CR1_RXNEIE))
		{
			USART1->SR &= ~USART_SR_RXNE;
			if(USART1->DR == '1')
			{
				USART1_Send_String("that is one\r\n");
			}
		  if(USART1->DR == '0')
			{
				USART1_Send_String("that is two\r\n");
			}	
		}
}
