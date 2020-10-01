#include "PID.h"
#include "sys.h"
#include "math.h"
#include "PWM.h"
/**********************************************************************
此程序主要为定义定时器及中断，计算目标值及控制电机的输出
***********************************************************************/
const float priod = 1700;
float t,y_1,y_2;//目标值的时间常数
float time=0;

//PID调节参数
float roll,yaw,pitch; 		//欧拉角
float Gyro_x,Gyro_y,Gyro_z;	//陀螺仪原始数据

//X方向的控制函数参数
float X_error;         //本次位置值与期望误差值
float X_error_last=0;  //上一次误差值
float X_PID;           //最后输出的PID值
float X_P=-100;             //要写上初值
float X_D=2000;             //写上初值

//y方向的控制参数
float Y_error;         //本次位置值与期望误差值
float Y_error_last=0;  //上一次误差值
float Y_PID;           //最后输出的PID值
float Y_P=-100;
float Y_D=2000;

//x方向
float X_P_1=-100;             //要写上初值
float X_D_1=1670;             //写上初值

//y方向
float Y_P_1=-100;
float Y_D_1=1670;

float A;
float s;

void TIM2_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
				 
}
//定时器2中断服务程序
void TIM2_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
		{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx更新中断标志 
			
		//计算目标值
		time+=20;
		if(time==priod)time=0;
		t = 2.0*3.14159*((float)time/priod);
			
		//调用输出函数
			Auto_PID();
			//circle(25);
			//len(90,25);
		}
}

//x方向计算函数(自稳)
void Control_XPID(float expect_x){
		X_error=expect_x-pitch;         //pitch为测出的值
		X_PID=X_P*X_error+X_D*(X_error-X_error_last);       //PID计算公式
	  X_error_last = X_error;
	}

//x方向
	void Control_XPID_1(float expect_x){
		X_error=expect_x-pitch;         //pitch为测出的值
		X_PID=X_P_1*X_error+X_D_1*(X_error-X_error_last);       //PID计算公式
	  X_error_last = X_error;
//		if(X_PID>600)X_PID=600;
//		if(X_PID<-600)X_PID=-600;
	}
	
//y方向计算函数（自稳）
void Control_YPID(float expect_y){
		Y_error=expect_y-roll;         //pitch为测出的值
		Y_PID=Y_P*Y_error+Y_D*(Y_error-Y_error_last);       //PID计算公式
	  Y_error_last = Y_error;
	}

//y方向
	void Control_YPID_1(float expect_y){
		Y_error=expect_y-roll;         //pitch为测出的值
		Y_PID=Y_P_1*Y_error+Y_D_1*(Y_error-Y_error_last);       //PID计算公式
	  Y_error_last = Y_error;
//		if(Y_PID>600)Y_PID=600;
//		if(Y_PID<-600)Y_PID=-600;
	}
	
//自稳函数	
void Auto_PID(void)	
{
	Control_XPID(0);
	Control_YPID(0);
	x_control(X_PID);
	y_control(Y_PID);
}

//直线
void len(float B,float s)
{
	y_1=s*cos(B/57.6)*sin(t);
	y_2=s*sin(B/57.6)*sin(t);
	y_1=atan(y_1/76)*57.6;
	y_2=atan(y_2/76)*57.6;
	Control_XPID_1(y_1); 
	Control_YPID_1(y_2);
	x_control(X_PID);
	y_control(Y_PID);
}

//画圆
void circle(float K)
{
	A=atan(K/76)*57.6;
	if(K==20)y_1=1.05*A*sin(t),y_2=A*cos(t);
	if(K==15)y_1=1.05*A*sin(t),y_2=A*cos(t);
	if(K==25)y_1=1.03*A*sin(t),y_2=0.98*A*cos(t);
	y_2=A*cos(t);
	Control_XPID_1(y_1);
	Control_YPID_1(y_2);
	x_control(X_PID);
	y_control(Y_PID);
}	
