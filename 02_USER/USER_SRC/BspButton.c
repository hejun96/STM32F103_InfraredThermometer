#include "global.h"
#include "delay.h"


unsigned char KeyIsAnyButtonPress(KEY_TYPE KeyType)
{
	//检测温度按键是否按下
	if(GPIO_ReadInputDataBit(ALL_BUTTON_GPIO_PORT,TEMPERATURE_BUTTON_GPIO_PIN) == BUTTON_ON)
	{
		//消抖
		delay_ms(30);
		if(GPIO_ReadInputDataBit(ALL_BUTTON_GPIO_PORT,TEMPERATURE_BUTTON_GPIO_PIN) == BUTTON_ON)
		{
			while(GPIO_ReadInputDataBit(ALL_BUTTON_GPIO_PORT,TEMPERATURE_BUTTON_GPIO_PIN) == BUTTON_ON);
			KeyType = EN_BUTTON_TYPE_UP;//上按键
			return BUTTON_ON;
		}
	}
	
	if(GPIO_ReadInputDataBit(ALL_BUTTON_GPIO_PORT,SELECT_BUTTON_GPIO_PIN) == BUTTON_ON)
	{
		//消抖
		delay_ms(30);
		if(GPIO_ReadInputDataBit(ALL_BUTTON_GPIO_PORT,SELECT_BUTTON_GPIO_PIN) == BUTTON_ON)
		{
			while(GPIO_ReadInputDataBit(ALL_BUTTON_GPIO_PORT,SELECT_BUTTON_GPIO_PIN) == BUTTON_ON);
			KeyType = EN_BUTTON_TYPE_RIGHT;//右按键
			return BUTTON_ON;
		}
	}
	
	if(GPIO_ReadInputDataBit(ALL_BUTTON_GPIO_PORT,BACK_BUTTON_GPIO_PIN) == BUTTON_ON)
	{
		//消抖
		delay_ms(30);
		if(GPIO_ReadInputDataBit(ALL_BUTTON_GPIO_PORT,BACK_BUTTON_GPIO_PIN) == BUTTON_ON)
		{
			while(GPIO_ReadInputDataBit(ALL_BUTTON_GPIO_PORT,BACK_BUTTON_GPIO_PIN) == BUTTON_ON);
			KeyType = EN_BUTTON_TYPE_LEFT;//左按键
			return BUTTON_ON;
		}
	}
	
	if(GPIO_ReadInputDataBit(ALL_BUTTON_GPIO_PORT,VOLTAGE_BUTTON_GPIO_PIN) == BUTTON_ON)
	{
		//消抖
		delay_ms(30);
		//再次检测按键是否被按下
		if(GPIO_ReadInputDataBit(ALL_BUTTON_GPIO_PORT,VOLTAGE_BUTTON_GPIO_PIN) == BUTTON_ON)
		{
			while(GPIO_ReadInputDataBit(ALL_BUTTON_GPIO_PORT,VOLTAGE_BUTTON_GPIO_PIN) == BUTTON_ON);
			KeyType = EN_BUTTON_TYPE_DOWN;//下按键
			return BUTTON_ON;
		}
	}

	return BUTTON_OFF;
}
