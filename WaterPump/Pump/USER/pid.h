#ifndef _PID_H//宏定义，定义文件名称
#define _PID_H
#include "stm8s.h"//引入STM8的头文件


typedef struct
{
  u16 P;                                //比例参数
  u16 I;                                //积分	                      
//  u16 D;                                //微分

//  u16 Out_Min;
//  u16 Out_Max;                        
          
//  float Up;
//  double Ui;
//  float Ud;

  int Err;                                //误差
//  double LastErr;                         //上一次误差        
  double SumErr;                          //误差总和
  u16 Out;	                           
}PID_TypeDef;



void PID_Init(void);
void PID_DeInit(void);
u16 PID_Compute(u16 Target,u16 Real,PID_TypeDef* PID);
//u16 PID_Compute(unsigned int Target,unsigned int Real);
 


#endif //定义文件名称结束


