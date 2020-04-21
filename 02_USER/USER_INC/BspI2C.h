#ifndef __BSPI2C_H_
#define	__BSPI2C_H_

#include "global.h"

#define DEVICE_I2C					1

#ifdef	DEVICE_I2C

#define	I2C_WR						0//写控制bit
#define I2C_RD						1//读控制bit

//封装描述软件I2C的操作函数集 File Operations 
struct SofI2CFOps
{
	void (*GPIOSet)(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);//设置GPIO输出高
	void (*GPIOReset)(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);//设置GPIO输出高
	uint8_t (*GPIOReadBit)(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);//读取GPIO电平
	///void (*delay_ms)(u16 nms);
	void (*delay_us)(u32 nus);
};
//
struct SofI2CInit
{
	uint32_t SDaPeriphClock;//SDA RCC时钟
	uint32_t SClPeriphClock;//SCL RCC时钟
	GPIO_TypeDef* SDaGPIOx;
	GPIO_TypeDef* SClGPIOx;

};

typedef struct __ArgSofI2CTypeDef
{
	struct SofI2CInit port;
	uint32_t scl;
	uint32_t sda;
	uint32_t TimeOut;
	struct SofI2CFOps FOps;

}SofI2CTypeDef;

typedef struct __ArgI2CDevice{
	SofI2CTypeDef* SofI2C;
	uint8_t SlaveAddr;
}I2CDevice;

extern SofI2CTypeDef SofI2COLED;

//I2C集(暂时只用到OLED和传感器)


void I2CGPIOConfig(SofI2CTypeDef* SofI2CInode);
void I2CDelayUs(u32 nus);
void I2CGPIOConfig(SofI2CTypeDef* SofI2CInode);
static void SDaOut(SofI2CTypeDef* SofI2CInode);
static void SDaIn(SofI2CTypeDef* SofI2CInode);
void I2CStart(SofI2CTypeDef* SofI2CInode);
void I2CStop(SofI2CTypeDef* SofI2CInode);
void I2CSendACK(SofI2CTypeDef* SofI2CInode,uint8_t ack);
void SofI2CInit();
#endif

#endif