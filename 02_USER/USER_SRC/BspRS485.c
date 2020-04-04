#include "global.h"
#include "delay.h"

//战舰开发板上面只有232的USB口，485打印信息可以用TFTLCD,并且需要另一块带RS485的板子双方连接

//初始化USART2
//串口通信配置代码
static void Usart2RS485NvicConfig(void)
{	
	//5.开启中断并且初始化NVIC 
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* 嵌套向量中断控制寄存器组选择 */
	///NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置NVIC中断优先级分组，2位抢占优先级和2位响应优先级	
	
	//配置USART为中断源 
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	
	// 使能中断 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	
	//抢占优先级为3
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	
	//子优先级为3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
		
	//初始化NVIC 
	NVIC_Init(&NVIC_InitStructure);
}

void Usart2RS485Config()
{
	/* 3.GPIOGPIO端口模式设置 */
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	/* 4.串口参数初始化 */
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//PA口时钟使能
	
	//1.串口时钟使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	//RS485 PD7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//PD7
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_InitStructure);//初始化相应的IO口
		
	//将USART Tx的GPIO配置为推挽复用模式 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//PA2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);//初始化相应的IO口
	
	//将UASRT Rx的GPIO配置为输入浮空模式 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3;//PA3
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	/* 配置串口的工作参数 */
	
	//配置波特率
	USART_InitStructure.USART_BaudRate = 9600;
	
	//配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	
	//配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	
	//完成串口初始化配置 
	USART_Init(USART2,&USART_InitStructure);
	
	/* 6.使能串口 */
	USART_Cmd(USART2,ENABLE);
	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//使能相关中断
	
	Usart2RS485NvicConfig();//串口中断优先级配置
	
	///GPIO_ResetBits(GPIOD,GPIO_Pin_7);//默认为接收模式
	RS485_TX_EN = 0;//默认为接收模式
}

void RS485SendData(u8 *buf,u8 len)
{
	u8 t;
	///GPIO_SetBits(GPIOD,GPIO_Pin_7);//设置为发送模式
	RS485_TX_EN = 1;//设置为发送模式
	for(t = 0;t < len;t++)
	{
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);
		USART_SendData(USART2,buf[t]);
		
	}
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);
	RS485_RX_CNT = 0;
	///GPIO_ResetBits(GPIOD,GPIO_Pin_7);//设置为接收模式
	
	RS485_TX_EN = 0;//设置为接收模式
}

void RS485ReceiveData(u8 *buf,u8 *len)
{
	u8 rxlen = RS485_RX_CNT;
	u8 i = 0;
	*len = 0;
	delay_ms(10);
	if(rxlen == RS485_RX_CNT && rxlen)
	{
		for(i = 0;i < rxlen;i++)
		{
			buf[i] = RS485_RX_BUF[i];
		}
		*len = RS485_RX_CNT;
		RS485_RX_CNT = 0;
	}
}
#ifdef EN_USART2_RX	//如果使能了接收
//接收缓冲区

u8 RS485_RX_BUF[64];
u8 RS485_RX_CNT = 0;
void USART2_IRQHandle(void)
{
	u8 res;
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)
	{
		res = USART_ReceiveData(USART2);
		if(RS485_RX_CNT < 64)
		{
			RS485_RX_BUF[RS485_RX_CNT] = res;
			RS485_RX_CNT++;
		}
	}
}
#endif