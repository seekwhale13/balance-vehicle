#ifndef __CTRL_H
#define __CTRL_H

/***********************定义PID参数************************************/

extern float Angle_Kp;
void PIDinit(void);
int Verti(float Mid, float Real_Ang,short gyro_Y);
int Veloci(int Left_Enco,int Right_Enco);
void PID_Crtl(int Vtical_Out);








#endif