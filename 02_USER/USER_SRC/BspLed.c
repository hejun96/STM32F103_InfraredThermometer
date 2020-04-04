#include "global.h"
 

void LEDCtrl(LED_TYPE LEDType,uint8_t LEDState)
{
	switch(LEDType)
	{
		case RED:
			if(LEDState == ON)
			{
				RED_LED(ON);
			}
			else
			{
				RED_LED(OFF);
			}
			break;
		case GREEN:
			if(LEDState == ON)
			{
				GREEN_LED(ON);
			}
			else
			{
				GREEN_LED(OFF);
			}
			break;
		case BLUE:
			if(LEDState == ON)
			{
				BLUE_LED(ON);
			}
			else
			{
				BLUE_LED(OFF);
			}
			break;
	}

}