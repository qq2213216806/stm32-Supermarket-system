#include "Delay.h"
#include "stm32f10x.h"                  // Device header

/*   �ӳٺ�����ʼ��
     ʹ��TIM4 ����׼ʱ�� 
	 Ƶ��1M�����¼���ģʽ 
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
    TIM4->CNT = nus - 1; // �������װҪ�ݼ�����������0��ᴥ����ʱ����TIM_FLAG_UpDate��־λ
    TIM4->CR1 |= TIM_CR1_CEN; // ʹ�ܼ������� ��������ʼ�ݼ�
    while((TIM4->SR & TIM_FLAG_Update) != SET); // �ȵ�����������0
    TIM4->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN));//�رռ�����
    TIM4->SR &= ~TIM_FLAG_Update; // �����ʱ����Ϊ0�ı�־λ
}

void delay_ms(unsigned int nms)
{
	while(nms--)
	{ delay_us(1000);}
}
