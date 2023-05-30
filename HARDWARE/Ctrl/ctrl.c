#include "ctrl.h"
#include "motor.h"


/*******************************************************************/
float Pre_Erro; 	//�ϴνǶ�ƫ��
int Enc_Err_last_Lowout=0,Encoder_S=0;	//�ϴ�ƫ���
//ֱ����PID����
float Angle_Kp = 50.0;
float Angle_Kd = -0.1;
//�ٶȻ�PID����
float Velocity_Kp = -0.4;
float	Velocity_Ki = -0.0002;








/****************************************************
�������ܣ�ֱ����
ԭ��Kp*EK+Kd*EK_dt
���룺�����Ƕȡ���ʵ�Ƕ�
�����ֱ����PWM
*****************************************************/

int Verti(float Mid, float Real_Ang,short gyro_Y)
{
float Ang_Erro;	//�Ƕ�ƫ�ǰһ��ƫ��,ƫ��仯��С
int Vertical_out;
Ang_Erro = Mid - Real_Ang;	//ƫ��=�����Ƕ�-��ʵ�Ƕ�
//gyro_Y = Ang_Erro-Pre_Erro;//ƫ����ٷ���
//Pre_Erro = Ang_Erro;	//�����˿�ƫ������һ��ƫ�Ϊ��һ�μ�����׼��
Vertical_out = Angle_Kp*(Mid - Real_Ang) + Angle_Kd*(gyro_Y-0);//Kp*EK+Kd*EK_dt
return Vertical_out;
}


/****************************************************
�������ܣ��ٶȻ�
ԭ��Kp*VEK+Ki*VEK_sum
���룺����������ٶ�,
�����ֱ����PWM
Left_Enco=-Read_Encoder(3);	//��õ���ٶ�
			Right_Enco=	Read_Encoder(4);	
*****************************************************/
int Veloci(int Left_Enco,int Right_Enco)
{
	int Encoder, Enc_Err_Lowout,PWM_out;
	//1.�����ٶ����
	Encoder =(Left_Enco + Right_Enco)-0;	
	//2.���ٶ�ƫ�����ƫ���˲�
	Enc_Err_Lowout = (1-0.7)*Encoder + 0.7*Encoder;	
	Enc_Err_last_Lowout = Enc_Err_Lowout;
	//3.���ٶ�ƫ�����
	Encoder_S += Enc_Err_last_Lowout;
	//4.�����޷�
	Encoder_S = Encoder_S>7200?7200:(Encoder_S<-7200?-7200:Encoder_S);
	
	PWM_out = Velocity_Kp*Enc_Err_Lowout + Velocity_Ki*Encoder_S;
	return PWM_out;
}








void PID_Crtl(int Vertical_out)
{
		if(Vertical_out > 720)	//�޷����
		Vertical_out = 720;
	if(Vertical_out < -720)	//�޷����
		Vertical_out = -720;
	
	if(Vertical_out < 0)	//�жϷ���
	{
		Vertical_out = -Vertical_out;

		Back();
	}
	else
	  Forward();
	
	

	TIM1->CCR1 = Vertical_out;  //��TIM�����ʼ��
	TIM1->CCR4 = Vertical_out;  //��TIM�����ʼ��

}

