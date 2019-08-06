#ifndef _CONFER_H
#define _CONFER_H

#include "stm8s.h"

#define BIT0 0x01
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80

#define MAX_SIGNAL_PERIOD       40      //最大信号周期
#define MIN_SIGNAL_PERIOD       1       //最小
#define START_SIGNAL_PERIOD     13      //开始
#define THRESHOLD_SIGNAL_PERIOD  28       //临界
typedef struct
{
    volatile u8 StartFlag;            //转1  停0
    volatile u8 Step;              
    volatile u8 ErrorFlag;        //有故障1  无0
    volatile u8 BemfCount;        //反电动势中过零次数
    volatile u8 BemfSwitchStepCount;        //反电动势中换相次数
    
    
    u8  StartSwitchStepCount;	//启动函数里换相的计数
    u8  StOK;                    //正常启动1  未正常启动0
    u16 PreDuty;                //预定位占空比
    u16 StartDuty;              //启动占空比
    u16 PwmValue;              //实时PWM
    u16 Start_Delay;
    u8  NumPair;

}MotorParam_TypeDef;


typedef enum
{
    START = 0,
    OPEN_LOOP,
    CLOSE_LOOP,
    STOP,
    COMMUNICATION,

}WORK_STATE;










#endif 


