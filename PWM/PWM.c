#include "PWM.h"
#include "usart.h"

//////////////////////////////////////////////////////////////////////////////////	
//�˳���ΪPWM���IO�ڵ�����
//TIME3 PB0 PB1 PA6 PA7
//////////////////////////////////////////////////////////////////////////////////
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��   
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	
	//PWM�˿ڳ�ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO
	
	///////////////////
 
   //��ʼ��TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	
	
	//��ʼ��TIM3 Channel2 PWMģʽ	 (ͨ����)///////////////////////////////////////////////////////////
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//�������:TIM����Ƚϼ��Ե�
	
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	
	
	
	//ʹ��ͨ����ʱ��////////////////////////////////////////////////////////////////////////////////////
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
 
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3
	
	//PWM���Գ������Բ��Թ��������Ŀ����ٴβ���
}

void x_control(float x)
{
	if(x>0)
	{
		TIM_SetCompare3(TIM3,0);
		TIM_SetCompare1(TIM3,x);
	}
	if(x<0)
	{
		TIM_SetCompare3(TIM3,-x);
		TIM_SetCompare1(TIM3,0);
	}
}

void y_control(float y)
{
	if(y>0)
	{
		TIM_SetCompare2(TIM3,0);
		TIM_SetCompare4(TIM3,y);
	}
	if(y<0)
	{
		TIM_SetCompare2(TIM3,-y);
		TIM_SetCompare4(TIM3,0);
	}
}
