#ifndef __BSP_SPEED_H
#define	__BSP_SPEED_H


#include "sys.h" 
#include "stm32f10x.h"

#define ENCODER_TIM_PERIOD (u16)(65535)   //103�Ķ�ʱ����16λ 2��16�η������65536


void Encoder_Init_TIM4(void);
void Encoder_Init_TIM3(void);
int Read_Encoder(u8 TIMX);
void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);


#endif