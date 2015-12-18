
//Test Led is Port B Pin 5


#include "ClockUartLed.h"

//for direct SPI_Read_Register()
#include "nRF_SPI.h"

//for nRF_SetMode_TX()
#include "nRF_Modes.h"

#include "nRF_Tx.h"
#include "nRF_RegText.h"

#include "nRF_IRQ.h"

void Print_ConfigAndStatus_Registers()
{
	BYTE ConfigVal;
        UARTPrintf("  ");
	ConfigVal = SPI_Read_Register(CONFIG);
	UARTPrintf(" ; CONFIG: ");
	UARTPrintfHex(ConfigVal);
	UARTPrintf(" ; ");
	BYTE status = SPI_Read_Register(STATUS);		
	nRF_PrintStatus(status);
}


int main( void )
{
	
	BYTE counter = 0;

    InitialiseSystemClock();

    Initialise_TestLed_GPIO_B5();

    Test_Led_Off();

    InitialiseUART();
	
	UARTPrintf("\r\n_________________________\n\r");
	UARTPrintf("STM8 Receiver with (IRQ), Node 2\n\r");

	//Applies the compile time configured parameters from nRF_Configuration.h
	BYTE status = nRF_Config();

	//The RX Mode is independently set from nRF_Config() and can be changed on run time
	nRF_SetMode_RX();

    while (1)
    {

		counter++;
		UARTPrintf("loop Rx counter: ");
		UARTPrintfHex(counter);
		
		Print_ConfigAndStatus_Registers();
		
		Test_Led_Off();
		delay_1ms_Count(4900);
		
		Test_Led_On();
		delay_1ms_Count(100);
    }
}
