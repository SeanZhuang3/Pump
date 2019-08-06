#ifndef _LED_H//宏定义，定义文件名称
#define _LED_H

#include "stm8s.h"//引入STM8的头文件

#define LED_ON     GPIO_WriteHigh(GPIOD, GPIO_PIN_7)
#define LED_OFF    GPIO_WriteLow(GPIOD, GPIO_PIN_7)


void Led_Int(void);





#endif //定义文件名称结束


