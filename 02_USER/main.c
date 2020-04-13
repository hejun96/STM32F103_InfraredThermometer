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
	
	///EXTIConfig();
	
	USART1Config();

	TIM2Config();
	
	TIM3PWMConfig();
	
	OLEDConfig();
	
	SMBus_Init();//MXL90614
	
	///fTemperatureAdcConfig();//初始化STM32F1内部温度采集器
	ADCConfig();
	
	IWDGConfig(4,625);
}

int main()
{
	BspInit();
	
	//启动无操作界面	
	OLED_DrawBMP(0,0,128,8,Peacock);///OLED显示"海东青"图案
	
	while(1)
	{
		InfraredThermometerTask();
		
		IWDG_Feed();
	}
}