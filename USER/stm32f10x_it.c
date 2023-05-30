/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h" 
#include "bsp_systick.h"
#include "led.h"
#include "usart.h"
#include "timer.h"
#include "bsp_adc.h"
#include "mpu6050.h"  
#include "inv_mpu.h"
#include "bsp_speed.h"
#include "ctrl.h"
extern __IO uint16_t ADC_ConvertedValue;
extern volatile uint32_t couter;
extern volatile uint32_t led_cout ;
/***********************�������ɼ�����**********************************/
int Left_Enco;	//�����ٶ�
int Right_Enco;	//�ҵ���ٶ�

/***********************MPU6050�ɼ�����**********************************/
float pitch,roll,yaw;	//ŷ����
float Med_Angle = -0.2;//��е��ֵ�ǣ�С��ƽ��ʱ�ĽǶ�
short gx1,gy1,gz1;
int Vertical_out, Veloci_out,Last_pwm;



//��ʱ��2�жϷ������

void GENERAL_TIM_IRQHandler (void)
{

	if ( TIM_GetITStatus( GENERAL_TIM, TIM_IT_Update) != RESET ) 
	{	
		couter++;
//		led_cout++;
		if(couter == 2)
		{
			led_cout++;
			mpu_dmp_get_data(&pitch,&roll,&yaw);	//��������ǽǶ�
			MPU_Get_Gyroscope(&gx1, &gy1, &gz1);
			Left_Enco=-Read_Encoder(3);	//��õ���ٶ�
			Right_Enco=	Read_Encoder(4);
			//ֱ�������
			Vertical_out = Verti(Med_Angle, roll,gx1);
			//�ٶȻ������
		  Veloci_out = Veloci(Left_Enco, Right_Enco);
			//���ӿ����ź�
			Last_pwm = Vertical_out + Angle_Kp*Veloci_out;
			PID_Crtl(Last_pwm);
		  couter = 0;
		}
//		if(led_cout == 1000)
//		{
//			LED=~LED;
//			led_cout = 0;
//		}

		TIM_ClearITPendingBit(GENERAL_TIM , TIM_FLAG_Update);  		 
	}		 	
}





void NMI_Handler(void)
{
}
 
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}
 
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

 
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}
 
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}
 
void SVC_Handler(void)
{
}
 
void DebugMon_Handler(void)
{
}
 
void PendSV_Handler(void)
{
}
 


void SysTick_Handler(void)
{
	
}
//ADC�жϷ������
void ADC_IRQHandler(void)
{	
	if (ADC_GetITStatus(ADCx,ADC_IT_EOC)==SET) 
	{
		// ��ȡADC��ת��ֵ
		ADC_ConvertedValue = ADC_GetConversionValue(ADCx);
	}
	ADC_ClearITPendingBit(ADCx,ADC_IT_EOC);
}







/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
