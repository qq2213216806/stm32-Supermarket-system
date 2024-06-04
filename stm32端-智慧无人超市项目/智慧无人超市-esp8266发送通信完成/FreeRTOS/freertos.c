/* USER CODE BEGIN Header */
#include "LED.h"
#include "OLED.h"
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
#include "Serial.h"
#include "BEEP.h"
#include "ESP8266.h"

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
struct TaskPrintInfo 
{
	uint8_t x;
	uint8_t y;
	char name[16];
};
static struct TaskPrintInfo task1Info = {1,1,"task1"};
static struct TaskPrintInfo task2Info = {2,1,"task2"};
static struct TaskPrintInfo task3Info = {3,1,"task3"};
static int g_Lcduse = 1;

 void LcdPrintTask(void *params)
 {
	 struct TaskPrintInfo *pInfo = params;
	 uint32_t cnt =0;
	 
	 while(1)
	 {
		 if(g_Lcduse)
		{
			g_Lcduse = 0;
			OLED_ShowString(pInfo->x,pInfo->y,pInfo->name);
			OLED_ShowNum(pInfo->x,pInfo->y + 6,cnt++,2);
			g_Lcduse = 1; 
		}
		vTaskDelay(500);
	 }
 }
 
 void BEEP_Task(void *params)
 {
	 
	 while(1)
	 { 
		 BEEP_ON();
	 vTaskDelay(500);
	 BEEP_OFF();
	 vTaskDelay(500);
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
  // xTaskCreate(LcdPrintTask,"task1",128,&task1Info,osPriorityNormal,NULL);
   //xTaskCreate(LcdPrintTask,"task2",128,&task2Info,osPriorityNormal,NULL);
   //xTaskCreate(LcdPrintTask,"task3",128,&task3Info,osPriorityNormal,NULL);
   //xTaskCreate(BEEP_Task,"beep_task",128,NULL,osPriorityNormal,NULL);
   
   
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
 int cnt = 0;	
  /* Infinite loop */
  for(;;)
  {
	  LED_Turn();
	  ESP8266_Send("hello",5);
	  vTaskDelay(1000);
	  
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
/* USER CODE END Application */

