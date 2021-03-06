#include "global.h"

void rccConfigAll(void)
{
	//GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB,ENABLE);//GPIOA,GPIOB
	
	//USART1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	//ADC1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	//TIM2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	//TIM3
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	//DMA
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	//POWER
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);//使能PWM外设时钟
}

void gpioConfigAll(void)	
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);
	
	//A Output PA8-RED_LED PA11-GREEN_LED PA12-BLUE_LED
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速率
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12);
	
	//B Input PB12-BUTTON4 PB13-BUTTON3 PB14-BUTTON2 PB15-BUTTON1 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//按键均为浮空输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;//
	///GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速率
	GPIO_SetBits(GPIOB,GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);//输入1，关闭按键
	
	
	
	//B Output	PB1-BUZZER PB6-I2C1_SCL PB7-I2C1_SDA
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出，PB1无源蜂鸣器
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;///|GPIO_Pin_6|GPIO_Pin_7;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速率
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_SetBits(BUZZER_GPIO_PORT,BUZZER_GPIO_PIN);//输出1，关闭蜂鸣器
	
}

static void nvicUsart1(void)
{
	//5.开启中断并且初始化NVIC 
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* 嵌套向量中断控制寄存器组选择 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//设置NVIC中断优先级分组，2位抢占优先级和2位响应优先级	
	
	//配置USART为中断源 
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	
	// 使能中断 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	
	//抢占优先级为1 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	
	//子优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		
	//初始化NVIC 
	NVIC_Init(&NVIC_InitStructure);
}


//中断优先级配置
static void nvicTimer2(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);		
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	
		// 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	 
	  // 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

//按键的外部中断
static void nvicExti(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);		
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;	
		// 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	 
	  // 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;//BUTTON_DOWN

	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;//BUTTON_LEFT
	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;//BUTTON_RIGHT
	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;//BUTTON_UP
	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void nvicConfigAll(void)
{
	nvicUsart1();
	nvicTimer2();
	///nvicExti();
}


void extiConfig(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	
	//GPIOB.15-Temperature_BUTTON 中断线以及中断初始化 下降沿触发
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource15);
	EXTI_InitStructure.EXTI_Line = EXTI_Line15;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	//GPIOB.14-VOLTAGE_BUTTON 中断线以及中断初始化 下降沿触发
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);
	EXTI_InitStructure.EXTI_Line = EXTI_Line14;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	//GPIOB.13-BACK_BUTTON_GPIO 中断线以及中断初始化 下降沿触发
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource13);
	EXTI_InitStructure.EXTI_Line = EXTI_Line13;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	//GPIOB.12-SELECT_BUTTON_GPIO 中断线以及中断初始化 下降沿触发
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource12);
	EXTI_InitStructure.EXTI_Line = EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}
