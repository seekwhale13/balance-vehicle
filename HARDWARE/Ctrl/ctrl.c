#include "ctrl.h"
#include "motor.h"


/*******************************************************************/
float Pre_Erro; 	//上次角度偏差
int Enc_Err_last_Lowout=0,Encoder_S=0;	//上次偏差保存
//直立环PID参数
float Angle_Kp = 50.0;
float Angle_Kd = -0.1;
//速度环PID参数
float Velocity_Kp = -0.4;
float	Velocity_Ki = -0.0002;








/****************************************************
函数功能：直立环
原理：Kp*EK+Kd*EK_dt
输入：期望角度、真实角度
输出：直立环PWM
*****************************************************/

int Verti(float Mid, float Real_Ang,short gyro_Y)
{
float Ang_Erro;	//角度偏差，前一次偏差,偏差变化大小
int Vertical_out;
Ang_Erro = Mid - Real_Ang;	//偏差=期望角度-真实角度
//gyro_Y = Ang_Erro-Pre_Erro;//偏差减少幅度
//Pre_Erro = Ang_Erro;	//保留此刻偏差至上一次偏差，为下一次计算作准备
Vertical_out = Angle_Kp*(Mid - Real_Ang) + Angle_Kd*(gyro_Y-0);//Kp*EK+Kd*EK_dt
return Vertical_out;
}


/****************************************************
函数功能：速度环
原理：Kp*VEK+Ki*VEK_sum
输入：电机编码器速度,
输出：直立环PWM
Left_Enco=-Read_Encoder(3);	//获得电机速度
			Right_Enco=	Read_Encoder(4);	
*****************************************************/
int Veloci(int Left_Enco,int Right_Enco)
{
	int Encoder, Enc_Err_Lowout,PWM_out;
	//1.计算速度误差
	Encoder =(Left_Enco + Right_Enco)-0;	
	//2.对速度偏差进行偏差滤波
	Enc_Err_Lowout = (1-0.7)*Encoder + 0.7*Encoder;	
	Enc_Err_last_Lowout = Enc_Err_Lowout;
	//3.对速度偏差积分
	Encoder_S += Enc_Err_last_Lowout;
	//4.积分限幅
	Encoder_S = Encoder_S>7200?7200:(Encoder_S<-7200?-7200:Encoder_S);
	
	PWM_out = Velocity_Kp*Enc_Err_Lowout + Velocity_Ki*Encoder_S;
	return PWM_out;
}








void PID_Crtl(int Vertical_out)
{
		if(Vertical_out > 720)	//限幅输出
		Vertical_out = 720;
	if(Vertical_out < -720)	//限幅输出
		Vertical_out = -720;
	
	if(Vertical_out < 0)	//判断方向
	{
		Vertical_out = -Vertical_out;

		Back();
	}
	else
	  Forward();
	
	

	TIM1->CCR1 = Vertical_out;  //对TIM外设初始化
	TIM1->CCR4 = Vertical_out;  //对TIM外设初始化

}

