/****************************************Library*****************************************/

#include "main.h"


/*****************************************Main*******************************************/

int main(void)
{
	RCC_Init();
	GPIO_Init();
	UART_Init();
	PWM_Init();
	ADC_Init();
	DAC_Init();

	xTaskCreate(vTaskADC, "ADC", 128, NULL, 1, NULL );
	xTaskCreate(vTaskTFT, "TFT", 128, NULL, 1, NULL );
	
	SendDataAdc = xQueueCreate( 5, sizeof( uint16_t ) );
	
	FlashUnlock();
	FlashWriteData(0x081E0000, 0x444451FF);
	FlashLock();
	
  vTaskStartScheduler();
	
	while(1)
	{

	}
}

/****************************************Tasks******************************************/


void vTaskADC (void *argument)
{
	uint16_t adcResult;
	uint16_t adcResult1;
	while(1)
	{
		adcResult1 = Adc1SendData();  //read from adc1 and send dac, after read adc3 from dac and send uart 
		DacWriteData(adcResult1);   //write dac
		adcResult = Adc3SendData();
		xQueueSend(SendDataAdc, &adcResult, 0);
		GPIOG->ODR ^= GPIO_ODR_OD13;
		//FlashUnlock();
		//FlashAllErase();
		vTaskDelay(100);
	}  
}

void vTaskTFT (void *argument)
{
	uint16_t adcResult;
	char adcResultStr[20];
	char adcVoltageStr[20];
	float step = 0.00122;
	float AdcVoltage; 
	while(1)
	{
		if(uxQueueMessagesWaiting(SendDataAdc) != 0)   // Check queue
		{
		xQueueReceive(SendDataAdc, &adcResult, 0);
		
		AdcVoltage = (float)adcResult * step; 
/*************************************ADC result***************************************/
		sprintf(adcResultStr, "Adc value is %u,",adcResult);
		USART1_Send_String(adcResultStr);
		sprintf(adcVoltageStr, " Voltage is %.3f\r\n",AdcVoltage);
		USART1_Send_String(adcVoltageStr);

		}
		vTaskDelay(100);
	}
}
/**************************************Handlers****************************************/

void USART1_IRQHandler(void)
{
		if((USART1->SR & USART_SR_RXNE) && (USART1->CR1 & USART_CR1_RXNEIE))
		{
			USART1->SR &= ~USART_SR_RXNE;
			
		}
}
