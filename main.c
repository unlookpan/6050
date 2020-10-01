#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "PID.h"
#include "usart.h"
#include "6050.h"
#include "PWM.h"

 extern float y_1;
 extern float y_2;
/************************************************
 6050实验
 代码实验过程：初始化基本函数，当初始化串口2时（开启了6050发送数据后的中断，会一定时间修改pitch，roll）
 当初始化到TIME2后延时一段时间后开启定时器2，随后每一段时间进入中断。而主函数则进入循环打印数据。
 
 重点！！！进入循环后开始计算此刻的目标值（三角函数），并调用自稳程序，自稳程序会调用计算当前xy方向分别
 需要的电机参数（转速）并将参数传递到电机驱动函数，电机驱动函数会根据PID_X的正负自动调整该要那个电机旋转
 
 注意：此PID非常简单，根据我们的调用可以适当增加内循环（现在只有一个循环）。
 
 提示：主要看PWM.C和PID.C及主函数
 
 已测试：
 6050串口发送程序（可以接收到数据）
 PWM输出程序（呼吸灯可以亮）
 电机正反转的IO口高低电频输出程序（高低点平输出）
 TIM2的中断程序（可以进入中断）
 
 测试是用c8测的，IO口都是按照芯片共通IO设的，除了高低电平的测试c8没有，现已改为ZET6
 
 接线：
 在这里只简要说一下6050接线
 6050用的串口2(PA2 TX    PA3 RX)
************************************************/

 int main(void)
 {
	u8 t[2]={0,0};
	/*注意初始化时候的顺序：
	延时函数放在最前面，因为他初始化了系统时钟
	中断优先级配置
	串口配置
	IO口的配置放在定时器前面
	最后是定时器配置
	开启定时器则放在想要的位置*/
	delay_init();	    	 //延时函数初始化	  
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	 
	uart_init(115200);	 //串口初始化为115200
	
	uart2_init(115200);
	LED_Init();
	KEY_Init();          //初始化与按键连接的硬件接口
	
	TIM3_PWM_Init(899,0);
	TIM2_Int_Init(199,7199);//20ms中断一次
	
	delay_ms(1000);
	
	TIM_Cmd(TIM2, ENABLE);  //使能TIMx	
 	while(1)
	{
		//串口发送程序
		printf("pitch  %f\r\n",pitch);
		printf("roll  %f\r\n",roll);
		printf("yaw  %f\r\n",yaw);
		printf("y_1  %f\r\n",y_1);
		printf("y_2  %f\r\n",y_2);
		printf("\r\n");
		delay_ms(50);
		
		//显示到山外上位机的程序
		/*注意：山外上位机的协议可以通过下面的发送程序来查看*/
		t[0]=roll+100;
		t[1]=y_2+100;
		Send_data_to_PC(t, 2);
	}
 }

