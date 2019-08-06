//#include "power.h" /************************ 自定义公共头文件 **************************************/

#ifndef _CONTROL_H//宏定义，定义文件名称
#define _CONTROL_H

#include "stm8s.h"//引入STM8的头文件
#include "clock.h" 

#include "timer.h"  
#include "bemf.h"
#include "motor.h"
#include "pid.h"
#include "capture.h"
#include "power.h" 
#include "usart.h" 


void BSP_Init(void);
void Run(void);


void Time_1ms(void);

void Task(void);
void Task_50ms(void);
void Task_100ms(void);



void Uart_Send(void);










#endif //定义文件名称结束


