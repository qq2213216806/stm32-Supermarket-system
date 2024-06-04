#include "stm32f10x.h"                  // Device header
#include "cmsis_os.h"

#include "LED.h"
#include "OLED.h"
#include "Delay.h"
#include "Serial.h"
#include "BEEP.h"
#include "Servo.h"
#include "ESP8266.h"
#include "rc522.h"
void DRIVE_Init(void);
void MX_FREERTOS_Init(void);

int main(void)
{
	/*hardware drive Init*/
	 DRIVE_Init();
	
	/* 设置系统中断优先级分组 4 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	/* Init scheduler */
	osKernelInitialize();  /* Call init function for freertos objects (in freertos.c) */
	MX_FREERTOS_Init();

	/* Start scheduler */
	osKernelStart();
	/* We should never get here as control is now taken by the scheduler */
	/* Infinite loop */
	
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		/* USER CODE END WHILE */
		/* USER CODE BEGIN 3 */
	}             
	/* USER CODE END 3 */
}


void DRIVE_Init(void)
{
	LED_Init();
	//OLED_Init();
	delay_Init();
	Serial_Init();
	BEEP_Init();
	Servo_Init();
	ESP8266_Init();
	RC522_Init();
}
