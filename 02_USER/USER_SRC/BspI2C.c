#include "BspI2c.h"
//使用芯知识学堂东方青老师讲解的I2C面向对象高级用法驱动


#ifdef DEVICE_I2C

void I2CDelayUs(u32 nus)
{
	void delay_us(u32 nus);
}




SofI2CTypeDef SofI2COLED = 
{
	.port = 
	{
		.SDaPeriphClock = RCC_APB2Periph_GPIOB,
		.SClPeriphClock = RCC_APB2Periph_GPIOB,
		.SDaGPIOx = GPIOB,
		.SClGPIOx = GPIOB,
	},
	
	.sda = GPIO_Pin_7,//OLED SDA
	.scl = GPIO_Pin_6,//OLED SCL
	.FOps = 
	{
		.GPIOSet = GPIO_SetBits,
		.GPIOReset = GPIO_ResetBits,
		.GPIOReadBit = GPIO_ReadInputDataBit,
		.delay_us = I2CDelayUs,
		
	},
};


void I2CGPIOConfig(SofI2CTypeDef* SofI2CInode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(SofI2CInode->port.SDaPeriphClock,ENABLE);
	RCC_APB2PeriphClockCmd(SofI2CInode->port.SClPeriphClock,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = SofI2CInode->scl;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SofI2CInode->port.SClGPIOx,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SofI2CInode->sda;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SofI2CInode->port.SDaGPIOx,&GPIO_InitStructure);
	
	GPIO_SetBits(SofI2CInode->port.SClGPIOx,SofI2CInode->scl);
	GPIO_SetBits(SofI2CInode->port.SDaGPIOx,SofI2CInode->sda);
}

//SdaIn和SdaOut只是SDA IO口的方向是做输入还是输出用
static void SDaOut(SofI2CTypeDef* SofI2CInode)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = SofI2CInode->sda;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_Init(SofI2CInode->port.SDaGPIOx,&GPIO_InitStructure);
	
	SofI2CInode->FOps.GPIOSet(SofI2CInode->port.SDaGPIOx,SofI2CInode->sda);
	
}

static void SDaIn(SofI2CTypeDef* SofI2CInode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = SofI2CInode->sda;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//带上拉输入
	GPIO_Init(SofI2CInode->port.SDaGPIOx,&GPIO_InitStructure);
	
	SofI2CInode->FOps.GPIOSet(SofI2CInode->port.SDaGPIOx,SofI2CInode->sda);//此行代码因为上面SdaOut也出现过，可要可不要，此行代码可换成加延时


}

void I2CStart(SofI2CTypeDef* SofI2CInode)
{
	SDaOut(SofI2CInode);
	SofI2CInode->FOps.GPIOSet(SofI2CInode->port.SDaGPIOx,SofI2CInode->sda);
	SofI2CInode->FOps.GPIOSet(SofI2CInode->port.SClGPIOx,SofI2CInode->scl);
	SofI2CInode->FOps.delay_us(8);
	SofI2CInode->FOps.GPIOReset(SofI2CInode->port.SDaGPIOx,SofI2CInode->sda);
	SofI2CInode->FOps.delay_us(8);
	SofI2CInode->FOps.GPIOReset(SofI2CInode->port.SClGPIOx,SofI2CInode->scl);
}

void I2CStop(SofI2CTypeDef* SofI2CInode)
{
	SDaOut(SofI2CInode);
	SofI2CInode->FOps.GPIOReset(SofI2CInode->port.SDaGPIOx,SofI2CInode->sda);
	SofI2CInode->FOps.GPIOSet(SofI2CInode->port.SClGPIOx,SofI2CInode->scl);
	SofI2CInode->FOps.delay_us(8);
	SofI2CInode->FOps.GPIOSet(SofI2CInode->port.SDaGPIOx,SofI2CInode->sda);
	SofI2CInode->FOps.delay_us(8);
}

