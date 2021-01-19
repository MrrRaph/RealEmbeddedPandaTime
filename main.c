/**
  ******************************************************************************
  * @file    main.c
  * @author  P. Foubet
  * @version V1.0
  * @date    31-December-2018
  * @brief   Default main function.
  ******************************************************************************
*/

#include "FreeRTOS.h"
#include "task.h"
#include "interface.h"
#include "semphr.h"

SemaphoreHandle_t sem = NULL;

void msleep(unsigned int ms)
{
	vTaskDelay(ms/portTICK_RATE_MS);
}

void task(void * args)
{
	while(1)
	{
		if (sem != NULL)
		{
			if (xSemaphoreTake(sem, (TickType_t) 10) == pdTRUE)
			{
				unsigned int ms = (unsigned int) args;
				for (int i = 0; i < 3; ++i)
				{
					BSP_LED_On(LED3);
					BSP_LED_On(LED4);
					msleep(ms);
					BSP_LED_Off(LED3);
					BSP_LED_Off(LED4);
					msleep(ms);
				}
				xSemaphoreGive(sem);
			}
			else
			{
				//TKT
			}
		}
		msleep(300);
	}
}

int main(void)
{
	char * taskName[]=
	{
			"Task #1",
			"Task #2",
			"Task #3"
	};
	interface_init();

	sem = xSemaphoreCreateMutex();

	/* creation des threads */
	if (!(pdPASS == xTaskCreate( task, taskName[0], 64, (void *) 300, configMAX_PRIORITIES-1, NULL ))) goto err;
	if (!(pdPASS == xTaskCreate( task, taskName[1], 64, (void *) 150,  configMAX_PRIORITIES-1, NULL ))) goto err;
	if (!(pdPASS == xTaskCreate( task, taskName[2], 64, (void *) 75,  configMAX_PRIORITIES-1, NULL ))) goto err;

	/* lancement du systeme */
	vTaskStartScheduler();
	err:              // en principe jamais atteint !
	  while(1);
  	  return 0;
}
