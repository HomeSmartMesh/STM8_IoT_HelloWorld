/** @file WS2812B.h
 *
 * @author Wassim FILALI
 * Inferred from the Datasheet
 * 
 *
 * @compiler IAR STM8
 *
 *
 * started on local project on 10.10.2015
 * this file was created for refactoring on 03.01.2016
 * $Date:
 * $Revision:
 *
 */

#include <iostm8s103f3.h>

#include "commonTypes.h"
#include "ClockUartLed.h"
#include "WS2812B.h"

void TestColors()
{
  BlinkColors(1,0,0);//Blink Red
  BlinkColors(1,0,0);//Twice
  BlinkColors(0,1,0);//Blink Green
  BlinkColors(0,1,0);//Twice
  BlinkColors(0,0,1);//Blink Blue
  BlinkColors(0,0,1);//Twice
  delay_1ms_Count(500);//rest a while

  BlinkColors(1,1,0);//Blink RG
  BlinkColors(0,1,1);//Blink GB
  BlinkColors(1,0,1);//Blink RB

  delay_1ms_Count(500);//rest a while

  RampColors(5,1,1,1,0,0,0);//All Up
  RampColors(5,2,2,2,0,0,0);//All Down

  SwitchOffLed();
  delay_1ms_Count(500);//rest a while
  
  //Red Stays 0, Green stays 100, Blue Goes Up then Down
  //Flash strange
  RampColors(0,0,0,1,20,100,0);
  RampColors(0,0,0,2,20,100,0);
  SwitchOffLed();
  delay_1ms_Count(500);
  //replay the Flash strange in slow motion
  RampColors(10,0,0,1,20,100,0);
  RampColors(10,0,0,2,20,100,0);
  SwitchOffLed();
}


int main( void )
{

	InitialiseSystemClock();

	Initialise_TestLed_GPIO_B5();
	Test_Led_Off();
	
	InitialiseUART();
	
	RGBLeds_PIO_Init_A2();

        SwitchOffLed();//Switch Off on startup
        
	UARTPrintf("Hello from my RGB Leds on Pin A2....\n\r");

	while (1)
	{
          TestColors();
          delay_1ms_Count(6000);
	}
}
