#include "Servo.h"
#include "stm32f10x.h"                  // Device header

void Servo_Timer_Init()
{
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM2EN,ENABLE);
	//时基单元初始化
	TIM_TimeBaseInitTypeDef TimeBaseInitStruct;
	TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TimeBaseInitStruct.TIM_Period = 20000 -1;
	TimeBaseInitStruct.TIM_Prescaler = 72-1;
	TimeBaseInitStruct.TIM_ClockDivision =  TIM_CKD_DIV1;
	TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2,&TimeBaseInitStruct);
	
	//输出比较设置
	TIM_OCInitTypeDef TimeOCInitStruct;
	TIM_OCStructInit(&TimeOCInitStruct); 
	
	TimeOCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TimeOCInitStruct.TIM_Pulse =  500;
	TimeOCInitStruct.TIM_OCPolarity =   TIM_OCPolarity_High;
	TimeOCInitStruct.TIM_OutputState = TIM_OutputState_Enable;   //极性输出使能
	TIM_OC2Init(TIM2,&TimeOCInitStruct);
	
	//GPIO TIM_CH1 配置
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	TIM_Cmd(TIM2,ENABLE);
	
}

void Servo_Init(void)
{
	Servo_Timer_Init();
}

void Servo_SetAngle(float Angle)
{
	TIM_SetCompare1(TIM2,(Angle / 180 * 2000 + 500));
}
