#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "mpu6050.h"  
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "motor.h"
#include "bsp_systick.h"
#include "timer.h"
#include "bsp_adc.h"
#include "bsp_speed.h"
#include "ctrl.h"
/***********************ADC采集变量**********************************/
extern __IO uint16_t ADC_ConvertedValue;//ADC采集电压寄存器存储值
float Bat_Volt;	//转换后的实际电压值电压值












uint32_t couter = 0; // ms 计时变量 
uint32_t led_cout=0 ;


int main(void)
{ 
  
	ADCx_Init();	//ADC初始化
	uart_init(115200);	//串口初始化为115200
	delay_init();	//延时初始化
	LED_Init();	//LED初始化
				
	Encoder_Init_TIM3();	//编码器定时器初始化
	Encoder_Init_TIM4();	//编码器定时器初始化	
	
  //开机测量一次电池电压		
	Bat_Volt =(float)ADC_ConvertedValue/4096*16.5; 	
	if(Bat_Volt < 11.1)
	{
		printf("电池电压值为:%.3f\n",Bat_Volt);	//测量电池电压
		printf("电池电压过低，请立即充电\n");	//电池电压提醒
	}
  else
	{
		printf("电池电压值为:%.3f\n",Bat_Volt);	//测量电池电压
		printf("电池电压电量良好，请放心使用\n");	//电池电压提醒
	}

	TIM1_PWM_Init();
	Trun_Init();	//电机控制初始化
	
	if(MPU_Init()==0)	
		printf("MPU初始化成功\n");	
	else
		printf("MPU初始化失败\n");
	
	//MPU_DMP初始化
	if(mpu_dmp_init() == 0)	
		printf("MPU_DMP初始化成功\n");	
	else
		printf("MPU_DMP初始化失败\n");	
	delay_ms(20);	
	GENERAL_TIM_Init();	//定时器中断初始化
	
	while(1)
	{
		//	delay_ms(5);	
		if(led_cout == 300)
		{
			LED=~LED;
			led_cout = 0;
		
		}

		
	//printf("陀螺仪角度：%f,%f,%f\r\n",pitch,roll,yaw);
	//printf("左电机速度：%d，右电机速度：%d\n",Left_Enco,Right_Enco);	


	}

	
}

 



//int PID_Param(void)
//{	
//	float De;
//  mpu_dmp_get_data(&pitch,&roll,&yaw);
//	Erro = roll - Med_Aangle;
//	De = Erro - Pre_Erro;
//	Vtical_Out = Vtical_Kp*Erro + De*Vtical_Kd;

//}

