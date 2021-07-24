/**************************************Library****************************************/

#include "AdcLib.h"

/***********************************Use function**************************************/

void ADC_Init(void)
{
		//ADC1_Init();
		ADC3_Init();
	
}

/***************************************ADC1******************************************/

void ADC1_Init(void)
{
			RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;    //Enable clock for port A
			ADC->CCR |= ADC_CCR_ADCPRE_0;     // Divide clock apb2 to 4 for ADC; maximal frequency must be 12 MHz
	
			GPIOA->MODER |= GPIO_MODER_MODE0;    // PA0- pp mode, analog mode for channel 1
			GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR0;
	
			ADC1->CR2 |= ADC_CR2_ADON;  // ADC ENABLE
			ADC1->CR2 &= ~ADC_CR2_CONT;  // single mode conversion
			ADC1->CR2 &= ~ADC_CR2_EXTEN;  //disconnection external trigger
			ADC1->SMPR2 |= ADC_SMPR2_SMP1;  // Sampling maximal cycles , low msps = 480 cycles
			ADC1->SQR3 &= ~ADC_SQR3_SQ1;   //channel and gpio pin selection    ch 1 and pin pa0   - 00000
			
}


void ADC3_Init(void)
{
			RCC->APB2ENR |= RCC_APB2ENR_ADC3EN;
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;   //Enable clock for port F
			ADC->CCR |= ADC_CCR_ADCPRE_0;     // Divide clock apb2 to 4 for ADC; maximal frequency must be 12 MHz
	
			GPIOF->MODER |= GPIO_MODER_MODE6;    // PF6- pp mode, analog mode for channel 4
			GPIOF->PUPDR &= ~GPIO_PUPDR_PUPDR6;
			//GPIOF->OTYPER &= ~GPIO_OTYPER_OT6;
			//GPIOF->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR6;
			

			//ADC3->CR2 |= ADC_CR2_CAL;    //Start calibration
			//while(!(ADC1->CR2 & ADC_CR2_CAL)); // Waiting to end callibration     // in STM32f4 not fount callibration future
	
			ADC3->CR2 |= ADC_CR2_ADON;  // ADC ENABLE
			ADC3->CR2 &= ~ADC_CR2_CONT;  // single mode conversion
			ADC3->CR2 &= ~ADC_CR2_EXTEN;  //disconnection external trigger
			//ADC3->CR2 |= ADC_CR2_EXTSEL +1U; // event start conversion SWSTART
			//ADC3->CR2 |= ADC_CR2_EXTEN;  //ENABLE START CONVERSION EXTERNAL SIGNAL 
			//ADC3->CR2 &= ~ADC_CR2_ALIGN;
			ADC3->SMPR2 |= ADC_SMPR2_SMP4;  // Sampling maximal cycles , low msps = 480 cycles
			ADC3->SQR3 |= ADC_SQR3_SQ1_2 ;   //channel and gpio pin selection    ch 4 and pin pf6   - 00110
			//ADC3->CR1 |= ADC_CR1_AWDCH_2;
}

uint16_t Adc3SendData(void)
{
	uint16_t data;
	
	ADC3->CR2 |= ADC_CR2_SWSTART;
	while(!(ADC3->SR & ADC_SR_EOC));
	data = ADC3->DR;
	
	return data;
}

