#ifndef __BSPI2C_H_
#define	__BSPI2C_H_

#include "global.h"

#define DEVICE_I2C					1

#ifdef	DEVICE_I2C

#define	I2C_WR						0//写控制bit
#define I2C_RD						1//读控制bit

//封装描述软件I2C的操作函数集 File Operations 
struct sofI2cFops
{
	void (*GpioSet)(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);//设置GPIO输出高
	void (*GpioReset)(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);//设置GPIO输出高
	uint8_t (*GpioReadBit)(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);//读取GPIO电平
	
	void (*delay_us)(u32 nus);
	void (*delay_ms)(u16 nms);
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
	struct sofI2cFops Fops;

}SofI2CTypeDef;

typedef struct __Argi2cDevice
{
	SofI2CTypeDef* SofI2C;
	uint8_t SlaveAddr;
}i2cDevice;

extern SofI2CTypeDef sofI2cOled;

//I2C集(暂时只用到OLED和传感器)



void i2cDelayUs(u32 nus);
void i2cDelayMs(u16 nms);

void i2cGpioConfig(SofI2CTypeDef* sofI2cInode);
static void sdaOut(SofI2CTypeDef* sofI2cInode);
static void sdaIn(SofI2CTypeDef* sofI2cInode);
void i2cStart(SofI2CTypeDef* sofI2cInode);
void i2cStop(SofI2CTypeDef* sofI2cInode);
void i2cSendAck(SofI2CTypeDef* sofI2cInode,uint8_t ack);

void oledWriteByte(unsigned data,unsigned cmd);
void oledInit(SofI2CTypeDef* sofI2cInode);

#endif

#endif