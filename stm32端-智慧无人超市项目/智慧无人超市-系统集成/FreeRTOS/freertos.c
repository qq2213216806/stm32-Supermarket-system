/* USER CODE BEGIN Header */
#include "LED.h"
#include "OLED.h"
#include "Serial.h"
#include "BEEP.h"
#include "ESP8266.h"
#include "Delay.h"
#include "rc522.h"
#include "string.h"
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */


/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
 void rc522_Task(void *params)
 {
	 char cStr [ 30 ];
	 char cStr2[30];
	uint8_t ucArray_ID [ 4 ];    /*œ»∫Û¥Ê∑≈ICø®µƒ¿‡–Õ∫ÕUID(ICø®–Ú¡–∫≈)*/                                                                                         
	uint8_t ucStatusReturn;      /*∑µªÿ◊¥Ã¨*/
	while(1)
	{	
		/*ÂºÄÂßãÂØªÂç°*/
		if ( ( ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID ) ) != MI_OK )
		{
		  /*ÂØªÂç°ÊàêÂäü*/
		  printf ( "ÂØªÂç°Â§±Ë¥•\n" );
		 // ESP8266_Printf("ÂØªÂç°Â§±Ë¥•\n",15);
		  ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID ); //PICC_REQALL   PICC_REQIDL
		}
		if ( ucStatusReturn == MI_OK  )
		{
			printf ( "ÂØªÂç°ÊàêÂäü\n" );
			//ESP8266_Printf("ÂØªÂç°ÊàêÂäü\n",15);
		  /*∑¿≥Â◊≤£®µ±”–∂‡’≈ø®Ω¯»Î∂¡–¥∆˜≤Ÿ◊˜∑∂Œß ±£¨∑¿≥ÂÕªª˙÷∆ª·¥”∆‰÷–—°‘Ò“ª’≈Ω¯––≤Ÿ◊˜£©*/
			if ( PcdAnticoll ( ucArray_ID ) == MI_OK )                                                                   
			{
				sprintf ( cStr, "The Card ID is: %02X%02X%02X%02X",ucArray_ID [0], ucArray_ID [1], ucArray_ID [2],ucArray_ID [3] );
				printf ( "%s\r\n",cStr );  //¥Ú”°ø®∆¨ID
				sprintf(cStr2,"Âç°Âè∑:%02X%02X%02X%02X",ucArray_ID [0], ucArray_ID [1], ucArray_ID [2],ucArray_ID [3]);
				ESP8266_SendPrintf(cStr2,strlen(cStr2));
				//ESP8266_SendData(ucArray_ID,sizeof(ucArray_ID));
				BEEP_ON();
				vTaskDelay(100);
				BEEP_OFF();
			}		
		}
		vTaskDelay(1000);
	}
 }
 
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);
  xTaskCreate(rc522_Task,"rc522_task",256,NULL,osPriorityNormal,NULL);
   
  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */

/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
	 LED_Turn();
	//ESP8266_SendPrintf("hello",5);
	vTaskDelay(1000);
	 // Delay_ms(1000);
	  
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
/* USER CODE END Application */

