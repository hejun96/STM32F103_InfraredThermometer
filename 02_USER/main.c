#include "global.h"
#include "bmp.h"
///#include "sys.h"
#include "BspI2C.h"
/*
 *@brief:板级支持包，初始化STM32F103外围硬件
 *@function:BspInit
 *@param:none
 *@retval:none
 */


static void BspInit()
{
	SysTick_Init();
	
	rccConfigAll();
	
	gpioConfigAll();

	nvicConfigAll();
	
	///extiConfig();
	
	usart1ConfigAll();

	tim2Config();
	
	tim3PwmConfig();
	
	oledInit(&sofI2cOled);
	///oledConfig();
	
	SMBus_Init();//MXL90614
	
	///TemperatureadcConfig();//初始化STM32F1内部温度采集器
	adcConfig();
	
	iwdgConfig(4,625);
}

int main()
{
	BspInit();
	
	//启动无操作界面	
	OLED_DrawBMP(0,0,128,8,Peacock);///OLED显示"海东青"图案
	
	while(1)
	{
		///delay_ms(500);
		///SystemSta.s.StandbyTimerFlag = TIMER_RUN;
		///printf("Hello STM32F103_InfraredThermometer\n");
		///SysEnterStandby();
		
		ButtonTask();
		
		IWDG_Feed();
	}
}