#include "stm32f429xx.h"


void delay(uint32_t time);

int main(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN; 
	GPIOG->MODER |= GPIO_MODER_MODE13_0;
	GPIOG->OTYPER |= 0x00000000;
	GPIOG->OSPEEDR |= GPIO_OSPEEDR_OSPEED13_0;
	GPIOG-> PUPDR &= GPIO_PUPDR_PUPDR13;
	
	GPIOG->MODER |= GPIO_MODER_MODE14_0;
	GPIOG->OSPEEDR |= GPIO_OSPEEDR_OSPEED14_0;
	GPIOG-> PUPDR &= GPIO_PUPDR_PUPDR14;
	
	

	while(1)
	{
	GPIOG-> ODR |= GPIO_ODR_OD13_Msk;
	GPIOG-> ODR &= ~GPIO_ODR_OD14;
		delay(1000000);
	GPIOG-> ODR |= GPIO_ODR_OD14_Msk;
	GPIOG-> ODR &= ~GPIO_ODR_OD13;
		delay(1000000);
	}
}

void delay(uint32_t time)
{
		uint32_t i;
	for(i=0;i<time;i++){
	}
}
