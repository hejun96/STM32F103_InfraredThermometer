#ifndef __GLOBAL_H_
#define __GLOBAL_H_

#include "stm32f10x.h"
#include <stdio.h>
#include "sys.h"//RS485,TFTLCD用 类PBout()定义
#include "stdlib.h"//TFTLCD用

//LED
//LED_GPIO
#define RGB_LED_CLK					RCC_APB2Periph_GPIOA
#define RGB_LED_GPIO_PORT			GPIOA
#define RED_LED_GPIO_PIN			GPIO_Pin_8

#define GREEN_LED_GPIO_PIN			GPIO_Pin_11

#define BLUE_LED_GPIO_PIN			GPIO_Pin_12

/*
//直接操作寄存器的方法控制IO
#define	digitalHi(p,i)		 					{p->BSRR=i;}	 //输出为高电平				
#define digitalLo(p,i)		 					{p->BRR=i;}	 //输出低电平
#define digitalToggle(p,i) 					{p->ODR ^=i;} //输出反转状态
#define RED_LED_TOGGLE()						digitalToggle(RED_LED_GPIO_PORT,RED_LED_GPIO_PIN)
#define YELLOW_LED_TOGGLE()					digitalToggle(YELLOW_LED_GPIO_PORT,YELLOW_LED_GPIO_PIN)
*/
#define RED_LED_TOGGLE()			{RED_LED_GPIO_PORT -> ODR ^= RED_LED_GPIO_PIN;}
#define YELLOW_LED_TOGGLE()			{YELLOW_LED_GPIO_PORT -> ODR ^= YELLOW_LED_GPIO_PIN;}

#define ON							0//低电平有效
#define OFF							1

//RGB灯对应引脚置高置低电平的宏定义
#define RED_LED(a)					if(a) GPIO_WriteBit(RGB_LED_GPIO_PORT,RED_LED_GPIO_PIN,Bit_SET); \
											else GPIO_WriteBit(RGB_LED_GPIO_PORT,RED_LED_GPIO_PIN,Bit_RESET)
												
#define GREEN_LED(a)				if(a) GPIO_WriteBit(RGB_LED_GPIO_PORT,GREEN_LED_GPIO_PIN,Bit_SET); \
											else GPIO_WriteBit(RGB_LED_GPIO_PORT,GREEN_LED_GPIO_PIN,Bit_RESET)

#define BLUE_LED(a)					if(a) GPIO_WriteBit(RGB_LED_GPIO_PORT,BLUE_LED_GPIO_PIN,Bit_SET); \
											else GPIO_WriteBit(RGB_LED_GPIO_PORT,BLUE_LED_GPIO_PIN,Bit_RESET)											
//RS232
//USART1 - 串口1 宏定义
#define DEBUG_USARTX				USART1
#define DEBUG_USART_CLK				RCC_APB2Periph_USART1
#define DEBUG_USART_APBXCLKCMD 		RCC_APB2PeriphClockCmd
#define DEBUG_USART_BAUDRATE		9600
//USART GPIO引脚宏定义
#define DEBUG_USART_GPIO_CLK		RCC_APB2Periph_GPIOA
#define DEBUG_USART_GPIO_APBXCLKCMD	RCC_APB2PeriphClockCmd
#define DEBUG_USART_TX_GPIO_PORT	GPIOA
#define DEBUG_USART_TX_GPIO_PIN		GPIO_Pin_9
#define DEBUG_USART_RX_GPIO_PORT	GPIOA
#define DEBUG_USART_RX_GPIO_PIN		GPIO_Pin_10
#define DEBUG_USARTX_IRQ			USART1_IRQn
#define DEBUG_USARTX_IRQHandler		USART1_IRQHandler
#define USART_REC_LEN  				200  	//定义最大接收字节数 200
#define EN_USART1_RX 				1		//使能（1）/禁止（0）串口1接收	  

//BUZZER_GPIO
#define BUZZER_CLK					RCC_APB2Periph_GPIOB
#define BUZZER_GPIO_PORT			GPIOB
#define BUZZER_GPIO_PIN				GPIO_Pin_1
#define BUZZER_TOGGLE()				{BUZZER_GPIO_PORT -> ODR ^= BUZZER_GPIO_PIN;}

//BUTTON_GPIO 

#define ALL_BUTTON_CLK				RCC_APB2Periph_GPIOB
#define ALL_BUTTON_GPIO_PORT		GPIOB
#define TEMPERATURE_BUTTON_GPIO_PIN	GPIO_Pin_15
#define VOLTAGE_BUTTON_GPIO_PIN		GPIO_Pin_14
#define BACK_BUTTON_GPIO_PIN		GPIO_Pin_13
#define SELECT_BUTTON_GPIO_PIN		GPIO_Pin_12


