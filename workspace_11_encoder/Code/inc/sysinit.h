/**************************************Library****************************************/

#include "stm32f429xx.h"

/*************************************DEFINES*****************************************/

#define MEMORY_KEY1 ((uint32_t) 0x45670123)
#define MEMORY_KEY2 ((uint32_t) 0xCDEF89AB)
#define MEMORY_OPTKEY1 ((uint32_t) 0x08192A3B)
#define MEMORY_OPTKEY2 ((uint32_t) 0x4C5D6E7F)


/***********************************Use function**************************************/

void RCC_Init (void);
void GPIO_Init (void);

void FlashUnlock(void);
void FlashLock(void);
void FlashAllErase(void);
void FlashSectorErase(void);
uint32_t FlashReadData(uint32_t address);
void FlashWriteData(uint32_t address, uint32_t data);


