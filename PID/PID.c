#include "PID.h"
#include "sys.h"
#include "math.h"
#include "PWM.h"
/**********************************************************************
�˳�����ҪΪ���嶨ʱ�����жϣ�����Ŀ��ֵ�����Ƶ�������
***********************************************************************/
const float priod = 1700;
float t,y_1,y_2;//Ŀ��ֵ��ʱ�䳣��
float time=0;

//PID���ڲ���
float roll,yaw,pitch; 		//ŷ����
float Gyro_x,Gyro_y,Gyro_z;	//������ԭʼ����

//X����Ŀ��ƺ�������
float X_error;         //����λ��ֵ���������ֵ
float X_error_last=0;  //��һ�����ֵ
float X_PID;           //��������PIDֵ
float X_P=-100;             //Ҫд�ϳ�ֵ
float X_D=2000;             //д�ϳ�ֵ

//y����Ŀ��Ʋ���
float Y_error;         //����λ��ֵ���������ֵ
float Y_error_last=0;  //��һ�����ֵ
float Y_PID;           //��������PIDֵ
float Y_P=-100;
float Y_D=2000;

//x����
float X_P_1=-100;             //Ҫд�ϳ�ֵ
float X_D_1=1670;             //д�ϳ�ֵ

//y����
float Y_P_1=-100;
float Y_D_1=1670;

float A;
float s;

void TIM2_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
				 
}
//��ʱ��2�жϷ������
void TIM2_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
		{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
			
		//����Ŀ��ֵ
		time+=20;
		if(time==priod)time=0;
		t = 2.0*3.14159*((float)time/priod);
			
		//�����������
			Auto_PID();
			//circle(25);
			//len(90,25);
		}
}

//x������㺯��(����)
void Control_XPID(float expect_x){
		X_error=expect_x-pitch;         //pitchΪ�����ֵ
		X_PID=X_P*X_error+X_D*(X_error-X_error_last);       //PID���㹫ʽ
	  X_error_last = X_error;
	}

//x����
	void Control_XPID_1(float expect_x){
		X_error=expect_x-pitch;         //pitchΪ�����ֵ
		X_PID=X_P_1*X_error+X_D_1*(X_error-X_error_last);       //PID���㹫ʽ
	  X_error_last = X_error;
//		if(X_PID>600)X_PID=600;
//		if(X_PID<-600)X_PID=-600;
	}
	
//y������㺯�������ȣ�
void Control_YPID(float expect_y){
		Y_error=expect_y-roll;         //pitchΪ�����ֵ
		Y_PID=Y_P*Y_error+Y_D*(Y_error-Y_error_last);       //PID���㹫ʽ
	  Y_error_last = Y_error;
	}

//y����
	void Control_YPID_1(float expect_y){
		Y_error=expect_y-roll;         //pitchΪ�����ֵ
		Y_PID=Y_P_1*Y_error+Y_D_1*(Y_error-Y_error_last);       //PID���㹫ʽ
	  Y_error_last = Y_error;
//		if(Y_PID>600)Y_PID=600;
//		if(Y_PID<-600)Y_PID=-600;
	}
	
//���Ⱥ���	
void Auto_PID(void)	
{
	Control_XPID(0);
	Control_YPID(0);
	x_control(X_PID);
	y_control(Y_PID);
}

//ֱ��
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

//��Բ
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
