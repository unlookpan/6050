#include "led.h"

//////////////////////////////////////////////////////////////////////////////////	 
//�ó���Ϊ���IO�ڿ�������ת�����þ��忴����
//PE5ΪLED���ԵƵĳ�����ʵ�����
//PD9 PD10 PD11 PD12 PF12 PF13 PF14 PF15
////////////////////////////////////////////////////////////////////////////////// 	   

//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOF, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	
	//led���Գ���
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //LED1-->PE.5 �˿�����, �������
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOE, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
	
 GPIO_ResetBits(GPIOE,GPIO_Pin_5);						 //PE5����ͣ��ɸ��������
 
 //������ſ���IO�ڸ�ֵ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOD,GPIO_Pin_10);
	GPIO_ResetBits(GPIOD,GPIO_Pin_9);
	
	GPIO_SetBits(GPIOD,GPIO_Pin_12);
	GPIO_ResetBits(GPIOD,GPIO_Pin_11);
	
	GPIO_SetBits(GPIOF,GPIO_Pin_15);
	GPIO_ResetBits(GPIOF,GPIO_Pin_14);
	
	GPIO_SetBits(GPIOF,GPIO_Pin_13);
	GPIO_ResetBits(GPIOF,GPIO_Pin_12);
}
 
