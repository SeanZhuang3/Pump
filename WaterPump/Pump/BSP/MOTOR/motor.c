#include "motor.h"   

#include "timer.h" 




//上桥臂开关控制端口定义
#define AH_PORT 	GPIOC
#define AH_PIN	        GPIO_PIN_3
#define BH_PORT 	GPIOC
#define BH_PIN	        GPIO_PIN_7
#define CH_PORT 	GPIOC
#define CH_PIN	        GPIO_PIN_6

//下桥臂开关控制端口定义
#define AL_PORT         GPIOC
#define AL_PIN	        GPIO_PIN_2 
#define BL_PORT         GPIOC
#define BL_PIN	        GPIO_PIN_1 
#define CL_PORT         GPIOE
#define CL_PIN	        GPIO_PIN_5 

//下桥臂低电平开关管导通

#define PWM_AL_OFF GPIO_WriteHigh(AL_PORT, AL_PIN) 
#define PWM_BL_OFF GPIO_WriteHigh(BL_PORT, BL_PIN) 
#define PWM_CL_OFF GPIO_WriteHigh(CL_PORT, CL_PIN)

#define PWM_AL_ON GPIO_WriteLow(AL_PORT, AL_PIN) 
#define PWM_BL_ON GPIO_WriteLow(BL_PORT, BL_PIN)
#define PWM_CL_ON GPIO_WriteLow(CL_PORT, CL_PIN)

#define STM8_FREQ_MHZ 16000000
#define PWM_FREQUENCY 16000    //  16K
#define ARRVAL STM8_FREQ_MHZ / PWM_FREQUENCY

MotorParam_TypeDef Motor;



