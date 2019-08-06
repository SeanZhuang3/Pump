#include "pid.h"

#include "confer.h"

PID_TypeDef PID;


/*PID 初始化*/
void PID_Init(void)
{
  PID.P = 60;                                //比例参数
  PID.I = 35;                                 //积分
//  PID.D = 0;                                 //微分
  
//  PID.Out_Max =1000  ;        // duty = 100%         //限幅
//  PID.Out_Min = 1 ;          //duty = 0.1%
  
//  PID.Up = 0;
//  PID.Ui = 0;
//  PID.Ud = 0;
  PID.Err = 0;  
//  PID.LastErr = 0;
  PID.SumErr = 0;
  PID.Out = 0 ;
}

/*PID 复位*/
void PID_DeInit(void)
{
  PID.P = 0;                                //比例参数
  PID.I = 0;                                 //积分

  PID.Err = 0;  
  PID.SumErr = 0;
  PID.Out = 0 ;
}



/*PID计算*/
u16 PID_Compute(u16 Target,u16 Real,PID_TypeDef* PID)
{  
    double i = 0;
  
//    PID->LastErr = PID->Err;   //上次误差
    
    PID->Err=Target - Real;        //本次误差
    
    PID->SumErr +=PID->Err;
    
    i = PID->P * PID->Err;                         //参数为零 不运算
    i = i + PID->I * PID->SumErr;
//    i = i + PID->D * (PID->Err - PID->LastErr);
    
//    PID->Up = PID->P * PID->Err;                         //参数为零 不运算
//    
//    PID->Ui= PID->I * PID->SumErr;  
//    PID->Ud = PID->D * (PID->Err - PID->LastErr);  
//    if(PID->Ui >  PID->I_max) PID->Ui =  PID->I_max;	             //积分限幅
//    if(PID->Ui <  PID->I_max) PID->Ui = -PID->I_max;	             //积分限幅
    
        
    PID->Out = i / 1000;
    
      if(PID->Out  >  1000)             // duty = 100%         //限幅
      { 
        PID->Out = 1000;		
      } 
      else if(PID->Out  <  1)
      { 
        PID->Out = 1;				
      } 
    
    
    return PID->Out;
}

    




