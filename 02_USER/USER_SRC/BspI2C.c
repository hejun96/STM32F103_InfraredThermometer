#include "BspI2c.h"
//使用芯知识学堂东方青老师讲解的I2C面向对象高级用法驱动


#ifdef DEVICE_I2C

void i2cDelayUs(u32 nus)
{
	void delay_us(u32 nus);
}

void i2cDelayMs(u16 nms)
{
	void delay_ms(u16 nms);
}


SofI2CTypeDef sofI2cOled = 
{
	.port = 
	{
		.sdaPeriphClock = RCC_APB2Periph_GPIOB,
		.sclPeriphClock = RCC_APB2Periph_GPIOB,
		.sdaGpiox = GPIOB,
		.sclGpiox = GPIOB,
	},
	
	.sda = GPIO_Pin_7,//OLED SDA
	.scl = GPIO_Pin_6,//OLED SCL
	.Fops = 
	{
		.GpioSet = GPIO_SetBits,
		.GpioReset = GPIO_ResetBits,
		.GpioReadBit = GPIO_ReadInputDataBit,
		.delay_us = i2cDelayUs,
		.delay_ms = i2cDelayMs,
	},
};


void i2cGpioConfig(SofI2CTypeDef* sofI2cInode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(sofI2cInode->port.sdaPeriphClock,ENABLE);
	RCC_APB2PeriphClockCmd(sofI2cInode->port.sclPeriphClock,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = sofI2cInode->scl;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(sofI2cInode->port.sclGpiox,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = sofI2cInode->sda;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(sofI2cInode->port.sdaGpiox,&GPIO_InitStructure);
	
	GPIO_SetBits(sofI2cInode->port.sclGpiox,sofI2cInode->scl);
	GPIO_SetBits(sofI2cInode->port.sdaGpiox,sofI2cInode->sda);
}

//sdaIn和sdaOut只是SDA IO口的方向是做输入还是输出用
static void sdaOut(SofI2CTypeDef* sofI2cInode)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = sofI2cInode->sda;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_Init(sofI2cInode->port.sdaGpiox,&GPIO_InitStructure);
	
	sofI2cInode->Fops.GpioSet(sofI2cInode->port.sdaGpiox,sofI2cInode->sda);
	
}

static void sdaIn(SofI2CTypeDef* sofI2cInode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = sofI2cInode->sda;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//带上拉输入
	GPIO_Init(sofI2cInode->port.sdaGpiox,&GPIO_InitStructure);
	
	sofI2cInode->Fops.GpioSet(sofI2cInode->port.sdaGpiox,sofI2cInode->sda);//此行代码因为上面sdaOut也出现过，可要可不要，此行代码可换成加延时


}

void i2cStart(SofI2CTypeDef* sofI2cInode)
{
	sdaOut(sofI2cInode);
	sofI2cInode->Fops.GpioSet(sofI2cInode->port.sdaGpiox,sofI2cInode->sda);
	sofI2cInode->Fops.GpioSet(sofI2cInode->port.sclGpiox,sofI2cInode->scl);
	sofI2cInode->Fops.delay_us(8);
	sofI2cInode->Fops.GpioReset(sofI2cInode->port.sdaGpiox,sofI2cInode->sda);
	sofI2cInode->Fops.delay_us(8);
	sofI2cInode->Fops.GpioReset(sofI2cInode->port.sclGpiox,sofI2cInode->scl);
}

void i2cStop(SofI2CTypeDef* sofI2cInode)
{
	sdaOut(sofI2cInode);
	sofI2cInode->Fops.GpioReset(sofI2cInode->port.sdaGpiox,sofI2cInode->sda);
	sofI2cInode->Fops.GpioSet(sofI2cInode->port.sclGpiox,sofI2cInode->scl);
	sofI2cInode->Fops.delay_us(8);
	sofI2cInode->Fops.GpioSet(sofI2cInode->port.sdaGpiox,sofI2cInode->sda);
	sofI2cInode->Fops.delay_us(8);
}

void i2cSendAck(SofI2CTypeDef* sofI2cInode,uint8_t ack)
{
	sdaOut(sofI2cInode);
	sofI2cInode->Fops.GpioReset(sofI2cInode->port.sclGpiox,sofI2cInode->scl);
	sofI2cInode->Fops.delay_us(8);
	if(ack)
	{
		sofI2cInode->Fops.GpioSet(sofI2cInode->port.sdaGpiox,sofI2cInode->sda);
	}
	else
	{
		sofI2cInode->Fops.GpioReset(sofI2cInode->port.sdaGpiox,sofI2cInode->sda);
	
	}
	sofI2cInode->Fops.GpioSet(sofI2cInode->port.sclGpiox,sofI2cInode->scl);
	sofI2cInode->Fops.delay_us(8);
	sofI2cInode->Fops.GpioReset(sofI2cInode->port.sclGpiox,sofI2cInode->scl);
	sofI2cInode->Fops.delay_us(8);
}

uint8_t i2cRecvAck(SofI2CTypeDef* sofI2cInode)
{
	u8 ucErrTime = 0;
	uint8_t ucValue = 0;
	
	sdaIn(sofI2cInode);
	sofI2cInode->Fops.GpioSet(sofI2cInode->port.sdaGpiox,sofI2cInode->sda);
	sofI2cInode->Fops.delay_us(4);
	sofI2cInode->Fops.GpioSet(sofI2cInode->port.sclGpiox,sofI2cInode->scl);
	sofI2cInode->Fops.delay_us(4);
	ucValue = sofI2cInode->Fops.GpioReadBit(sofI2cInode->port.sdaGpiox,sofI2cInode->sda);
	while(ucValue)
	{
		ucErrTime++;
		if(ucErrTime > 250)
		{
			i2cStop(sofI2cInode);
			return 1;
		}
		ucValue = sofI2cInode->Fops.GpioReadBit(sofI2cInode->port.sdaGpiox,sofI2cInode->sda);
		
	}
	sofI2cInode->Fops.GpioReset(sofI2cInode->port.sclGpiox,sofI2cInode->scl);
	return 0;

}

void i2cSendByte(SofI2CTypeDef* sofI2cInode,uint8_t dat)
{
	uint8_t t;
	sdaOut(sofI2cInode);
	sofI2cInode->Fops.GpioReset(sofI2cInode->port.sclGpiox,sofI2cInode->scl);
	for(t=0;t<8;t++)
	{
		if(dat&0x80)
		{
			sofI2cInode->Fops.GpioSet(sofI2cInode->port.sdaGpiox,sofI2cInode->sda);
		}
		else
		{
			sofI2cInode->Fops.GpioReset(sofI2cInode->port.sdaGpiox,sofI2cInode->sda);
		}
		dat <<= 1;
		sofI2cInode->Fops.GpioSet(sofI2cInode->port.sclGpiox,sofI2cInode->scl);
		sofI2cInode->Fops.delay_us(5);
		sofI2cInode->Fops.GpioReset(sofI2cInode->port.sclGpiox,sofI2cInode->scl);
		sofI2cInode->Fops.delay_us(5);
	}
}

uint8_t i2cRecvByte(SofI2CTypeDef* sofI2cInode)
{
	uint8_t i;
	uint8_t ucByte = 0;
	uint8_t ucValue = 0;
	
	sdaIn(sofI2cInode);
	for(i=0;i<8;i++)
	{
		sofI2cInode->Fops.delay_us(5);
		sofI2cInode->Fops.GpioSet(sofI2cInode->port.sclGpiox,sofI2cInode->scl);
		sofI2cInode->Fops.delay_us(5);
		ucByte <<= 1;
		ucValue = sofI2cInode->Fops.GpioReadBit(sofI2cInode->port.sdaGpiox,sofI2cInode->sda);
		if(ucValue)
		{
			ucByte |= 0x01;
			
		}
		sofI2cInode->Fops.GpioReset(sofI2cInode->port.sclGpiox,sofI2cInode->scl);
		sofI2cInode->Fops.delay_us(5);
		
	
	}
	return ucByte;
}

/*
 *@brief:检测总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
 *@function:
 *@param:
 *@retval:
 */
uint8_t i2cCheckDevice(SofI2CTypeDef* sofI2cInode,uint8_t _Address)
{
	uint8_t ucAck;
	i2cStart(sofI2cInode);//发送启动信号
	
	//发送设备地址+读写控制bit(0=w,1=r)bit7先传
	i2cSendByte(sofI2cInode,_Address | I2C_WR);
	ucAck = i2cRecvAck(sofI2cInode);//检测设备的ACK应答
	i2cStop(sofI2cInode);//发送停止设备
	return ucAck;
}



/******************************************************************
*	函 数 名: eeprom_ReadBytes
*	功能说明: 从串行EEPROM指定地址处开始读取若干数据
*	形    参：dev-------:I2C总线上的设备
*			 _usAddress : 起始地址
*			 _usSize : 数据长度，单位为字节
*			 _pReadBuf : 存放读到的数据的缓冲区指针
*	返 回 值: 0 表示失败，1表示成功
********************************************************************/
uint8_t writeI2cData(i2cDevice* dev,unsigned char I2CData)
{
	
	//第一步：发起I2C总线启动信号
	i2cStart(dev->SofI2C);
	
	//第二步：发起控制字节，高7bit是地址,bit0是读写控制位，0表示写，1表示读
	i2cSendByte(dev->SofI2C,dev->SlaveAddr|I2C_WR);
	
	/* 第3步：发送一个时钟，判断器件是否正确应答 */
	if (i2cRecvAck(dev->SofI2C) == 0)
	{
		///break;
	}

	/* 第4步：发送字节地址*/

	i2cSendByte(dev->SofI2C,0x40);
			
	/* 第5步：发送ACK */
	if (i2cRecvAck(dev->SofI2C) != 0)
	{
		goto cmd_fail;	/* 器件无应答 */
	}
		
	
	/* 第6步：开始写入数据 */
	i2cSendByte(dev->SofI2C,I2CData);
	
	/* 第7步：发送ACK */
	if (i2cRecvAck(dev->SofI2C) != 0)
	{
		goto cmd_fail;	/* 器件无应答 */
	}
	/* 发送I2C总线停止信号 */
	i2cStop(dev->SofI2C);
	return 1;	/* 执行成功 */

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	i2cStop(dev->SofI2C);
//	printf("Read fild!\r\n");
	return 0;
}


/**********************************************************************************
*	函 数 名: eeprom_WriteBytes
*	功能说明: 向串行EEPROM指定地址写入若干数据，采用页写操作提高写入效率
*	形    参：dev-------:I2C总线上的设备
*			 _usAddress : 起始地址
*			 _usSize : 数据长度，单位为字节
*			 _pWriteBuf : 存放读到的数据的缓冲区指针
*	返 回 值: 0 表示失败，1表示成功
**********************************************************************************/
uint8_t writeI2cCommand(i2cDevice* dev,unsigned char i2cCommand)
{
	
	/* 第1步：发起I2C总线启动信号 */
	i2cStart(dev->SofI2C);

	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2cSendByte(dev->SofI2C,dev->SlaveAddr|I2C_WR);

	/* 第3步：发送一个时钟，判断器件是否正确应答 */
	if (i2cRecvAck(dev->SofI2C) == 0)
	{
		///break;
	}

	/* 第4步：发送字节地址*/

	i2cSendByte(dev->SofI2C,0x00);
			
	/* 第5步：发送ACK */
	if (i2cRecvAck(dev->SofI2C) != 0)
	{
		goto cmd_fail;	/* 器件无应答 */
	}
		
	
	/* 第6步：开始写入数据 */
	i2cSendByte(dev->SofI2C,i2cCommand);
	
	/* 第7步：发送ACK */
	if (i2cRecvAck(dev->SofI2C) != 0)
	{
		goto cmd_fail;	/* 器件无应答 */
	}
	/* 命令执行成功，发送I2C总线停止信号 */
	i2cStop(dev->SofI2C);
	return 1;

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	i2cStop(dev->SofI2C);
//	printf("Write fild!\r\n");
	return 0;
}

//举例，具体看东方青老师的额温枪项目讲解视频
i2cDevice i2cOledDev = {
	.SofI2C = &sofI2cOled,
	.SlaveAddr = 0x78,//
};

void oledWriteByte(unsigned data,unsigned cmd)
{
	if(cmd)
	{
		writeI2cData(&i2cOledDev,data);
	}
	else
	{
		writeI2cCommand(&i2cOledDev,data);
	}
}

void oledInit(SofI2CTypeDef* sofI2cInode)
{
	i2cGpioConfig(&sofI2cOled);
	sofI2cInode->Fops.delay_ms(200);	
	
	oledWriteByte(0xAE,I2C_WR);//关闭显示
	
	oledWriteByte(0x40,I2C_WR);//---set low column address
	oledWriteByte(0xB0,I2C_WR);//---set high column address

	oledWriteByte(0xC8,I2C_WR);//-not offset

	oledWriteByte(0x81,I2C_WR);//设置对比度
	oledWriteByte(0xff,I2C_WR);

	oledWriteByte(0xa1,I2C_WR);//段重定向设置

	oledWriteByte(0xa6,I2C_WR);//
	
	oledWriteByte(0xa8,I2C_WR);//设置驱动路数
	oledWriteByte(0x1f,I2C_WR);
	
	oledWriteByte(0xd3,I2C_WR);
	oledWriteByte(0x00,I2C_WR);
	
	oledWriteByte(0xd5,I2C_WR);
	oledWriteByte(0xf0,I2C_WR);
	
	oledWriteByte(0xd9,I2C_WR);
	oledWriteByte(0x22,I2C_WR);
	
	oledWriteByte(0xda,I2C_WR);
	oledWriteByte(0x02,I2C_WR);
	
	oledWriteByte(0xdb,I2C_WR);
	oledWriteByte(0x49,I2C_WR);
	
	oledWriteByte(0x8d,I2C_WR);
	oledWriteByte(0x14,I2C_WR);
	
	oledWriteByte(0xaf,I2C_WR);
	OLED_Clear();
}


#endif