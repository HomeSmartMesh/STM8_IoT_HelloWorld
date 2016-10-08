
//Test Led is Port B Pin 5


#include "ClockUartLed.h"
#include "commonTypes.h"

int main( void )
{
	
    BYTE counter = 0;

    InitialiseSystemClock();

    Initialise_TestLed_GPIO_B5();

    Test_Led_Off();

    InitialiseUART();
	
    UARTPrintf("\r\n_________________________\n\r");
    UARTPrintf("STM8 ADC...\n\r");

    while (1)
    {

		counter++;
		UARTPrintf("loop Rx counter: ");
		UARTPrintfHex(counter);
		
		Test_Led_Off();
		delay_1ms_Count(400);
		
		Test_Led_On();
		delay_1ms_Count(100);
    }
}
