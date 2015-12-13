
//Test Led is Port B Pin 5


#include "ClockUartLed.h"

//for direct SPI_Read_Register()
#include "nRF_SPI.h"

//for nRF_SetMode_TX()
#include "nRF_Modes.h"

#include "nRF_Tx.h"
#include "nRF_RegText.h"

int main( void )
{
    BYTE TxData[32];
    BYTE counter = 0;
	
    InitialiseSystemClock();

    Initialise_TestLed_GPIO_B5();

    InitialiseUART();
	
	//will wait 100 ms needed after Power On Reset, and intialise SPI
	//This call is optional, good for determinism, otherwise called by the library
	nRF_Init();

    Test_Led_Off();
    
    UARTPrintf("\r\n_________________________\n\r");
    UARTPrintf("STM8 Transmitter, Node 1\n\r");

	//This call is optional, good for determinism, otherwise called by the library
	BYTE status = nRF_Config();
	UARTPrintf("Config updated with : ");
	UARTPrintfHex(ConfigVal);
	UARTPrintf(" ; reread ");
        ConfigVal = SPI_Read_Register(CONFIG);
	UARTPrintfHex(ConfigVal);
	UARTPrintf(" ; ");
	nRF_PrintStatus(status);
	
	if(		( status & (bit_RX_DR | bit_TX_DS | bit_MAX_RT) ) != 0	)
	{
		UARTPrintf("Clearing the status, ");
		nRF_ClearStatus();
		UARTPrintf("New ");
        status = SPI_Read_Register(STATUS);		nRF_PrintStatus(status);
	}
    
	nRF_SetMode_TX();
	UARTPrintf("SetMode_Tx: ");
	UARTPrintf("Config updated with : ");
	UARTPrintfHex(ConfigVal);
	UARTPrintf(" ; reread ");
        ConfigVal = SPI_Read_Register(CONFIG);
	UARTPrintfHex(ConfigVal);
	UARTPrintf(" ; ");
	status = SPI_Read_Register(STATUS);			nRF_PrintStatus(status);

    while (1)
    {
		if(	( status & (bit_RX_DR | bit_TX_DS | bit_MAX_RT) ) != 0	)
		{
			UARTPrintf("Clearing the status, ");
			nRF_ClearStatus();
			UARTPrintf("New ");
			status = SPI_Read_Register(STATUS);			nRF_PrintStatus(status);
		}

		counter++;
		TxData[0] = counter;
		//for(int i=0;i<32;i++)		{		  TxData[i] = counter;		}
		
		status = nRF_Transmit(TxData,1);
		UARTPrintf("Transmitting: ");
		UARTPrintfHex(counter);
		ConfigVal = SPI_Read_Register(CONFIG);
		UARTPrintf(" ; CONFIG: ");
		UARTPrintfHex(ConfigVal);
		UARTPrintf(" ; ");
		status = SPI_Read_Register(STATUS);		nRF_PrintStatus(status);

#ifdef DumpRegisters
                for(int i=0;i<nRF24L01pRegNumber;i++)
                {
                    UARTPrintf("reg ");
                    UARTPrintfHex((unsigned char)i);
                    UARTPrintf(" ");
                    UARTPrintf(nRF24L01pRegNames[i]);
                    BYTE reg;
                    reg = SPI_Read_Register(i);
                    UARTPrintf(" : ");
                    UARTPrintfHexLn(reg);
                }
#endif
                
		Test_Led_Off();
		delay_1ms_Count(4900);

		
		Test_Led_On();
		delay_1ms_Count(100);
    }
}
