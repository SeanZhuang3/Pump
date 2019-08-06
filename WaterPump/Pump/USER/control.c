#include "control.h"   
#include "confer.h"


extern MotorParam_TypeDef Motor;
extern PID_TypeDef PID;
u16 Target = 2000;              //目标转速
u16 Real = 0;                   //真实转速        



u16 CurrentInit ;               //电流偏移量
u16 RealCurrent;                //实际电流

extern u8 HighPeriod ;          //高电平时长
extern u8 CapCount;             //捕获信号次数

u8 Tx_Data[5]={0xBB,0x13,0,0,0xEE};
             


WORK_STATE Work_State = STOP;

/*硬件初始化*/
void BSP_Init(void)
{    
    Watch_Dog_Init();
    Clock_Init();       //时钟
   
    Motor_Init();       
    UART_Init();
    Bemf_Init();
    Tim6_Init();
    
    Capture_Init();
    
    Current_Init();
    CurrentInit = Current_Offset();


    enableInterrupts();//中断使能
} 



  
void Run(void)
{

  while (1)
  {
      switch(Work_State)
      {
          case (START):         //开环启动
                 if(Motor.StartFlag == 0)         //未启动
                {
                    Motor.StartFlag = 1;
              
                    if(Sensorless_START() != 0)//正常启动
                    {

                      Work_State = CLOSE_LOOP;
                    }
                    else                      //无法启动
                    {
                      
                      Motor.ErrorFlag = 1;
                      Work_State = STOP;
                    }                
                }           
                 
                          break;	
                          
          case (CLOSE_LOOP):	//闭环转
                if(Motor.ErrorFlag == 0 && Motor.StartFlag == 1)
                {
                  
                  PID_Init();
                  Motor.PwmValue = PID_Compute(Target , Real ,&PID);
                  Work_State = COMMUNICATION;
                  
                }
                else                      //无法启动
                {                 
                  Motor.ErrorFlag = 2;
                  Work_State = STOP;
                }                
   
                          break;
                
          case (STOP):          //停止
                if(Motor.ErrorFlag != 0)//有故障
                {
                  Motor_Stop();
                  
                  Motor.StartFlag = 0;
//                  Motor.ErrorFlag = 0;          //清故障                 
                  Motor.StOK = 0;
                  Motor.Step = 5;
                  Real = 0;
                

                  PID_DeInit();
                  Nwp_Signal_H();       //NWP 置高
                  
                }
                else
                {

                  Work_State = START;
                }
                
                          
                          break;
          case (COMMUNICATION):	//通信
            
            
                  /*车信号通信*/              
                   Motor.ErrorFlag = Det_Step(Motor);            //检测换相的步数
                   if(Motor.ErrorFlag != 0)
                   {
                      Work_State = STOP;                     
                   }

//                  if( Cal_Speed(HighPeriod ,&Target) == 1)//转
//
//                  {
//                    if((Motor.StartFlag == 0) && (Motor.ErrorFlag == 0) &&(Motor.StOK == 0))    //未启动
//                    {
//                      Work_State = START;
//                    }
//                    else                        //变速
//                    {
//                        //会跳转50ms任务
//                    }
//                  }
//                  else                          //停止
//                  {
//                    Motor.ErrorFlag = 3;
//                    Work_State = STOP;
//                  }

                       
                          break;                          
          default:break;
      }
  }
} 
 
u8  Count6ms=0,Count50ms=0;
u16 Count100ms=0;  //软计时变量定义

void Time_1ms(void)//1ms定时器 
{   
    Count6ms++;

    Count50ms++;
    
    Count100ms++;
    
    Task_50ms();
    Task();
//    Task_100ms();
    TIM6_ClearITPendingBit(TIM6_IT_UPDATE);//清除更新标志位
   
}


/*定时任务*/
void Task(void)
{
  if((Count6ms % 6 )== 0 ) //6ms任务
  {

    if(Motor.StartFlag == 1)
    {
      Nwp_Signal();
    }

  } 
  
  if(Count6ms  >= 30)           //30ms任务
  {
    Feed_Dog();                                                         //喂狗
    
    Motor.ErrorFlag = Detect_Current(CurrentInit,&RealCurrent);         //电流检测
    if(Motor.ErrorFlag != 0)
    {
      Work_State = STOP;
      if((Motor.ErrorFlag != 0) &&(Motor.ErrorFlag == 5))  //过流，强制停机
      {
        
        Tx_Data[1]=0x14;Tx_Data[2]=0;Tx_Data[3] = Motor.ErrorFlag;;	
        Uart_Send();
        
        while(1)
        {
          Motor_Stop();
        }
      }
    }

    Count6ms = 0;
  }
}



/*50ms任务*/   
void Task_50ms(void)
{
  if(Count50ms >= 50)
  {
//    Det_Signal_Period_Reset(&HighPeriod ,CapCount);  //SWP信号复位
    
    Count50ms = 0;
  //RealS=NumCoun*20*60/6/MoPair;	//NumCoun  换相次数          MoPair  极数    
    Real= Motor.StartSwitchStepCount * 200/Motor.NumPair;//StartSwitchStepCount  换相次数  MoPair  极数
    
    Motor.StartSwitchStepCount=0;
  
    if(Motor.StartFlag==1)                      //运行状态
    Motor.PwmValue = PID_Compute(Target , Real ,&PID);
  }
}

void Task_100ms(void)
{
  if(Count100ms == 200)         //电流
  {
      Tx_Data[1]=0x12;Tx_Data[2]=RealCurrent/256;Tx_Data[3]=RealCurrent%256;	
      Uart_Send(); 

  }
  else if(Count100ms == 400)    //转速
  {
      Tx_Data[1]=0x13;Tx_Data[2]=Real/256;Tx_Data[3]=Real%256;	
      Uart_Send();
    
  }
  else if(Count100ms == 600)    //故障码
  {
      Tx_Data[1]=0x14;Tx_Data[2]=0;Tx_Data[3] = Motor.ErrorFlag;;	
      Uart_Send();
    
      Count100ms = 0;
  }
  

 
}


  

/*发送串口数据*/
void Uart_Send(void)
{
  u8 i;

  for(i=0;i<5;i++)
  {
    while(UART1_GetFlagStatus(UART1_FLAG_TC)==RESET);
    UART1->DR = Tx_Data[i];

  }

}




