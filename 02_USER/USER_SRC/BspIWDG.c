#include "global.h"


//Tout = ((4*2^prer)*rlr)/40
//1000ms => ((4*2^4)*625)/40 
void IWDGConfig(u8 prer ,u16 rlr)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//1.使能对寄存器写操作
	IWDG_SetPrescaler(IWDG_Prescaler_64);//2.设置IWDG预分频值
	IWDG_SetReload(rlr);//2.设置IWDG预分频重装载值
	IWDG_ReloadCounter();//3.按照IWDG重装载寄存器的值重装载IWDG计数器
	IWDG_Enable();//4.使能IWDG
}


void IWDG_Feed(void)
{
	IWDG_ReloadCounter();//自动重装载reload
}