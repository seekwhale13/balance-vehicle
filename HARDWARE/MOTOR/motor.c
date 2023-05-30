#include "motor.h"
 
//ARR���Զ���װ�ؼĴ���
//PSC��Ԥ��Ƶ��
//CCR������/�ȽϼĴ���

u16 ARR = (720-1);//�Զ���װֵ
u16 PSC = 0;//ʱ��Ԥ��Ƶϵ��
u16 CCR = 3600;//��װ�벶��ȽϼĴ���������ֵ
//u16 CCR = 50;//��װ�벶��ȽϼĴ���������ֵ

/*------����GPIO------*/
void TIM1_GPIO_Config (void)
{
GPIO_InitTypeDef GPIO_InitStructure; 	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);// ʹ��TIM1��ʱ�ӣ����ڲ�ʱ��FCK_INT=72M
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��GPIOA����ʱ��

//���ø�����Ϊ�����������,���TIM1_CH1��OC1����TIM1_CH4��OC4����PWM���岨��
//����Ƚ�ͨ��GPIO��ʼ��,PA8��PA11	
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11; //TIM1_CH1��ӦGOIOΪPA8
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //���������������Ϊʹ����PWM��PWM��Ϊ�˿ڸ��ã�����Ҫ���ó�AF
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ����������
}


/*------���ø߼���ʱ���Ĺ���ģʽ------*/
void TIM1_Mode_Config (void)
{	
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//����һ��TimeBaseInitTypeDef���͵ı���
TIM_OCInitTypeDef  TIM_OCInitStructure;
TIM_OCInitTypeDef  TIM_OC4InitStructure;
	
//1*--------------------ʱ���ṹ���ʼ��--------------------*/
/*ʱ����ʼ��,����TIM1��ʱ�����Ƶ�ʣ��Լ�������ز�����ʼ��*/
TIM_TimeBaseStructure.TIM_Period = ARR;//�Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»��ж� 	
TIM_TimeBaseStructure.TIM_Prescaler = PSC; //����������CNT��ʱ�ӣ�����ʱ�ӵ���FCK_INT/(PSC+1),��72M/8 = 9M
TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//����ʱ�ӷ�Ƶϵ����0������Ƶ��TIM_CKD_DIV1:����ʱ�ӷ�Ƶϵ����1��Ƶ	
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//����������ģʽ�����ϼ���ģʽ
TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //��ʼ����ʱ��
//---------------------------------------------------------------------------------------------//


	
//2*--------------------����ȽϽṹ���ʼ��--------------------*/
/*����PWM�������ʽ,PWMģʽ���ã�ͨ��1��OC1*/
TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //���ģʽ���ã���Ҫ��PWM1��PWM2�����ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�������ģʽ״̬��ʹ�ܻ�رա�����Ƚ����ʹ��
TIM_OCInitStructure.TIM_Pulse = CCR; //���ô�װ�벶��ȽϼĴ���������ֵ 
TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //������Ч��ƽ���ԣ���PWMģʽ����Ч��ƽ����Ϊ�ߵ�ƽ��͵�ƽ
TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx

//4ͨ��
TIM_OC4InitStructure.TIM_OCMode = TIM_OCMode_PWM2; //���ģʽ���ã���Ҫ��PWM1��PWM2�����ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
TIM_OC4InitStructure.TIM_OutputState = TIM_OutputState_Enable; //�������ģʽ״̬��ʹ�ܻ�رա�����Ƚ����ʹ��
TIM_OC4InitStructure.TIM_Pulse = CCR; //���ô�װ�벶��ȽϼĴ���������ֵ 
TIM_OC4InitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //������Ч��ƽ���ԣ���PWMģʽ����Ч��ƽ����Ϊ�ߵ�ƽ��͵�ƽ
TIM_OC4Init(TIM1, &TIM_OC4InitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	




//����ʹ�ܼĴ���TIM_CCMR1��OC1PEλ����һλ�ǿ���TIM1_CCR1�Ĵ�����Ԥװ�ع���
//TIMx_CCRx�Ĵ����ܹ����κ�ʱ��ͨ��������и����Կ��Ʋ��Σ����ͨ�����д����Ʋ��ε�ֵ��������Ч�ػ��ڶ�ʱ��������һ�θ����¼�ʱ�����µ�
//Enable������һ�θ����¼�ʱ�����£�Disable��������Ч
TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);		
TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);		
//---------------------------------------------------------------------------------------------//	



//3*--------------------ʹ��--------------------*/ 			
//������ֹ�ڶ�ʱ������ʱ��ARR�Ļ�������д����ֵ���Ա��ڸ����¼�����ʱ���븲����ǰ��ֵ��
//�ڿ�ʼ��ʼ����ʱ�����Ѿ���" TIM_TimeBaseStructure.TIM_Period=PSC;"������Ҳһֱ�����ֵ��
//����û�б�д�жϷ��������������жϷ������и�����û�и�ARR����������д����ֵ,
//����TIM_ARRPreloadConfig()����ΪDISABLE ��ENABLE��û��Ӱ�졣
TIM_ARRPreloadConfig(TIM1, ENABLE); //ʹ��TIM1��ARR�ϵ�Ԥװ�ؼĴ���
		
TIM_Cmd(TIM1, ENABLE);  //ʹ��TIM1
TIM_CtrlPWMOutputs(TIM1,ENABLE);	//MOE �����ʹ��	

}

 
void TIM1_PWM_Init(void)
{  
TIM1_GPIO_Config();//����ΪTIM����ͨ�����õ�GPIO���ų�ʼ��
TIM1_Mode_Config ();//��TIM�����ʼ��
}

void Trun_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PC�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				     //LED0-->PC.13 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					       //�����趨������ʼ��GPIOB.5

}

void Forward(void)

{
	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_15);
	GPIO_ResetBits(GPIOB,GPIO_Pin_12|GPIO_Pin_14);

}

void Back(void)

{
	GPIO_ResetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_15);
	GPIO_SetBits(GPIOB,GPIO_Pin_12|GPIO_Pin_14);
}

