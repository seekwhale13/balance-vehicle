#ifndef __BSP_SPEED_H
#define	__BSP_SPEED_H


#include "sys.h" 
#include "stm32f10x.h"

#define ENCODER_TIM_PERIOD (u16)(65535)   //103的定时器是16位 2的16次方最大是65536


void Encoder_Init_TIM4(void);
void Encoder_Init_TIM3(void);
int Read_Encoder(u8 TIMX);
void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);


#endif