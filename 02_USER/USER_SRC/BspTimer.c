
#include "global.h" 



///*
// * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
// * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
// * 另外三个成员是通用定时器和高级定时器才有.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            都有
// *	TIM_CounterMode			     TIMx,x[6,7]没有，其他都有
// *  TIM_Period               都有
// *  TIM_ClockDivision        TIMx,x[6,7]没有，其他都有
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]才有
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */


//1ms -> (arr:999 psc:71)

void TIM2Config(void)
{
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	
	
	// 开启定时器时钟,即内部时钟CK_INT=72M
   
	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
    TIM_TimeBaseStructure.TIM_Period = 999;// t=1/100KHz = 0.01ms 1ms定时器中断为1ms = 0.01ms * (99+1) ,故计数100次
	// 时钟预分频数
    TIM_TimeBaseStructure.TIM_Prescaler = 71;// (72MHz/100KHz)-1 = 719	
	// 时钟分频因子 ，没用到不用管
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// 计数器计数模式，设置为向上计数
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 		
	// 重复计数器的值，没用到不用管
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	//  初始化定时器
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	// 清除计数器中断标志位
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	  
	// 开启计数器中断
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	// 使能计数器
    TIM_Cmd(TIM2, ENABLE);//使能TIM3
}


//PB1-TIM3_CH4 复用BUZZER无源蜂鸣器
void TIM3PWMConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	
	
	//设置该引脚为复用输出功能,输出TIM3 CH4的PWM脉冲波形	GPIOB.1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	// 开启定时器时钟,即内部时钟CK_INT=72M
   
	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
    TIM_TimeBaseStructure.TIM_Period = 99;// t=1/100KHz = 0.01ms 1ms定时器中断为1ms = 0.01ms * (99+1) ,故计数100次
	// 时钟预分频数
    TIM_TimeBaseStructure.TIM_Prescaler = 719;// (72MHz/100KHz)-1 = 719	
	// 时钟分频因子 ，没用到不用管
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// 计数器计数模式，设置为向上计数
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 		
	// 重复计数器的值，没用到不用管
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	//  初始化定时器
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	// 清除计数器中断标志位
    ///TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	  
	// 开启计数器中断
    ///TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
	//初始化TIM3 Channel4 PWM
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;//选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCNPolarity_High;//输出极性:TIM输出比较极性高
	TIM_OC4Init(TIM3,&TIM_OCInitStructure);//根据T指定的参数初始化外设 TIM3 OC4
	
	TIM_OC4PreloadConfig(TIM3,TIM_OCPreload_Enable);//使能TIM3在CCR2上的预装载寄存器
	
	// 使能计数器
    TIM_Cmd(TIM3, ENABLE);//使能TIM3
}


/*
void delay_us(u32 i)//延时函数，延时us
{
    u32 temp;
    SysTick->LOAD = 9 * i;		 //设置重装数值, 72MHZ时
    SysTick->CTRL = 0X01;		 //使能，减到零是无动作，采用外部时钟源
    SysTick->VAL = 0;		   	 //清零计数器
    do
    {
        temp = SysTick->CTRL;		 //读取当前倒计数值
    }
    while((temp & 0x01) && (!(temp & (1 << 16))));	 //等待时间到达
    SysTick->CTRL = 0;	//关闭计数器
    SysTick->VAL = 0;		//清空计数器
}

void delay_ms(u32 i)//延时函数，延时ms
{
    u32 temp;
    SysTick->LOAD = 9000 * i;	 //设置重装数值, 72MHZ时
    SysTick->CTRL = 0X01;		//使能，减到零是无动作，采用外部时钟源
    SysTick->VAL = 0;			//清零计数器
    do
    {
        temp = SysTick->CTRL;	 //读取当前倒计数值
    }
    while((temp & 0x01) && (!(temp & (1 << 16))));	//等待时间到达
    SysTick->CTRL = 0;	//关闭计数器
    SysTick->VAL = 0;		//清空计数器
}
*/

/*********************************************END OF FILE**********************/
