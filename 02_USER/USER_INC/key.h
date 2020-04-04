#ifndef __KEY_H_
#define __KEY_H_

#include "sys.h"


#define KEY0 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)
#define KEY1 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)
#define KEY2 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)
#define KEY_UP 	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)

#define KEY0_PRES		1//KEY0按下
#define KEY1_PRES 	2//KEY1按下
#define KEY2_PRES		3//KEY2按下
#define KEY_UP_PRES 4//KEY_UP按下   此处有响应优先级的概念，优先级KEY0 > KEY1 > KEY2 > KEY_UP ,所以KEY_UP不能定义在其他按键前面


void KEY_Init(void);
u8 KEY_Scan(u8);

#endif