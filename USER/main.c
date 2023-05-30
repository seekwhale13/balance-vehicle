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
/***********************ADC�ɼ�����**********************************/
extern __IO uint16_t ADC_ConvertedValue;//ADC�ɼ���ѹ�Ĵ����洢ֵ
float Bat_Volt;	//ת�����ʵ�ʵ�ѹֵ��ѹֵ












uint32_t couter = 0; // ms ��ʱ���� 
uint32_t led_cout=0 ;


int main(void)
{ 
  
	ADCx_Init();	//ADC��ʼ��
	uart_init(115200);	//���ڳ�ʼ��Ϊ115200
	delay_init();	//��ʱ��ʼ��
	LED_Init();	//LED��ʼ��
				
	Encoder_Init_TIM3();	//��������ʱ����ʼ��
	Encoder_Init_TIM4();	//��������ʱ����ʼ��	
	
  //��������һ�ε�ص�ѹ		
	Bat_Volt =(float)ADC_ConvertedValue/4096*16.5; 	
	if(Bat_Volt < 11.1)
	{
		printf("��ص�ѹֵΪ:%.3f\n",Bat_Volt);	//������ص�ѹ
		printf("��ص�ѹ���ͣ����������\n");	//��ص�ѹ����
	}
  else
	{
		printf("��ص�ѹֵΪ:%.3f\n",Bat_Volt);	//������ص�ѹ
		printf("��ص�ѹ�������ã������ʹ��\n");	//��ص�ѹ����
	}

	TIM1_PWM_Init();
	Trun_Init();	//������Ƴ�ʼ��
	
	if(MPU_Init()==0)	
		printf("MPU��ʼ���ɹ�\n");	
	else
		printf("MPU��ʼ��ʧ��\n");
	
	//MPU_DMP��ʼ��
	if(mpu_dmp_init() == 0)	
		printf("MPU_DMP��ʼ���ɹ�\n");	
	else
		printf("MPU_DMP��ʼ��ʧ��\n");	
	delay_ms(20);	
	GENERAL_TIM_Init();	//��ʱ���жϳ�ʼ��
	
	while(1)
	{
		//	delay_ms(5);	
		if(led_cout == 300)
		{
			LED=~LED;
			led_cout = 0;
		
		}

		
	//printf("�����ǽǶȣ�%f,%f,%f\r\n",pitch,roll,yaw);
	//printf("�����ٶȣ�%d���ҵ���ٶȣ�%d\n",Left_Enco,Right_Enco);	


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

