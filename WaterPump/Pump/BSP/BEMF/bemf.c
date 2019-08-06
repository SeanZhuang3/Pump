#include "bemf.h"   //引入自定义公共头文件
#include "motor.h"
#include "confer.h"


//反电动势引脚数据
#define FC_PORT GPIOB->IDR  
#define FC_PIN  BIT5

#define FB_PORT GPIOB->IDR
#define FB_PIN  BIT6

#define FA_PORT GPIOB->IDR
#define FA_PIN  BIT7

#define STCount 5    //检测10次过零信号


extern MotorParam_TypeDef Motor;
u8 HallSteps[8]={7,5,3,4,1,0,2,7};
u8 CheckBEMF[6]={0x04,0x06,0x02,0x03,0x01,0x05};


void Bemf_Init(void)
{
  Bemf_Configuration();
  
  //使能外部中断
  GPIOB->CR2|=BIT5|BIT6|BIT7;

  EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOB, EXTI_SENSITIVITY_RISE_FALL);
  
}


/*初始化无感比较端口*/
void Bemf_Configuration(void)
{  
    //比较器引脚
    GPIO_Init(GPIOB, GPIO_PIN_5,GPIO_MODE_IN_PU_NO_IT);//C
    GPIO_Init(GPIOB, GPIO_PIN_6,GPIO_MODE_IN_PU_NO_IT);//B
    GPIO_Init(GPIOB, GPIO_PIN_7,GPIO_MODE_IN_PU_NO_IT);//A

    	
}

/*换相任务*/
void Task_Bemf(void)
{
    u8 State =0;
    u16 i = 0;
    if((Motor.StartFlag==0) || (Motor.ErrorFlag!=0))//电机没有启动命令或处于故障状态，不工作
      return;                                   
    
    /* 滤波 判断*/
    State = GPIOB->IDR;  //读状态
    for(i=0;i<=550;i++);                           //延时滤波
    
    i = GPIOB->IDR;
    
    if(State!=i) return;
    
    State = 0;	
            
    //A相过零点检测
    if (FA_PORT & FA_PIN)
    {
        State |= BIT2;          //PB7        
    }
    
    //B相过零点检测
    if (FB_PORT & FB_PIN)
    {
        State |= BIT1;          //PB6
    }
                    
    //C相过零点检测
    if (FC_PORT & FC_PIN)
    {
        State |= BIT0;          //PB5
    }

     if(Motor.StOK == 0)//启动过程
     { 
        if(State==CheckBEMF[Motor.Step])	//正确检测到对应相序的过零点
        {
          Motor.BemfCount++;
          GPIO_WriteReverse(GPIOD,GPIO_PIN_7);
        }//指示灯反转指示  
        else 
        {
            Motor.BemfCount = 0;
        }
         
        if(Motor.BemfCount >= STCount)//连续检测过固定数量的过零点时，切换至过零换相
        {
            Motor.StOK=1;//启动成功标志
        }
     }

    if(Motor.StOK == 1)
    {
        Motor.Step = HallSteps[State];//得到换相步序

        if (Motor.Step == 7)//运行过程中，状态读错，为错误，停止PWM输出
        { //故障
                Motor.ErrorFlag=4;
                return;
        }
        
        Motor.StartSwitchStepCount++;//正确换相次数加1
        Commutation(Motor.Step,Motor.PwmValue);//输出PWM信号，启动电机
    }		
}




