#include "stm32f10x.h"                  // Device header
void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_initstruct;
	GPIO_initstruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_initstruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_initstruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_initstruct);
	GPIO_SetBits(GPIOA,GPIO_Pin_7);
}
void LED_Turn(void)
{
	if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_7)==1)
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);
	}
	else 
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_7);
	}
}