//换相电路开关管IO初始化
static void Motor_IO_Configuration(void)
{	

  
  //PB012 下桥臂0有效 ,配置为高电平
  GPIO_Init(AL_PORT, AL_PIN,GPIO_MODE_OUT_PP_HIGH_FAST);
  GPIO_Init(BL_PORT, BL_PIN,GPIO_MODE_OUT_PP_HIGH_FAST);
  GPIO_Init(CL_PORT, CL_PIN,GPIO_MODE_OUT_PP_HIGH_FAST);
  
//    GPIOB->ODR = 0;
//    GPIOB->IDR = 0x1C;
//    GPIOB->DDR = 0;
//    GPIOB->CR1 = 0xE0;
//    GPIOB->CR2 = 0xE0;
  
  //PC123 上桥臂1有效,配置为低电平
  GPIO_Init(AH_PORT, AH_PIN,GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(BH_PORT, BH_PIN,GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(CH_PORT, CH_PIN,GPIO_MODE_OUT_PP_LOW_FAST);
  
//    GPIOC->ODR = 0x06;
//    GPIOC->IDR = 0x16;
//    GPIOC->DDR = 0xCE;
//    GPIOC->CR1 = 0xCE;
//    GPIOC->CR2 = 0xCE;
}

static void Motor_Configuration(void)
{
//  TIM1_DeInit();
//  
//  TIM1_TimeBaseInit(0, TIM1_COUNTERMODE_UP, ARRVAL, 0);
//
//  TIM1_OC1Init(TIM1_OCMODE_PWM1, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_DISABLE, ARRVAL*0, TIM1_OCPOLARITY_HIGH, TIM1_OCNPOLARITY_LOW, TIM1_OCIDLESTATE_RESET, TIM1_OCNIDLESTATE_SET); 
//
//  TIM1_OC2Init(TIM1_OCMODE_PWM1, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_DISABLE, ARRVAL*0, TIM1_OCPOLARITY_HIGH, TIM1_OCNPOLARITY_LOW, TIM1_OCIDLESTATE_RESET, TIM1_OCNIDLESTATE_SET); 
//
//  TIM1_OC3Init(TIM1_OCMODE_PWM1, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_DISABLE, ARRVAL*0, TIM1_OCPOLARITY_HIGH, TIM1_OCNPOLARITY_LOW, TIM1_OCIDLESTATE_RESET, TIM1_OCNIDLESTATE_SET); 
//
//  TIM1_CCPreloadControl(DISABLE);      //预装载
//  TIM1_Cmd(ENABLE);
    TIM1->CR1 = 0x01;
    TIM1->SR1 = 0x1f;
    TIM1->CCMR1 = 0x60;
    TIM1->CCMR2 = 0x60;
    TIM1->CCMR3 = 0x60;
    TIM1->CCER1 = 0X99;
    TIM1->CCER2 = 0X09;
    TIM1->ARRH = 0X03;
    TIM1->ARRL = 0XE8;
    TIM1->OISR = 0X2A;
    
}





//换向输出PWM值，
//Step:当前换相步序0-5，OutPwmValue 输出PWM值
/*换相函数*/
u8 Commutation(u8 Step,u16 OutPwmValue)
{

    TIM1->BKR &= (u8)(~TIM1_BKR_MOE);//禁止PWM输出
       
    PWM_AL_OFF;PWM_BL_OFF;PWM_CL_OFF; /*全关*/

    //根据换相步序，打开不同的开关管，并施加正确的PWM信号
    if(Step==0)//AB
    {

        TIM1->CCR3H = (uint8_t)(OutPwmValue >> 8);
        TIM1->CCR3L = (uint8_t)(OutPwmValue);   
        
        TIM1->CCER1 =0x00;
        TIM1->CCER2 =0x01;

        PWM_BL_ON;
    }
    else if(Step==1)	//AC
    {
        TIM1->CCR3H = (uint8_t)(OutPwmValue >> 8);
        TIM1->CCR3L = (uint8_t)(OutPwmValue);  
        
        TIM1->CCER1 =0x00;
        TIM1->CCER2 =0x01;

        PWM_CL_ON;
    }
    else if(Step==2)	//BC
    {
        TIM1->CCR2H = (uint8_t)(OutPwmValue >> 8);
        TIM1->CCR2L = (uint8_t)(OutPwmValue); 
        
        TIM1->CCER1 =0x10;
        TIM1->CCER2 =0x00;
        
        PWM_CL_ON;
    }
    else if(Step==3)	//BA
    {
        TIM1->CCR2H = (uint8_t)(OutPwmValue >> 8);
        TIM1->CCR2L = (uint8_t)(OutPwmValue);  
        
        TIM1->CCER1 =0x10;
        TIM1->CCER2 =0x00; 

        PWM_AL_ON;
    }
    else if(Step==4)    //CA
    {
        TIM1->CCR1H = (uint8_t)(OutPwmValue >> 8);
        TIM1->CCR1L = (uint8_t)(OutPwmValue);        
        TIM1->CCER1 =0x01;
        TIM1->CCER2 =0x00;

        PWM_AL_ON;
    }
    else if(Step==5)	//CB
    {
        
        TIM1->CCR1H = (uint8_t)(OutPwmValue >> 8);
        TIM1->CCR1L = (uint8_t)(OutPwmValue);
        TIM1->CCER1 =0x01;
        TIM1->CCER2 =0x00;

        PWM_BL_ON;
    }

    TIM1->BKR|=TIM1_BKR_MOE;//使能PWM输出
    return Step;
}

/*电机停*/
void Motor_Stop(void)
{
//  Start_F=0;//启动标志设为停止
  Motor.StOK = 0;
  TIM1->BKR &= (uint8_t)(~TIM1_BKR_MOE);//禁止PWM输出
  PWM_AL_OFF;//下管全关
  PWM_BL_OFF;
  PWM_CL_OFF;
}
/*电机参数初始化*/
void Motor_Init(void)
{
  Motor.StartFlag= 0;
  Motor.Step = 5;
  Motor.ErrorFlag = 0;
  Motor.BemfCount = 0;          //反电动势中过零次数
  Motor.BemfSwitchStepCount = 0;//反电动势中正确换相次数
  Motor.StartSwitchStepCount = 0;
  Motor.StOK = 0;
  Motor.PreDuty = 25;           //预定位25%占空比
  Motor.StartDuty = 40;           //启动占空比
  Motor.PwmValue = 0;        //实时占空比
  Motor.Start_Delay = 5;            //起动过程，延时时间ms为单位
  Motor.NumPair = 4;            //极对数
  
  Motor_IO_Configuration();
  Motor_Configuration();
  Motor_Stop();
}


  

/*无感启动*/
u8 Sensorless_START(void)
{
    u16 StartCount = 0;      //启动函数中的换相次数
    u16 PwmValue = 0;

    
    PwmValue =(ARRVAL * Motor.PreDuty)/100; 
    Motor.Step = Commutation(Motor.Step,PwmValue);//按预定位PWM设定预定位
    
    delay_us(10);//较小时间			

    PwmValue =(ARRVAL * Motor.StartDuty)/100; 
    do
    {	
        Motor.Step++;
        if(Motor.Step>=6) Motor.Step=0;
        Motor.Step = Commutation(Motor.Step,PwmValue);//输出PWM信号，启动电机				
        delay_ms(Motor.Start_Delay);
        StartCount++;
    }while((Motor.StOK == 0) && (StartCount < 200));
            
     if(StartCount>=200)//200次换相还没启动成功，认为启动失败
    {
        Motor_Stop();
        return 0;
    }
            
     return 1;
}

/*检测换相步数*/
u8 Det_Step(MotorParam_TypeDef MotorParam)
{       

   if( MotorParam.Step >= 7)
   {
      MotorParam.ErrorFlag = 4; 
      return MotorParam.ErrorFlag;
   }
   return 0;
 
}