//按键按下标置宏
//按键按下为高电平，设置 KEY_ON=1， KEY_OFF=0
//若按键按下为低电平，把宏设置成KEY_ON=0 ，KEY_OFF=1 即可
#define BUTTON_ON				0
#define BUTTON_OFF				1

//TIMER
//


//ADC
//ADC_GPIO宏定义
#define ADC_GPIO_APBxClockFUN		RCC_APB2PeriphClockCmd
#define ADC_GPIO_CLK				RCC_APB2Periph_GPIOA
#define ADC_PORT					GPIOA
#define ADC_PIN						GPIO_Pin_11
//ADC编号选择
#define ADC_APBxClock_FUN			RCC_APB2PeriphClockCmd
#define ADC_x						ADC1
#define ADC_CLk						RCC_APB2Periph_ADC1
#define ADC_CHANNEL							
#define ADC_IRQ						ADC1_2_IRQn
#define ADC_IRQHandler				ADC1_2_IRQHnadler


//IIC

//IO方向设置
 
#define SDA_IN()  					{GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
#define SDA_OUT() 					{GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}

//IO操作函数	 
#define IIC_SCL    					PBout(6) //SCL
#define IIC_SDA    					PBout(7) //SDA	 
#define READ_SDA   					PBin(7)  //输入SDA 


//AT24CXX
#define AT24C01						127
#define AT24C02						255
#define AT24C04						511
#define AT24C08						1023
#define AT24C16						2047
#define AT24C32						4095
#define AT24C64	    				8191
#define AT24C128					16383
#define AT24C256					32767  

//Mini/Warship STM32开发板均使用的是24c02，所以定义EE_TYPE为AT24C02
#define EE_TYPE AT24C02


//函数声明

//GPIO
void RCCConfigAll(void);
void GPIOConfigAll(void);

static void NVIC_USART1(void);
void NVIC_TIMER3(void);
void NVICConfigAll(void);


//LED


//异或:C语言的一个二进制运算符 与0异或不变,与1异或改变。相同为0，不同为1

typedef enum
{
	RED = 0,
	GREEN,
	BLUE,
}LED_TYPE;
void LEDCtrl(LED_TYPE LEDType,uint8_t LEDState);



void USART1Config(void);//串口初始化
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记		
//如果想串口中断接收，请不要注释以下宏定义
void uart_init(u32 bound);
int fputc(int ch, FILE *f);
int fgetc(FILE *f);



//OLCD_128*32

//LCD重要参数集
typedef struct  
{										    
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u16 id;				//LCD ID
	u8  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	u16	wramcmd;		//开始写gram指令
	u16  setxcmd;		//设置x坐标指令
	u16  setycmd;		//设置y坐标指令 
}_lcd_dev; 	  



//BUZZER



//BUTTON
void ButtonConfig(void);
///u8 ButtonScan(u8);
///uint8_t KeyScan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);
typedef enum
{
	BUTTON_TYPE_MENU = 0,
	EN_BUTTON_TYPE_UP,
	EN_BUTTON_TYPE_DOWN,
	EN_BUTTON_TYPE_LEFT,
	EN_BUTTON_TYPE_RIGHT,
	EN_BUTTON_TYPE_NONE,

}KEY_TYPE;
unsigned char KeyIsAnyButtonPress(KEY_TYPE KeyType);


//TIMER
void Timer3Config(void);


/*
void delay_us(u32 i);
void delay_ms(u32 i);
*/

static void AdcxGPIOConfig(void);
static void AdcxModeConfig(void);
void AdcConfig(void);
u16 GetAdc(u8 ch);
u16 GetAdcAverage(u8 ch,u8 times);


//ADC Temperature Sensor

void TemperatureAdcConfig(void);
u16 GetTemperatureAdc(u8 ch);
u16 GetTemperatureAverage(void);
u16 GetTemperatureAdcAverage(u8 ch,u8 times);
short GetTemperatureValue(void);



//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	



					  
u8 AT24CXX_ReadOneByte(u16 ReadAddr);							//指定地址读取一个字节
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite);		//指定地址写入一个字节
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len);//指定地址开始写入指定长度的数据
u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len);					//指定地址开始读取指定长度数据
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);	//从指定地址开始写入指定长度的数据
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead);   	//从指定地址开始读出指定长度的数据

u8 AT24CXX_Check(void);  //检查器件
void AT24CXX_Init(void); //初始化IIC


//IWDG
void IWDGConfig(u8 prer ,u16 rlr);
void IWDG_Feed(void);


//Bsp --- Board Support Pakeage
static void BspInit();

#endif



