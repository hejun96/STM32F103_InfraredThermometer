#ifndef __GLOBAL_H_
#define __GLOBAL_H_

#include "stm32f10x.h"
#include <stdio.h>
#include "sys.h"//RS485,TFTLCD用 类PBout()定义
#include "stdlib.h"//TFTLCD用

//SysTick
///#define delay_ms(x) 				delay_us(100*x)	 //ms

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
#define Temperature_BUTTON_GPIO_PIN	GPIO_Pin_15
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

//DMA
/*采集的通道数*/
#define SAMPLE_CHANNEL_NUM  		(1)//只需要1个DMA通道///(5)

/*一次采集的次数*/
#define SAMPLE_COUNT  				(10)  
#define ADC1_DR_ADDRESS             ((uint32_t)0x4001244C)
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

/*
#define DEVICE_I2C					1

#define	I2C_WR						0//写控制bit
#define I2C_RD						1//读控制bit
*/

//OLED_128*4*8
#define OLED_MODE 					0
#define SIZE 						8
#define XLevelL						0x00
#define XLevelH						0x10
#define Max_Column					128
#define Max_Row						64
#define	Brightness					0xFF 
#define X_WIDTH 					128
#define Y_WIDTH 					64

#define OLED_IIC_GPIO_PORT          GPIOB   

#define OLED_SCLK_GPIO_PIN  		GPIO_Pin_6
#define OLED_SDA_GPIO_PIN  			GPIO_Pin_7

#define OLED_SCLK_Clr() 			GPIO_WriteBit(OLED_IIC_GPIO_PORT, OLED_SCLK_GPIO_PIN,Bit_RESET)//SCL IIC接口的时钟信号
#define OLED_SCLK_Set() 			GPIO_WriteBit(OLED_IIC_GPIO_PORT, OLED_SCLK_GPIO_PIN,Bit_SET)

#define OLED_SDIN_Clr() 			GPIO_WriteBit(OLED_IIC_GPIO_PORT, OLED_SDA_GPIO_PIN,Bit_RESET)//SDA IIC接口的数据信号
#define OLED_SDIN_Set() 			GPIO_WriteBit(OLED_IIC_GPIO_PORT, OLED_SDA_GPIO_PIN,Bit_SET)

 		     
#define OLED_CMD  					0	//写命令
#define OLED_DATA 					1	//写数据

//MLX90614
#define ACK	 						0 //应答
#define	NACK 						1 //无应答
#define SA							0x00 //Slave address 单个MLX90614时地址为0x00,多个时地址默认为0x5a
#define RAM_ACCESS					0x00 //RAM access command RAM存取命令
#define EEPROM_ACCESS				0x20 //EEPROM access command EEPROM存取命令
#define RAM_TOBJ1					0x07 //To1 address in the eeprom 目标1温度,检测到的红外温度 -70.01 ~ 382.19度

#define SMBUS_PORT					GPIOB      //PB端口(端口和下面的两个针脚可自定义)
#define SMBUS_SCK					GPIO_Pin_10 //PB10：SCL
#define SMBUS_SDA					GPIO_Pin_11 //PB11：SDA


//SCL 引脚宏定义
#define SMBUS_SCK_H()	    		GPIO_WriteBit(SMBUS_PORT, SMBUS_SCK,Bit_SET)//置高电平
#define SMBUS_SCK_L()	    		GPIO_WriteBit(SMBUS_PORT, SMBUS_SCK,Bit_RESET)//置低电平
//SDA 引脚宏定义
#define SMBUS_SDA_H()	    		GPIO_WriteBit(SMBUS_PORT, SMBUS_SDA,Bit_SET) 
#define SMBUS_SDA_L()	    		GPIO_WriteBit(SMBUS_PORT, SMBUS_SDA,Bit_RESET)

//#define SMBUS_SDA_PIN()	    	SMBUS_PORT->IDR & SMBUS_SDA //读取引脚电平
#define SMBUS_SDA_PIN() 			GPIO_ReadInputDataBit(SMBUS_PORT, SMBUS_SDA)

