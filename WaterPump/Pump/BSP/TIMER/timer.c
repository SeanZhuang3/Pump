#include "timer.h"  
#include "confer.h"




void Tim6_Init(void)
{
//  TIM6_TimeBaseInit(TIM6_PRESCALER_128,125);//初始化定时器4
//  TIM6_ARRPreloadConfig(ENABLE);//使能自动重装
//  ITC_SetSoftwarePriority(ITC_IRQ_TIM6_OVFTRI , ITC_PRIORITYLEVEL_1); //设置优先级
//  TIM6_ITConfig(TIM6_IT_UPDATE , ENABLE);//数据更新中断
//  TIM6_Cmd(ENABLE);//开定时器
    
    
    TIM6->CR1 = 0x81;
    TIM6->CR2  = 0;
    TIM6->IER = 0x01;

    TIM6->EGR = 0;
    TIM6->ARR = 0x7d ;
    TIM6->PSCR = 0x07;
    TIM6->CNTR = 0;
  
}

void Tim6_ReInit(void)
{
  TIM6->CR1 	= 0X00;
  TIM6->CR2 	= 0X00;
  TIM6->SMCR 	= 0X00;
  TIM6->IER 	= 0X00;
  TIM6->CNTR 	= 0X00;
  TIM6->PSCR	= 0X00;
  TIM6->ARR 	= 0XFF;
  TIM6->SR1 	= 0X00;
//  TIM6_DeInit();
  
  TIM6->CR1 &= 0xF0;
//  TIM6_Cmd(DISABLE);//关定时器
}

void delay_us(u16 nCount)   //16M 晶振时  延时 1个微妙
{
    nCount*=3;
    while(--nCount);
}


void delay_ms(u16 nCount)  //16M 晶振时  延时 1个毫秒
{
    while(nCount--)\
    {
        delay_us(1000);\
    }
}  