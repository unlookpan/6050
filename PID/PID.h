#ifndef __PID_H
#define __PID_H
#include "sys.h"
void TIM2_Int_Init(u16 arr,u16 psc);
void TIM2_IRQHandler(void);
void Control_XPID(float expect_x);
void Control_YPID(float expect_y);
void Control_XPID_1(float expect_x);
void Control_YPID_1(float expect_y);
void Auto_PID(void);
void len(float A,float s);
void circle(float K);	
#endif
