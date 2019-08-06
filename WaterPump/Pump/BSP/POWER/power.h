#ifndef _POWER_H//宏定义，定义文件名称
#define _POWER_H
#include "stm8s.h"//引入STM8的头文件

void Current_Init(void);


u16 Current_Offset(void);
u8 Detect_Current(u16 Offset,u16 *CurCurrent );




#endif //定义文件名称结束


