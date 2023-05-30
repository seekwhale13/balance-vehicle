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
/***********************编码器采集变量**********************************/
int Left_Enco;	//左电机速度
int Right_Enco;	//右电机速度

/***********************MPU6050采集变量**********************************/
float pitch,roll,yaw;	//欧拉角
float Med_Angle = -0.2;//机械中值角，小车平衡时的角度
short gx1,gy1,gz1;
int Vertical_out, Veloci_out,Last_pwm;



//定时器2中断服务程序

void GENERAL_TIM_IRQHandler (void)
{

	if ( TIM_GetITStatus( GENERAL_TIM, TIM_IT_Update) != RESET ) 
	{	
		couter++;
//		led_cout++;
		if(couter == 2)
		{
			led_cout++;
			mpu_dmp_get_data(&pitch,&roll,&yaw);	//获得陀螺仪角度
			MPU_Get_Gyroscope(&gx1, &gy1, &gz1);
			Left_Enco=-Read_Encoder(3);	//获得电机速度
			Right_Enco=	Read_Encoder(4);
			//直立环输出
			Vertical_out = Verti(Med_Angle, roll,gx1);
			//速度环环输出
		  Veloci_out = Veloci(Left_Enco, Right_Enco);
			//叠加控制信号
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
//ADC中断服务程序
void ADC_IRQHandler(void)
{	
	if (ADC_GetITStatus(ADCx,ADC_IT_EOC)==SET) 
	{
		// 读取ADC的转换值
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
