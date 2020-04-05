#include "global.h"
#include "bmp.h"
#include "sys.h"

/*
 *@brief:板级支持包，初始化STM32F103外围硬件
 *@function:BspInit
 *@param:none
 *@retval:none
 */


static void BspInit()
{
	SysTick_Init();
	
	RCCConfigAll();
	
	GPIOConfigAll();

	NVICConfigAll();
	
	USART1Config();

	TIM3_PWM_INIT();
	
	OLEDConfig();
	
	SMBus_Init();//MXL90614
	
	///TemperatureAdcConfig();//初始化STM32F1内部温度采集器
	AdcConfig();
	
	IWDGConfig(4,625);
}

int main()
{
	BspInit();
	
	//启动无操作界面	
	OLED_DrawBMP(0,0,128,8,Peacock);
	
	while(1)
	{
		InfraredThermometerTask();
		
		IWDG_Feed();
	}
}