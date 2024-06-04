#include "BEEP.h"
#include "stm32f10x.h"                  // Device header

void BEEP_Init(void)
{
	RCC_APB1PeriphClockCmd(BEEP_RCC,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = BEEP_PIN; 
	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz;
	GPIO_Init(BEEP_PORT,&GPIO_InitStructure);
	
	GPIO_SetBits(BEEP_PORT,BEEP_PIN);
	//GPIO_ResetBits(BEEP_PORT,BEEP_PIN);
}

void BEEP_ON(void)
{
	GPIO_ResetBits(BEEP_PORT,BEEP_PIN);
}

void BEEP_OFF(void)
{
	GPIO_SetBits(BEEP_PORT,BEEP_PIN);
}

