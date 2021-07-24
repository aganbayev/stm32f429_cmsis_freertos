/**************************************Library****************************************/

#include "sysinit.h"

/***********************************Use function**************************************/

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

void GPIO_Init (void)      // LEDS AND BUTTON
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

/***********************************FLASH MEMORY**************************************/

void FlashUnlock(void)
{
		FLASH->KEYR = MEMORY_KEY1;
		FLASH->KEYR = MEMORY_KEY2;
		FLASH->OPTKEYR = MEMORY_OPTKEY1;
		FLASH->OPTKEYR = MEMORY_OPTKEY2;
	
}

void FlashLock(void)
{
		FLASH->CR |= FLASH_CR_LOCK;
}

void FlashAllErase(void)
{
		FLASH->CR |= FLASH_CR_MER | FLASH_CR_MER1; //mass erase block 1 and block 2
		FLASH->CR |= FLASH_CR_STRT;
	
}

void FlashSectorErase(void)
{
		//FLASH->OPTCR1 |= FLASH_OPTCR1_nWRP_11;
		FLASH->CR |= FLASH_CR_SER; //Sector erase activate
		FLASH->CR |= FLASH_CR_SNB_0 | FLASH_CR_SNB_1 | FLASH_CR_SNB_3 | FLASH_CR_SNB_4;
		FLASH->CR |= FLASH_CR_STRT;
	
}

uint32_t FlashReadData(uint32_t address)
{
		//FLASH->OPTCR1 |= FLASH_OPTCR1_nWRP_11; // after reset allow write-read
		return (*(__IO uint32_t*) address);
}

void FlashWriteData(uint32_t address, uint32_t data)
{
		//FLASH->OPTCR1 |= FLASH_OPTCR1_nWRP_11;
		FLASH->CR |= FLASH_CR_PG;
		FLASH->CR &= ~FLASH_CR_PSIZE;   //type of data word, byte ...
		FLASH->CR |= FLASH_CR_PSIZE_0;  //choose half word - 16 bit format
		while((FLASH->SR & FLASH_SR_BSY) != 0 );
		*(__IO uint16_t*) address = (uint16_t) data;
		while((FLASH->SR & FLASH_SR_BSY) != 0 );
		address += 2;
		data >>= 16;
		*(__IO uint16_t*) address = (uint16_t) data;
		while((FLASH->SR & FLASH_SR_BSY) != 0 );

/*
  FLASH->CR &= ~FLASH_CR_PSIZE;
  FLASH->CR |= FLASH_CR_PSIZE_1;
  FLASH->CR |= FLASH_CR_PG;

  *(__IO uint32_t*)address = data;	
		FLASH->CR &= ~(FLASH_CR_PG);
*/
}