//用户根据自己的需要设置
#define STM32_FLASH_SIZE 64 	 		//所选STM32的FLASH容量大小(单位为K) STM32F103C8T6 是64K 
#define STM32_FLASH_WREN 1              //使能FLASH写入(0，不是能;1，使能)


//FLASH起始地址
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址  

//STM32F103C8T6保存地址  0X0800FFFF  (0X0800FFF - 0X08000000)/1024 = 63K


//结构体，枚举体定义
enum
{
	FLAG_OFF = 0,
	FLAG_ON,
	FLAG_PAUSE
};

typedef enum
{
	RED = 0,
	GREEN,
	BLUE,
}LED_TYPE;

typedef enum
{
	BUTTON_TYPE_MENU = 0,
	EN_BUTTON_TYPE_UP,
	EN_BUTTON_TYPE_DOWN,
	EN_BUTTON_TYPE_LEFT,
	EN_BUTTON_TYPE_RIGHT,
	EN_BUTTON_TYPE_NONE,

}BUTTON_TYPE,*buttonType;
///extern volatile BUTTON_TYPE *ButtonType;

enum
{
	TIMER_RESET = 0,//定时器复位
	TIMER_RUN,//定时器运行
	TIMER_END,//定时器结束
	TIMER_PAUSE,//定时器暂停
};


typedef union
{
	u32 sta[8];
	struct
	{
		u32 TempCleanScreenFlag:2;//温度清屏标志位 0:复位 1:运行 2:完成 3:暂停
		u32 VolCleanScreenFlag:2;//电压清屏标志位 0:复位 1:运行 2:完成 3:暂停
		u32 CollectionFlag:2;//采集数据标志位 0:复位 1:运行 2:完成 3:暂停
		u32 ButtonTimerFlag:2;//按钮运行时间标志位 0:复位 1:运行 2:完成 3:暂停
		u32 StandbyTimerFlag:2;//低功耗待机标志位 0:复位 1:运行 2:完成 3:暂停
		
	}s;

}STA_UNION;
extern volatile STA_UNION SystemSta;

typedef struct
{
	u16 usButtonCnt;
	u16 usStandbyCnt;

}DATA_STRUCT;
extern volatile DATA_STRUCT SystemData;


/*
//封装描述软件I2C的操作函数集 File Operations 
struct sofI2cFops
{
	void (*GpioSet)(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);//设置GPIO输出高
	void (*GpioReset)(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);//设置GPIO输出高
	uint8_t (*GpioReadBit)(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);//读取GPIO电平
	///void (*delay_ms)(u16 nms);
	void (*delay_us)(u32 nus);
};
//
struct sofI2cInit
{
	uint32_t sdaPeriphClock;//SDA RCC时钟
	uint32_t sclPeriphClock;//SCL RCC时钟
	GPIO_TypeDef* sdaGpiox;
	GPIO_TypeDef* sclGpiox;

};

typedef struct __ArgSofI2CTypeDef
{
	struct sofI2cInit port;
	uint32_t scl;
	uint32_t sda;
	uint32_t TimeOut;
	struct sofI2cFops FOps;

}SofI2CTypeDef;

typedef struct __Argi2cDevice{
	SofI2CTypeDef* SofI2C;
	uint8_t SlaveAddr;
}i2cDevice;
*/

//函数声明

//SysTick

void TimingDelay_Decrement(void);

//GPIO
void rccConfigAll(void);
void gpioConfigAll(void);

static void nvicUsart1(void);
void nvicTimer2(void);
void nvicExti(void);

void nvicConfigAll(void);

void extiConfig(void);
//SysTick
void SysTick_Init(void);
///void delay_us(__IO uint32_t nTime);

void delay_us(u32 nus);

void delay_ms(u16 nms);

//LED
void LEDCtrl(LED_TYPE LEDType,uint8_t LEDState);

//异或:C语言的一个二进制运算符 与0异或不变,与1异或改变。相同为0，不同为1




