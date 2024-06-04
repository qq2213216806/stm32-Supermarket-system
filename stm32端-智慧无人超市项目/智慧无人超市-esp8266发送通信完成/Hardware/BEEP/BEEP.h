#ifndef __BEEP_H
#define __BEEP_H

#define BEEP_PORT   GPIOA
#define BEEP_PIN    GPIO_Pin_2
#define BEEP_RCC	RCC_APB2Periph_GPIOA

void BEEP_Init(void);
void BEEP_ON(void);
void BEEP_OFF(void);

#endif /*__BEEP_H*/
