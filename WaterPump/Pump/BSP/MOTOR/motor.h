#ifndef _MOTOR_H//宏定义，定义文件名称
#define _MOTOR_H
#include "stm8s.h"//引入STM8的头文件
#include "confer.h"


static void Motor_IO_Configuration(void);
static void Motor_Configuration(void);

void Motor_Stop(void);
u8 Commutation(u8 Step,u16 OutPwmValue);

void Motor_Init(void);
u8 Sensorless_START(void);
u8 Det_Step(MotorParam_TypeDef MotorParam);


#endif //定义文件名称结束

