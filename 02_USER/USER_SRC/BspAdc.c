#include "global.h"
#include "delay.h"

/*采集存放的AD值*/
volatile u16 ADC_ConvertedValue[SAMPLE_CHANNEL_NUM*SAMPLE_COUNT]; 

//PA7-Voltage ADC
static void ADCxGPIOConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//1.配置ADC_IO引脚模式，模拟输入
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//ADC对应PA1口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//模拟输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
}


static void ADCDMAConfig(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	// 配置 DMA 初始化结构体
	// 外设基址为：ADC 数据寄存器地址
	
	/*使能DMA1通道1*/
	DMA_DeInit(DMA1_Channel1);
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_ADDRESS;
	
	// 存储器地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_ConvertedValue;
	
	// 数据源来自外设
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	
	// 缓冲区大小，应该等于数据目的地的大小
	DMA_InitStructure.DMA_BufferSize = 1;
	
	// 外设寄存器只有一个，地址不用递增
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

	// 存储器地址递增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
	
	// 外设数据大小为半字，即两个字节
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	
	// 内存数据大小也为半字，跟外设数据大小相同
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	
	// 循环传输模式
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	

	// DMA 传输通道优先级为高，当使用一个DMA通道时，优先级设置不影响
	///DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	
	// 禁止存储器到存储器模式，因为是从外设到存储器
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	// 初始化DMA
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	// 使能 DMA 通道
	DMA_Cmd(DMA1_Channel1 , ENABLE);

}
//ADC2没有DMA,故使用DMA1
static void ADCxModeConfig(void)
{	
	ADC_InitTypeDef ADC_InitStructure;
	//1.打开ADC_IO接口时钟
	ADCDMAConfig();
	
	// 只使用一个ADC，属于单模式
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//6分频
	ADC_DeInit(ADC1);
	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_NbrOfChannel = 1;//单通道采集
	
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_Init(ADC1,&ADC_InitStructure);
	
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	
	ADC_ResetCalibration(ADC1);	//使能复位校准
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	
	ADC_StartCalibration(ADC1);	 //开启AD校准
	
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
	
}
void ADCConfig(void)
{
	ADCxGPIOConfig();
	ADCxModeConfig();
}
u16 GetAdc(u8 ch)
{
	ADC_RegularChannelConfig(ADC1,ch,1,ADC_SampleTime_239Cycles5);
	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));
	return ADC_GetConversionValue(ADC1);	
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
	float fValue;
	
	adcx = GetAdcAverage(ADC_Channel_7,10);//PA7-AD_IN7，采集10次
	fValue = (float)adcx*(3.3/4096);
}
