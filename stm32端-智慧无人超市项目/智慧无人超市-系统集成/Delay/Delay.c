#include "Delay.h"
#include "stm32f10x.h"                  // Device header

/*   延迟函数初始化
     使用TIM4 做基准时钟 
	 频率1M，向下计数模式 
*/

void delay_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;
	TIM_TimeBaseInitStructure.TIM_Period = 1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Down;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
}

void delay_us(unsigned int nus)
{
    TIM4->CNT = nus - 1; // 向计数器装要递减的数，减到0后会触发定时器的TIM_FLAG_UpDate标志位
    TIM4->CR1 |= TIM_CR1_CEN; // 使能计数器， 计数器开始递减
    while((TIM4->SR & TIM_FLAG_Update) != SET); // 等到计数器减到0
    TIM4->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN));//关闭计数器
    TIM4->SR &= ~TIM_FLAG_Update; // 清除定时器变为0的标志位
}

void delay_ms(unsigned int nms)
{
	while(nms--)
	{ delay_us(1000);}
}
