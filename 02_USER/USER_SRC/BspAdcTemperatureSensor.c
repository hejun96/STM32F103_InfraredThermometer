#include "global.h"
#include "delay.h"
#include "sys.h"



void TemperatureAdcConfig(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	//1.打开ADC_IO接口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1,ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//分频因子6,时钟为72MHz/6 = 12MHz
	ADC_DeInit(ADC1);//将外设ADC1的全部寄存器重设为缺省值
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//ADC独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//模数转换，单通道模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1,&ADC_InitStructure);
	
	ADC_TempSensorVrefintCmd(ENABLE);//开启内部温度传感器
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	ADC_ResetCalibration(ADC1);	//使能复位校准
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	ADC_StartCalibration(ADC1);	 //开启AD校准
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
}

u16 GetTemperatureAdc(u8 ch)
{
	ADC_RegularChannelConfig(ADC1,ch,1,ADC_SampleTime_239Cycles5);
	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));
	return ADC_GetConversionValue(ADC1);	
}

u16 GetTemperatureAverage(void)
{
	u16 temperatureValue = 0;
	u8 t;
	for(t=0;t<10;t++)
	{
		temperatureValue += GetTemperatureAdc(ADC_Channel_16);
		///delay_ms(5);
		if(TIMER2_1MS > 1000)
		{
			TIMER2_1MS = 0;
		}
	}
	return temperatureValue/10;
}

u16 GetTemperatureAdcAverage(u8 ch,u8 times)
{
	u32 TemperatureAdcAverageValue=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		TemperatureAdcAverageValue+=GetTemperatureAdc(ch);
		///delay_ms(5);
		
		if(TIMER2_1MS > 1000)
		{
			TIMER2_1MS = 0;
		}
		
	}
	return TemperatureAdcAverageValue/times;
}
/*
 *@brief:得到温度值
 *@function:GetTemperatureValue
 *@param:none
 *@retval:
 */

short GetTemperatureValue(void)
{
	u32 adcx;
	short result;
	double temperature;
	adcx = GetTemperatureAdcAverage(ADC_Channel_16,20);//获取通道16，且20次
	temperature = (float)adcx*(3.3/4096);
	temperature = (1.43 - temperature)/0.0043 + 25;
	result = temperature*=100;
	return result;
}


