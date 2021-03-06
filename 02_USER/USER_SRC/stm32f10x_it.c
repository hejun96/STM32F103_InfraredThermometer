/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

///#include "FreeRTOS.h"//FreeRTOS使用
///#include "task.h"

#include "global.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */

void SVC_Handler(void)
{
}


/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */

void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */

/*
extern void xPortSysTickHandler(void);
//systick中断服务函数，FreeRTOS用
void SysTick_Handler(void)
{
	#if (INCLUDE_xTaskGetSchedulerState  == 1 )
      if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
      {
    #endif  // INCLUDE_xTaskGetSchedulerState   
        ///xPortSysTickHandler();
    #if (INCLUDE_xTaskGetSchedulerState  == 1 )
      }
    #endif  // INCLUDE_xTaskGetSchedulerState 
	///xPortSysTickHandler();
}

*/

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */


/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

void SysTick_Handler(void)
{
	TimingDelay_Decrement();
}


//7.中断处理函数
void USART1_IRQHandler(void)
{
	u8 res;//定义一个变量，将接收到的值赋给变量
	if(USART_GetITStatus(USART1,USART_IT_RXNE))//获取中断状态标志位
	{
		res = USART_ReceiveData(USART1);//接收数据到串口
		USART_SendData(USART1,res);//发数据到串口
	}
}
//通用定时器中断
void TIM2_IRQHandler(void)
{
	//溢出中断
	if(TIM2->SR&0x0001)
	{
		//按键定时器中断
		if(TIMER_RUN == SystemSta.s.ButtonTimerFlag)
		{
			SystemData.usButtonCnt++;
		}
		else if(TIMER_RESET == SystemSta.s.ButtonTimerFlag)
		{
			SystemData.usButtonCnt = 0;
		}
		
		if(TIMER_RUN == SystemSta.s.StandbyTimerFlag)
		{
			SystemData.usStandbyCnt++;
		}
		if(TIMER_RESET == SystemSta.s.StandbyTimerFlag)
		{
			SystemData.usStandbyCnt = 0;
		}
		
		
	}
	
	/*清除中断标志位*/
	TIM2->SR&=~(1<<0);   
}
/*
//按键中断
volatile BUTTON_TYPE ButtonType = EN_BUTTON_TYPE_NONE;
void EXTI15_10_IRQHandler(void)
{
	if(TIMER_RESET == SystemSta.s.ButtonTimerFlag)
	{
		SystemSta.s.ButtonTimerFlag = TIMER_RUN;
		ButtonType = EN_BUTTON_TYPE_UP;
	}
	EXTI_ClearITPendingBit(EXTI_Line15);
	
	if(TIMER_RESET == SystemSta.s.ButtonTimerFlag)
	{
		SystemSta.s.ButtonTimerFlag = TIMER_RUN;
		ButtonType = EN_BUTTON_TYPE_DOWN;
	}
	EXTI_ClearITPendingBit(EXTI_Line12);
	
	if(TIMER_RESET == SystemSta.s.ButtonTimerFlag)
	{
		SystemSta.s.ButtonTimerFlag = TIMER_RUN;
		ButtonType = EN_BUTTON_TYPE_LEFT;
	}
	EXTI_ClearITPendingBit(EXTI_Line13);
	
	if(TIMER_RESET == SystemSta.s.ButtonTimerFlag)
	{
		SystemSta.s.ButtonTimerFlag = TIMER_RUN;
		ButtonType = EN_BUTTON_TYPE_RIGHT;
	}
	EXTI_ClearITPendingBit(EXTI_Line14);
}
*/

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
