
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
	BYTE RxData[32];
	
	BYTE counter = 0;
	
    InitialiseSystemClock();

    Initialise_TestLed_GPIO_B5();

    InitialiseUART();
	
	//will wait 100 ms needed after Power On Reset, and intialise SPI
	//This call is optional, good for determinism, otherwise called by the library
	nRF_Init();

    Test_Led_Off();
    
    UARTPrintf("\r\n_________________________\n\r");
    UARTPrintf("STM8 Receiver, Node 2\n\r");

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
    
	nRF_SetMode_RX();
	UARTPrintf("SetMode_Rx: ");
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
			if(( status & (bit_RX_DR) ) != 0)
			{
				SPI_Read_Buf(RD_RX_PLOAD,RxData,1);
				//for(int i=0;i<32;i++)				{					UARTPrintfHex(RxData[i]);					UARTPrintf(" ");				}
				UARTPrintf("Received Data: ");UARTPrintfHex(RxData[0]);UARTPrintf("\n\r");
			}
			UARTPrintf("Clearing the status, ");
			nRF_ClearStatus();
			UARTPrintf("New ");
			status = SPI_Read_Register(STATUS);			nRF_PrintStatus(status);
			BYTE fifo_Status = SPI_Read_Register(FIFO_STATUS);
			UARTPrintf("FifoStatus: ");UARTPrintfHexLn(fifo_Status);
			if((fifo_Status & 0x01) == 0 )//data still in fifo
			{
				UARTPrintf("Data still in Fifo: ");UARTPrintfHexLn(fifo_Status);
				SPI_Read_Buf(RD_RX_PLOAD,RxData,1);
				UARTPrintf("Received more Data: ");UARTPrintfHex(RxData[0]);UARTPrintf("\n\r");
				nRF_ClearStatus();
			}
		}

		counter++;
		
		UARTPrintf("Rx counter loop: ");
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
		delay_1ms_Count(900);

		
		Test_Led_On();
		delay_1ms_Count(100);
    }
}
