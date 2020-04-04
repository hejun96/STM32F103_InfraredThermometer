#include "global.h"

u16 adcx;
float temp;
static void AdcxGPIOConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//1.配置ADC_IO引脚模式，模拟输入
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//ADC对应PA1口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//模拟输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
}


static void AdcxModeConfig(void)
{	
	ADC_InitTypeDef ADC_InitStructure;
	//1.打开ADC_IO接口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	ADC_DeInit(ADC1);
	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_Init(ADC1,&ADC_InitStructure);
	
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	
	ADC_ResetCalibration(ADC1);	//使能复位校准
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	
	ADC_StartCalibration(ADC1);	 //开启AD校准
	
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
	
}
void AdcConfig(void)
{
	AdcxGPIOConfig();
	AdcxModeConfig();
}
u16 GetAdc(u8 ch)
{
	ADC_RegularChannelConfig(ADC1,ch,1,ADC_SampleTime_239Cycles5);
	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));
	return ADC_GetConversionValue(ADC1);	
}

u16 GetAdcAverage(u8 ch,u8 times)
{
	u32 AdcAverageValue=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		AdcAverageValue+=GetAdc(ch);
		///delay_ms(5);
		
	}
	return AdcAverageValue/times;
}


