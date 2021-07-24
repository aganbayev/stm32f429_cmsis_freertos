
#include "stm32f429xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


//-------------Functions------------------------------------------------
void RCC_Init (void);
void GPIO_Init (void);
void UART_Init(void);
void USART1_Send(char chr);
void USART1_Send_String(char* str);
void PWM_Init(void);

//Tasks
void vTaskButton1 (void *argument);
void vTaskLed1 (void *argument);

//-------------------------------------------------------------


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
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN; // Clock on for port G
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
		  if(USART1->DR == '2')
			{
				USART1_Send_String("that is two\r\n");
			}	
		}
}

void PWM_Init(void)
{
			//----------------------TIM 2
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  // Enable clock for port A
		RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;   // Enable clock for TIM2
		
		GPIOA->MODER &= ~GPIO_MODER_MODE5;
		GPIOA->MODER |= GPIO_MODER_MODE5_1;    // PA5- pp mode, alternate function
		GPIOA->OTYPER &= ~GPIO_OTYPER_OT5;
		GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED5;
		GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR5;
		GPIOA->AFR[0] |= 0x00100000;  //AF init for Tim2 in pa5  - AFRL
	
		TIM2->PSC = 45000 - 1;
		TIM2->ARR = 1000;
		TIM2->CCR1 = 200;
	
		TIM2->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2; 
		TIM2->CCER |= TIM_CCER_CC1E;
		TIM2->CCER &= ~TIM_CCER_CC1P;
	
		TIM2->CR1 &= ~TIM_CR1_DIR;
		TIM2->CR1 |= TIM_CR1_CEN;
		
		
		//----------------------TIM 1
		
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;  // Enable clock for port B
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;  // Enable clock for port E
		RCC -> APB2ENR |= RCC_APB2ENR_TIM1EN; 
		
		GPIOA->MODER &= ~GPIO_MODER_MODE7;
		GPIOA->MODER |= GPIO_MODER_MODE7_1;    // PA7- pp mode, alternate function       TIM CH1N
		GPIOA->OTYPER &= ~GPIO_OTYPER_OT7;
		GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED7;
		GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR7;
		GPIOA->AFR[0] |= 0x10000000;  //AF init for Tim2 in pa7  - AFRL
		
		GPIOB->MODER &= ~GPIO_MODER_MODE0;
		GPIOB->MODER |= GPIO_MODER_MODE0_1;    // PB0- pp mode, alternate function       TIM CH2N
		GPIOB->OTYPER &= ~GPIO_OTYPER_OT0;
		GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED0;
		GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR0;
		GPIOB->AFR[0] |= 0x00000001;  //AF init for Tim2 in pb0  - AFRL
		
		GPIOB->MODER &= ~GPIO_MODER_MODE1;
		GPIOB->MODER |= GPIO_MODER_MODE1_1;    // PB1- pp mode, alternate function       TIM CH3N
		GPIOB->OTYPER &= ~GPIO_OTYPER_OT1;
		GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED1;
		GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR1;
		GPIOB->AFR[0] |= 0x00000010;  //AF init for Tim2 in pb1  - AFRL
		
		GPIOE->MODER &= ~GPIO_MODER_MODE9;
		GPIOE->MODER |= GPIO_MODER_MODE9_1;    // PE9- pp mode, alternate function       TIM CH1
		GPIOE->OTYPER &= ~GPIO_OTYPER_OT9;
		GPIOE->OSPEEDR |= GPIO_OSPEEDR_OSPEED9;
		GPIOE->PUPDR &= ~GPIO_PUPDR_PUPDR9;
		GPIOE->AFR[1] |= 0x00000010;  //AF init for Tim2 in pE9  - AFRL
		
		GPIOE->MODER &= ~GPIO_MODER_MODE11;
		GPIOE->MODER |= GPIO_MODER_MODE11_1;    // PE11- pp mode, alternate function     TIM CH2
		GPIOE->OTYPER &= ~GPIO_OTYPER_OT11;
		GPIOE->OSPEEDR |= GPIO_OSPEEDR_OSPEED11;
		GPIOE->PUPDR &= ~GPIO_PUPDR_PUPDR11;
		GPIOE->AFR[1] |= 0x00001000;  //AF init for Tim2 in pE11  - AFRL
		
		GPIOE->MODER &= ~GPIO_MODER_MODE13;
		GPIOE->MODER |= GPIO_MODER_MODE13_1;    // PE13- pp mode, alternate function      TIM CH3
		GPIOE->OTYPER &= ~GPIO_OTYPER_OT13;
		GPIOE->OSPEEDR |= GPIO_OSPEEDR_OSPEED13;
		GPIOE->PUPDR &= ~GPIO_PUPDR_PUPDR13;
		GPIOE->AFR[1] |= 0x00100000;  //AF init for Tim2 in pE13  - AFRL

		TIM1->CR1   |=  TIM_CR1_CMS_0;    //Center-aligned mode 1
		TIM1->CR1   |=  TIM_CR1_ARPE; 


		TIM1->PSC  = 45000-1;
		TIM1->ARR  = 1000; 

		TIM1->CCR1 = 500; 
		TIM1->CCR2 = 500;  
		TIM1->CCR3 = 500;

		TIM1->CCMR1 &=  ~TIM_CCMR1_OC1M_0; 
		TIM1->CCMR1 |=  TIM_CCMR1_OC1M_1;
		TIM1->CCMR1 |=  TIM_CCMR1_OC1M_2; //110: PWM mode 1
  
		TIM1->CCMR1 &=  ~TIM_CCMR1_OC2M_0;
		TIM1->CCMR1 |=  TIM_CCMR1_OC2M_1;
		TIM1->CCMR1 |=  TIM_CCMR1_OC2M_2; //110: PWM mode 1
  
		TIM1->CCMR2 &=  ~TIM_CCMR2_OC3M_0;
		TIM1->CCMR2 |=  TIM_CCMR2_OC3M_1;
		TIM1->CCMR2 |=  TIM_CCMR2_OC3M_2; //110: PWM mode 1
  
		TIM1->CCER  |=  TIM_CCER_CC1E;   // Capture/Compare 1 output enable
		TIM1->CCER  |=  TIM_CCER_CC1NE;  // Capture/Compare 1 complementary output enable
		TIM1->CCER  |=  TIM_CCER_CC2E;   // Capture/Compare 2 output enable
		TIM1->CCER  |=  TIM_CCER_CC2NE;  // Capture/Compare 2 complementary output enable
		TIM1->CCER  |=  TIM_CCER_CC3E;   // Capture/Compare 3 output enable
		TIM1->CCER  |=  TIM_CCER_CC3NE;  // Capture/Compare 3 complementary output enable

		//DTG[7:0]: Dead-time generator setup  1 mks
		TIM1->BDTR  |= TIM_BDTR_DTG_0;
		TIM1->BDTR  |= TIM_BDTR_DTG_1;
		TIM1->BDTR  |= TIM_BDTR_DTG_2;
		TIM1->BDTR  |= TIM_BDTR_DTG_3;
		TIM1->BDTR  |= TIM_BDTR_DTG_4;
		TIM1->BDTR  |= TIM_BDTR_DTG_5;
		TIM1->BDTR  |= TIM_BDTR_DTG_6;
		TIM1->BDTR  |= TIM_BDTR_DTG_7;

//		TIM1->DIER |= TIM_DIER_CC1IE; //Capture/Compare 1 interrupt enable
//		TIM1->DIER |= TIM_DIER_CC2IE; //Capture/Compare 2 interrupt enable
//		TIM1->DIER |= TIM_DIER_CC3IE; //Capture/Compare 3 interrupt enable
  
		TIM1->CR1 |= TIM_CR1_CEN; //Bit 0 CEN: Counter enable
		TIM1->BDTR  |=  TIM_BDTR_MOE;     //MOE: Main output enable
    
	//	NVIC_EnableIRQ (TIM1_CC_IRQn);
		

  
}



