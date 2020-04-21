#include "global.h"

//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                  
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
void _ttywrch(int ch)
{
	ch = ch;
}    
void _sys_exit(int x)//移植正点原子的uart.c的这部分需要加函数类型为"void"  
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕    
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 


#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误      	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15,		接收完成标志
//bit14,		接收到0x0d
//bit13~0,	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记



//串口通信配置代码

void USART1Config(void)//串口初始化
{		
	/* 3.GPIOGPIO端口模式设置 */
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	/* 4.串口参数初始化 */
	USART_InitTypeDef USART_InitStructure;
	
	//将USART Tx的GPIO配置为推挽复用模式 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;//PA9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT,&GPIO_InitStructure);//初始化相应的IO口
	
	
	//将UASRT Rx的GPIO配置为输入浮空模式 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin =  DEBUG_USART_RX_GPIO_PIN;//PA10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT,&GPIO_InitStructure);
	
	/* 配置串口的工作参数 */
	
	//配置波特率
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	
	//配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	
	//配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	
	//完成串口初始化配置 
	USART_Init(DEBUG_USARTX,&USART_InitStructure);
	
	/* 6.使能串口 */
	USART_Cmd(DEBUG_USARTX,ENABLE);
	
	USART_ITConfig(DEBUG_USARTX,USART_IT_RXNE,ENABLE);//使能相关中断
}

#endif
/*
///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		//发送一个字节数据到串口   
		USART_SendData(DEBUG_USARTX, (uint8_t) ch);
		
		// 等待发送完毕 
		while (USART_GetFlagStatus(DEBUG_USARTX, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}
///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		//  等待串口输入数据 
		while (USART_GetFlagStatus(DEBUG_USARTX, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USARTX);
}
*/

