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
#include "DacLib.h"
#include "DmaLib.h"


/**************************************Defines****************************************/


/**************************************Use Tasks****************************************/

void vTaskDMA (void *argument);
void vTaskDMA2 (void *argument);
