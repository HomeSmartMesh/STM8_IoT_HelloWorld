
//Test Led is Port B Pin 5

//  currently linked to http://creativecommons.org/licenses/by-sa/3.0/legalcode

#include <iostm8s103f3.h>
#include <intrinsics.h>

#include "ClockUartLed.h"
#include "commonTypes.h"
#include "i2c_m.h"

void i2c_user_Rx_Callback(BYTE *userdata,BYTE size)
{
	/*UARTPrintf("I2C Transaction complete, received:\n\r");
	UARTPrintfHexTable(userdata,size);
	UARTPrintf("\n\r");
        */
}

void i2c_user_Tx_Callback(BYTE *userdata,BYTE size)
{
	/*UARTPrintf("I2C Transaction complete, Transmitted:\n\r");
	UARTPrintfHexTable(userdata,size);
	UARTPrintf("\n\r");
        */
}

void i2c_user_Error_Callback(BYTE l_sr2)
{
	if(l_sr2 & 0x01)
	{
		UARTPrintf("[I2C Bus Error]\n\r");
	}
	if(l_sr2 & 0x02)
	{
		UARTPrintf("[I2C Arbitration Lost]\n\r");
	}
	if(l_sr2 & 0x04)
	{
		UARTPrintf("[I2C no Acknowledge]\n\r");//this is ok for the slave
	}
	if(l_sr2 & 0x08)
	{
		UARTPrintf("[I2C Bus Overrun]\n\r");
	}
}


int main( void )
{
    BYTE userData[2];
    BYTE sensorData[2];
    BYTE counter = 0;
    
    sensorData[0] = 0xFF;
    sensorData[0] = 0xFF;
    
    InitialiseSystemClock();

    InitialiseUART();
	
    UARTPrintf("\r\n_________________________\n\r");
    UARTPrintf("STM8 I2C Ambient Light BH1750\n\r");

	I2C_Init();
	
	
	__enable_interrupt();
	
	while (1)
	{
		//__wait_for_interrupt();
                userData[0] = 0x10;//Continuously H Resolution mode
		I2C_Write(0x23, userData,1);//Slave Address 0100-0110; 
		
                //UARTPrintf("Written Command H Res:\r\n");
		delay_1ms_Count(200);//max 180ms wait for read to complete

                I2C_Read(0x23, sensorData,2);//Slave Address 0100-0110; 
		UARTPrintf("Read: ");
		UARTPrintfHexTable(sensorData,2);
		UARTPrintf("\n");
		delay_1ms_Count(200);//max 180ms wait for read to complete

	}
}
