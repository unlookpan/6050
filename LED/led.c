#include "led.h"

//////////////////////////////////////////////////////////////////////////////////	 
//该程序为电机IO口控制正反转的设置具体看下面
//PE5为LED测试灯的程序可适当调用
//PD9 PD10 PD11 PD12 PF12 PF13 PF14 PF15
////////////////////////////////////////////////////////////////////////////////// 	   

//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOF, ENABLE);	 //使能PB,PE端口时钟
	
	//led测试程序
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //LED1-->PE.5 端口配置, 推挽输出
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOE, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
	
 GPIO_ResetBits(GPIOE,GPIO_Pin_5);						 //PE5输出低，可根据情况改
 
 //电机引脚控制IO口赋值
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
 