void I2CSendACK(SofI2CTypeDef* SofI2CInode,uint8_t ack)
{
	SDaOut(SofI2CInode);
	SofI2CInode->FOps.GPIOReset(SofI2CInode->port.SClGPIOx,SofI2CInode->scl);
	SofI2CInode->FOps.delay_us(8);
	if(ack)
	{
		SofI2CInode->FOps.GPIOSet(SofI2CInode->port.SDaGPIOx,SofI2CInode->sda);
	}
	else
	{
		SofI2CInode->FOps.GPIOReset(SofI2CInode->port.SDaGPIOx,SofI2CInode->sda);
	
	}
	SofI2CInode->FOps.GPIOSet(SofI2CInode->port.SClGPIOx,SofI2CInode->scl);
	SofI2CInode->FOps.delay_us(8);
	SofI2CInode->FOps.GPIOReset(SofI2CInode->port.SClGPIOx,SofI2CInode->scl);
	SofI2CInode->FOps.delay_us(8);
}

uint8_t I2CRecvACK(SofI2CTypeDef* SofI2CInode)
{
	u8 ucErrTime = 0;
	uint8_t ucValue = 0;
	
	SDaIn(SofI2CInode);
	SofI2CInode->FOps.GPIOSet(SofI2CInode->port.SDaGPIOx,SofI2CInode->sda);
	SofI2CInode->FOps.delay_us(4);
	SofI2CInode->FOps.GPIOSet(SofI2CInode->port.SClGPIOx,SofI2CInode->scl);
	SofI2CInode->FOps.delay_us(4);
	ucValue = SofI2CInode->FOps.GPIOReadBit(SofI2CInode->port.SDaGPIOx,SofI2CInode->sda);
	while(ucValue)
	{
		ucErrTime++;
		if(ucErrTime > 250)
		{
			I2CStop(SofI2CInode);
			return 1;
		}
		ucValue = SofI2CInode->FOps.GPIOReadBit(SofI2CInode->port.SDaGPIOx,SofI2CInode->sda);
		
	}
	SofI2CInode->FOps.GPIOReset(SofI2CInode->port.SClGPIOx,SofI2CInode->scl);
	return 0;

}

void I2CSendByte(SofI2CTypeDef* SofI2CInode,uint8_t dat)
{
	uint8_t t;
	SDaOut(SofI2CInode);
	SofI2CInode->FOps.GPIOReset(SofI2CInode->port.SClGPIOx,SofI2CInode->scl);
	for(t=0;t<8;t++)
	{
		if(dat&0x80)
		{
			SofI2CInode->FOps.GPIOSet(SofI2CInode->port.SDaGPIOx,SofI2CInode->sda);
		}
		else
		{
			SofI2CInode->FOps.GPIOReset(SofI2CInode->port.SDaGPIOx,SofI2CInode->sda);
		}
		dat <<= 1;
		SofI2CInode->FOps.GPIOSet(SofI2CInode->port.SClGPIOx,SofI2CInode->scl);
		SofI2CInode->FOps.delay_us(5);
		SofI2CInode->FOps.GPIOReset(SofI2CInode->port.SClGPIOx,SofI2CInode->scl);
		SofI2CInode->FOps.delay_us(5);
	
	}

}



uint8_t I2CRecvByte(SofI2CTypeDef* SofI2CInode)
{
	uint8_t i;
	uint8_t ucByte = 0;
	uint8_t ucValue = 0;
	
	SDaIn(SofI2CInode);
	for(i=0;i<8;i++)
	{
		SofI2CInode->FOps.delay_us(5);
		SofI2CInode->FOps.GPIOSet(SofI2CInode->port.SClGPIOx,SofI2CInode->scl);
		SofI2CInode->FOps.delay_us(5);
		ucByte <<= 1;
		ucValue = SofI2CInode->FOps.GPIOReadBit(SofI2CInode->port.SDaGPIOx,SofI2CInode->sda);
		if(ucValue)
		{
			ucByte |= 0x01;
			
		}
		SofI2CInode->FOps.GPIOReset(SofI2CInode->port.SClGPIOx,SofI2CInode->scl);
		SofI2CInode->FOps.delay_us(5);
		
	
	}
	return ucByte;
}

/*
 *@brief:检测总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
 *@function:
 *@param:
 *@retval:
 */
uint8_t I2CCheckDrivce(SofI2CTypeDef* SofI2CInode,uint8_t _Address)
{
	uint8_t ucAck;
	I2CStart(SofI2CInode);//发送启动信号
	
	//发送设备地址+读写控制bit(0=w,1=r)bit7先传
	I2CSendByte(SofI2CInode,_Address | I2C_WR);
	ucAck = I2CRecvACK(SofI2CInode);//检测设备的ACK应答
	I2CStop(SofI2CInode);//发送停止设备
	return ucAck;
}


