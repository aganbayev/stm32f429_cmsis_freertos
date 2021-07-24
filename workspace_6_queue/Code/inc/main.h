/**************************************Library****************************************/

#include "stm32f429xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


#include "sysinit.h"
#include "TimLib.h"
#include "UartLib.h"


/**************************************Defines****************************************/

//	QueueHandle_t xQueue1;
	xQueueHandle SendData;

/**************************************Use Tasks****************************************/

void vTaskButton1 (void *argument);
void vTaskLed1 (void *argument);
void vTaskTest1 (void *argument);
void vTaskTest2 (void *argument);
