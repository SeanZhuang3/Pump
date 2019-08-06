#ifndef _CAPTURE_H//宏定义，定义文件名称
#define _CAPTURE_H
#include "stm8s.h"//引入STM8的头文件
#include "confer.h"


void Capture_Init(void);
void Task_Capture1(void);
void Nwp_Signal(void);
void Nwp_Signal_H(void);
void Det_Signal_Period_Reset(u8 *DetPeriod,u8 DetCapCount);
u8 Cal_Speed(u8 SigPeriod,u16 *Speed);
#endif //定义文件名称结束


