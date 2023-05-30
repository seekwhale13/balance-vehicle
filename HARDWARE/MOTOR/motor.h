#ifndef __MOTOR_H
#define __MOTOR_H
#include "sys.h"

void TIM1_PWM_Init(void);
void TIM1_GPIO_Config (void);
void TIM1_Mode_Config (void);
void Trun_Init(void);
void Forward(void);
void Back(void);
#endif