void usart1ConfigAll(void);//串口初始化
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记		
//如果想串口中断接收，请不要注释以下宏定义
void uart_init(u32 bound);
int fputc(int ch, FILE *f);
int fgetc(FILE *f);


//BUTTON
void ButtonConfig(void);
///u8 ButtonScan(u8);
///uint8_t KeyScan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);

unsigned char IsAnyButtonPress(buttonType ButtonType);


//TIMER
void tim2Config(void);
void tim3PwmConfig(void);


/*
void delay_us(u32 i);
void delay_ms(u32 i);
*/
static void ADCDMAConfig(void);
static void ADCxGPIOConfig(void);
static void ADCxModeConfig(void);
void adcConfig(void);
u16 GetAdc(u8 ch);
u16 GetAdcAverage(u8 ch,u8 times);

float GetVoltageValue(void);

static void Voltage_ADC_Filter(void);
float Get_VoltageValue(void);
//ADC Temperature Sensor

void TemperatureadcConfig(void);
u16 GetTemperatureAdc(u8 ch);
u16 GetTemperatureAverage(void);
u16 GetTemperatureAdcAverage(u8 ch,u8 times);
short GetTemperatureValue(void);

/*
//I2C集(暂时只用到OLED和传感器)
extern SofI2CTypeDef sofI2cOled;
void i2cDelayUs(u32 nus);
void i2cGpioConfig(SofI2CTypeDef* sofI2cInode);
static void sdaOut(SofI2CTypeDef* sofI2cInode);
static void sdaIn(SofI2CTypeDef* sofI2cInode);
void i2cStart(SofI2CTypeDef* sofI2cInode);
void i2cStop(SofI2CTypeDef* sofI2cInode);
void i2cSendAck(SofI2CTypeDef* sofI2cInode,uint8_t ack);
*/




//OLED_128*4*8控制用函数
void oledConfig(void);

void OLED_WR_Byte(unsigned dat,unsigned cmd);  
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Clear(void);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);
void OLED_ShowString(uint8_t x,uint8_t y, uint8_t *p,uint8_t Char_Size);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void Delay_50ms(unsigned int Del_50ms);
void Delay_1ms(unsigned int Del_1ms);
void fill_picture(unsigned char fill_Data);
void Picture(void);
void OLED_ShowCHinese16x16(uint8_t x,uint8_t y,uint8_t num,uint8_t (*buf)[16]);
void IIC_Start(void);
void IIC_Stop(void);
void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);
void Write_IIC_Byte(unsigned char IIC_Byte);

void IIC_Wait_Ack(void);
void OLED_DataClear(void) ;

//IWDG
void iwdgConfig(u8 prer ,u16 rlr);
void IWDG_Feed(void);

//MXL90614
void SMBus_Init();
uint16_t SMBus_ReadMemory(uint8_t slaveAddress, uint8_t command);
uint8_t PEC_Calculation(uint8_t pec[]);
float SMBus_ReadTemp(void);

void SMBus_SendBit(uint8_t bit_out);
uint8_t SMBus_ReceiveBit(void);
void SMBus_Delay(uint16_t time);


//STMFlash
//STM32F103 FLASH

//FLASH解锁键值
 

u16 STMFLASH_ReadHalfWord(u32 faddr);		  //读出半字  
void STMFLASH_WriteLenByte(u32 WriteAddr,u32 DataToWrite,u16 Len);	//指定地址开始写入指定长度的数据
u32 STMFLASH_ReadLenByte(u32 ReadAddr,u16 Len);						//指定地址开始读取指定长度数据
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);		//从指定地址开始写入指定长度的数据
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);   		//从指定地址开始读出指定长度的数据

//测试写入
void Test_Write(u32 WriteAddr,u16 WriteData);


//POWER
void SysEnterStandby(void);

//Bsp --- Board Support Pakeage
static void BspInit();



//system
void BuzzerVoiceRegulation(uint8_t VoiceSize);
void ButtonTask();
extern unsigned char BMP1[]; 
extern unsigned char Peacock[]; 
#endif



