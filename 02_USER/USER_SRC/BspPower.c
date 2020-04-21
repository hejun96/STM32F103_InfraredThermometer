#include "global.h"

/*
void SysStandby(void)
{
	PWR_WakeUpPinCmd(ENABLE);//使能唤醒引脚功能
	PWR_EnterSTANDBYMode();//进入待机命令(STANDBY)
}
*/


void SysEnterStandby(void)
{
	if(SystemData.usStandbyCnt > 15000)
	{
		SystemData.usStandbyCnt = 0;
		//RCC_APB2PeriphResetCmd(0X01FC,DISABLE);//复位所有IO口
		///SysStandby();
		PWR_WakeUpPinCmd(ENABLE);//使能唤醒引脚功能
		PWR_EnterSTANDBYMode();//进入待机命令(STANDBY)
	}
}


