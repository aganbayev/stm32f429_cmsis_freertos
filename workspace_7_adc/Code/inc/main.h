/**************************************Library****************************************/

#include "stm32f429xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "string.h"
#include "stdio.h"

#include "sysinit.h"
#include "TimLib.h"
#include "UartLib.h"
#include "AdcLib.h"


/**************************************Defines****************************************/

//	QueueHandle_t xQueue1;
	xQueueHandle SendDataAdc;

/**************************************Use Tasks****************************************/

void vTaskADC (void *argument);
void vTaskTFT (void *argument);
