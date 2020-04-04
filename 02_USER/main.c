#include "global.h"
#include "delay.h"
#include "sys.h"

/*
 *@brief:板级支持包，初始化STM32F103外围硬件
 *@function:BspInit
 *@param:none
 *@retval:none
 */
static void BspInit()
{
	RCCConfigAll();
	
	GPIOConfigAll();

	NVICConfigAll();
	
	USART1Config();

	Timer3Config();

	
	///TemperatureAdcConfig();//初始化STM32F1内部温度采集器
	AdcConfig();
	AT24CXX_Init();
}

int main()
{
	delay_init();
	
	BspInit();
	
	while(1)
	{
		
	}
}