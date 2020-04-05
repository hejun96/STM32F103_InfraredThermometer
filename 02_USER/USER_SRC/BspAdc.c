#include "global.h"
#include "delay.h"

//PA7-Voltage ADC
static void AdcxGPIOConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//1.配置ADC_IO引脚模式，模拟输入
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//ADC对应PA1口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//模拟输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
}


static void AdcxModeConfig(void)
{	
	ADC_InitTypeDef ADC_InitStructure;
	//1.打开ADC_IO接口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//6分频
	ADC_DeInit(ADC2);
	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_Init(ADC2,&ADC_InitStructure);
	
	ADC_Cmd(ADC2, ENABLE);	//使能指定的ADC1
	
	ADC_ResetCalibration(ADC2);	//使能复位校准
	 
	while(ADC_GetResetCalibrationStatus(ADC2));	//等待复位校准结束
	
	ADC_StartCalibration(ADC2);	 //开启AD校准
	
	while(ADC_GetCalibrationStatus(ADC2));	 //等待校准结束
	
}
void AdcConfig(void)
{
	AdcxGPIOConfig();
	AdcxModeConfig();
}
u16 GetAdc(u8 ch)
{
	ADC_RegularChannelConfig(ADC2,ch,1,ADC_SampleTime_239Cycles5);
	
	ADC_SoftwareStartConvCmd(ADC2,ENABLE);
	
	while(!ADC_GetFlagStatus(ADC2,ADC_FLAG_EOC));
	return ADC_GetConversionValue(ADC2);	
}

/*
 *@brief:获取ADC的平均值
 *@function:u16 GetAdcAverage
 *@param:u8 ch,u8 times
 *@retval:AdcAverageValue/times
 */


u16 GetAdcAverage(u8 ch,u8 times)
{
	u32 AdcAverageValue=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		AdcAverageValue+=GetAdc(ch);
		delay_ms(5);
	}
	return AdcAverageValue/times;
}

float GetVoltageValue(void)
{
	u16 adcx;
	float VoltageValue;
	
	adcx = GetAdcAverage(ADC_Channel_7,10);//PA7-AD_IN7
	VoltageValue = (float)adcx*(3.3/4096);
}
