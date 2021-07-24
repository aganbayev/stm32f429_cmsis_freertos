/**************************************Library****************************************/

#include "DacLib.h"

/***********************************Use function**************************************/

void DAC_Init(void)
{
		DAC1_Init();
	
}

/***************************************ADC1******************************************/

void DAC1_Init(void)
{
			RCC->APB1ENR |= RCC_APB1ENR_DACEN;
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;    //Enable clock for port A   DAC1 = pa4 and DAC2 = pa5
	
			GPIOA->MODER |= GPIO_MODER_MODE4;    // PA4- pp mode, analog mode
			GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR4;
//			GPIOA->MODER |= GPIO_MODER_MODE5;    // PA5- pp mode, analog mode
//			GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR5;

			DAC->CR |= DAC_CR_EN1;   // ENABLE DAC1
//			DAC->CR |= DAC_CR_EN2;   // ENABLE DAC2
			
			
}

void DacWriteData(uint16_t data)
{
			if(data<4096) DAC->DHR12R1  = data;
}
