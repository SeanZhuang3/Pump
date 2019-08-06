#ifndef _TIMER_H//宏定义，定义文件名称
#define _TIMER_H
#include "stm8s.h"//引入STM8的头文件



void Tim6_Init(void);
void Tim6_ReInit(void);

void delay_ms(u16 nCount);
void delay_us(u16 nCount);

#endif //定义文件名称结束


