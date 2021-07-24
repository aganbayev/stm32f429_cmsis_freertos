/**************************************Library****************************************/

#include "TimLib.h"

/***********************************Use function**************************************/

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