void SofI2CInit()
{
	I2CGPIOConfig(&SofI2COLED);

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
uint8_t I2CDevReadBytes(I2CDevice* dev,
							uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize)
{
	uint16_t i;
	
	I2CStart(dev->SofI2C);
	I2CSendByte(dev->SofI2C,dev->SlaveAddr|I2C_WR);
	
	/* 第3步：发送ACK */
	if (I2CRecvACK(dev->SofI2C) != 0)
	{
		goto cmd_fail;	
	}

	/* 第4步：发送字节地址 */
	I2CSendByte(dev->SofI2C,(uint8_t)_usAddress&0xff);
	
	/* 第5步：发送ACK */
	if (I2CRecvACK(dev->SofI2C) != 0)
	{
		goto cmd_fail;	/* 器件无应答 */
	}
	
	/* 第6步：重新启动I2C总线。下面开始读取数据 */
	I2CStart(dev->SofI2C);
	
	/* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	I2CSendByte(dev->SofI2C,dev->SlaveAddr|I2C_RD);
	/* 第8步：发送ACK */
	if (I2CRecvACK(dev->SofI2C) != 0)
	{
		goto cmd_fail;	/* 器件无应答 */
	}	
	
	/* 第9步：循环读取数据 */
	for (i = 0; i < _usSize; i++)
	{
		_pReadBuf[i] = I2CRecvByte(dev->SofI2C);	/* 读1个字节 */
		
		/* 每读完1个字节后，需要发送Ack， 最后一个字节不需要Ack，发Nack */
		if (i != _usSize - 1)
		{
			I2CSendACK(dev->SofI2C,0);	/* 中间字节读完后，CPU产生ACK信号(驱动SDA = 0) */
		}
		else
		{
			I2CSendACK(dev->SofI2C,1);	/* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
		}
	}
	/* 发送I2C总线停止信号 */
	I2CStop(dev->SofI2C);
	return 1;	/* 执行成功 */

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	I2CStop(dev->SofI2C);
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
uint8_t I2CDevWriteBytes(I2CDevice* dev,
							uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize)
{
	uint16_t i,m;
	uint16_t usAddr;
	

	usAddr = _usAddress;
	for (i = 0; i < _usSize; i++)
	{
		/* 当发送第1个字节或是页面首地址时，需要重新发起启动信号和地址 */
		if (i == 0)
		{
			/*　第０步：发停止信号，启动内部写操作　*/
			I2CStop(dev->SofI2C);
			
			/* 通过检查器件应答的方式，判断内部写操作是否完成, 一般小于 10ms 			
				CLK频率为200KHz时，查询次数为30次左右
			*/
			for (m = 0; m < 100; m++)
			{				
				/* 第1步：发起I2C总线启动信号 */
				I2CStart(dev->SofI2C);
				
				/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
				I2CSendByte(dev->SofI2C,dev->SlaveAddr|I2C_WR);
				
				/* 第3步：发送一个时钟，判断器件是否正确应答 */
				if (I2CRecvACK(dev->SofI2C) == 0)
				{
					break;
				}
			}
			/* 第4步：发送字节地址*/
			
			I2CSendByte(dev->SofI2C,(uint8_t)usAddr%256);
			
			/* 第5步：发送ACK */
			if (I2CRecvACK(dev->SofI2C) != 0)
			{
				goto cmd_fail;	/* 器件无应答 */
			}
		}
	
		/* 第6步：开始写入数据 */
		I2CSendByte(dev->SofI2C,_pWriteBuf[i]);
	
		/* 第7步：发送ACK */
		if (I2CRecvACK(dev->SofI2C) != 0)
		{
			goto cmd_fail;	/* 器件无应答 */
		}

		usAddr++;	/* 地址增1 */		
	}
	
	/* 命令执行成功，发送I2C总线停止信号 */
	I2CStop(dev->SofI2C);
	return 1;

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	I2CStop(dev->SofI2C);
//	printf("Write fild!\r\n");
	return 0;
}


//举例，具体看东方青老师的额温枪项目讲解视频
I2CDevice i2c_1_dev1 = {
	.SofI2C = &SofI2COLED,
	.SlaveAddr = 0x0c,
};

I2CDevice i2c_1_dev2 = {
	.SofI2C = &SofI2COLED,
	.SlaveAddr = 0x0d,
};






#endif