#include "global.h"

volatile BUTTON_TYPE ButtonType;//按键类型变量
volatile STA_UNION SystemSta;


//温度的单位 ℃
uint8_t TempCompany[][16]=
{
	0x06,0x09,0x09,0xE6,0xF8,0x0C,0x04,0x02,0x02,0x02,0x02,0x02,0x04,0x1E,0x00,0x00,
	0x00,0x00,0x00,0x07,0x1F,0x30,0x20,0x40,0x40,0x40,0x40,0x40,0x20,0x10,0x00,0x00,/*"℃",0*/

};

/*
 *@brief:无源蜂鸣器规律发声
 *@function:void BuzzerVoiceRegulation
 *@param:uint8_t VoiceSize
 *@retval:none
 */

void BuzzerVoiceRegulation(uint8_t VoiceSize)
{
	TIM3->CCR4 = VoiceSize;
}

//红外测温传感器任务进程
void InfraredThermometerTask()
{
	uint8_t i,j;
	
	
	float Temperature = 0;//温度数据变量
	char TempValue[80] = {0};//温度值(字符串)
	char VoltageValueStr[80] = {0};//电压值(字符串)
	float VoltageValue = 0.0;//Vsimple电压值变量
	float VBAT = 0.0;//锂电池电压的变量值
	
	if(IsAnyButtonPress(ButtonType) == BUTTON_ON)
	{
		switch((uint8_t)ButtonType)
		{
			case EN_BUTTON_TYPE_UP:
				
				//进入该模式,只清一次屏
				if(SystemSta.s.CollectionFlag != FLAG_OFF)
				{
					if(FLAG_OFF == SystemSta.s.TempCleanScreenFlag)
					{
						OLED_DataClear();
						SystemSta.s.CollectionFlag++;
					}
				}
				//温度单位显示 (℃)
				for(i=5;i<6;i++)
				{
					j = i - 5;
					OLED_ShowCHinese16x16(i*16,2,j,TempCompany);	
				}
				
				Temperature = SMBus_ReadTemp();//读取温度
				sprintf(TempValue,"%.1f",Temperature);//浮点型转换为字符串
				OLED_ShowString(40,2,(uint8_t *)TempValue,16);   //显示温度
				
				//发热分为： 低热 ：37.2～38℃；中等度热：38．1～39℃：高热：39．1～41℃； 超高热 ：41℃以上
				//低烧预警
				if(Temperature >= 37.2 && Temperature <= 38.0)
				{
					//喇叭和红灯预警
					for(i=0;i<5;i++)
					{
						BuzzerVoiceRegulation(10);
						LEDCtrl(RED,ON);
						delay_ms(500);
						
						BuzzerVoiceRegulation(0);
						LEDCtrl(RED,OFF);
						delay_ms(500);
					}
				}
				//中烧预警
				else if(Temperature >= 38.1 && Temperature <= 39.0)
				{
					for(i=0;i<5;i++)
					{
						BuzzerVoiceRegulation(10);
						LEDCtrl(RED,ON);
						delay_ms(300);
						
						BuzzerVoiceRegulation(0);
						LEDCtrl(RED,OFF);
						delay_ms(300);
					}
					
				}
				//高烧预警
				else if(Temperature >= 39.1 && Temperature <= 41.0)
				{
					for(i=0;i<5;i++)
					{
						BuzzerVoiceRegulation(10);
						LEDCtrl(RED,ON);
						delay_ms(80);
						
						BuzzerVoiceRegulation(0);
						LEDCtrl(RED,OFF);
						delay_ms(80);
					}
				}
				//超高烧预警
				else if(Temperature > 41)
				{
					for(i=0;i<5;i++)
					{
						BuzzerVoiceRegulation(10);
						LEDCtrl(RED,ON);
						delay_ms(50);
						
						BuzzerVoiceRegulation(0);
						LEDCtrl(RED,OFF);
						delay_ms(50);
					}
				}
				SystemSta.s.VolCleanScreenFlag = FLAG_OFF;//清除电压清屏的标志位
				break;
				
			//下按键:实现采集电压功能，并在显示屏显示
			case EN_BUTTON_TYPE_DOWN://下按键
				if(SystemSta.s.CollectionFlag != FLAG_OFF)
				{
					//进入该模式，只请一次屏
					if(SystemSta.s.CollectionFlag == FLAG_OFF)
					{
						OLED_DataClear();//清除数据行的屏幕信息
						OLED_ShowChar(80,2,'V',16);
						SystemSta.s.VolCleanScreenFlag++;
					}
					
					VoltageValue = GetVoltageValue();
					//由于板子在电压采集的电路中加了电阻，所以在串联电路中，电阻起到的作用是:分压
					//故,锂电池的电压VBAT = VoltageValue*(10K + 10K)/10K
					
					VBAT = VoltageValue * (10 + 10)/10;
					sprintf(VoltageValueStr,"%.2f",VBAT);//浮点型转换为字符串
					//由于板子在电压采集的电路中加入了电阻所以加1.2V
					sprintf(VoltageValueStr,"%.2f",(VoltageValue + 1.20));//浮点型转换为字符串
					OLED_ShowString(40,2,(uint8_t *)VoltageValueStr,16);//显示温度
					
					//低压预警
					if(VoltageValue < 1.5)
					{
						for(i=0;i<5;i++)//闪烁5次
						{
							BuzzerVoiceRegulation(10);//喇叭提示
							LEDCtrl(GREEN,ON);//绿灯
							delay_ms(800);
							
							BuzzerVoiceRegulation(0);//
							LEDCtrl(GREEN,OFF);//绿灯
							delay_ms(800);
						}
					}
					SystemSta.s.TempCleanScreenFlag = FLAG_OFF;//清除温度显示标志
				}
				break;
			case EN_BUTTON_TYPE_LEFT:
				delay_us(10);
				OLED_Clear();//清屏
				delay_ms(500);//延时500ms
				OLED_DrawBMP(0,0,128,8,Peacock);//显示首界面
				
				SystemSta.s.CollectionFlag = FLAG_OFF;//采集标志清空
				SystemSta.s.TempCleanScreenFlag = FLAG_OFF;
				SystemSta.s.VolCleanScreenFlag = FLAG_OFF;
				break;
			
			//进入选择界面
			case EN_BUTTON_TYPE_RIGHT:
				
				OLED_Clear();//清屏
				delay_ms(500);
				OLED_DrawBMP(0,0,128,8,BMP1);//显示首界面
				OLED_ShowString(0,2,"(T):up (V):dowm ",16);//显示温度
				SystemSta.s.CollectionFlag++;//采集标志位++
				SystemSta.s.TempCleanScreenFlag = FLAG_OFF;
				SystemSta.s.VolCleanScreenFlag = FLAG_OFF;
				break;
			default:
				break;
		}		
	}
}
