
//Test Led is Port B Pin 5

//  currently linked to http://creativecommons.org/licenses/by-sa/3.0/legalcode


#include <iostm8s103f3.h>
#include <intrinsics.h>

#include "ClockUartLed.h"
#include "commonTypes.h"
#include "i2c.h"


void i2c_user_Slave_Rx_Callback(BYTE *userdata,BYTE size)
{
	UARTPrintf("I2C Slave received :\n\r");
	UARTPrintfHexTable(userdata,size);
	UARTPrintf("\n\r");
}

void i2c_user_Slave_Tx_Callback(BYTE *userdata,BYTE size)
{
	UARTPrintf("I2C Slave Tx Complete :\n\r");
	UARTPrintfHexTable(userdata,size);
	UARTPrintf("\n\r");
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
		//UARTPrintf("[no Ack]\n\r");//this is ok for the slave
	}
	if(l_sr2 & 0x08)
	{
		UARTPrintf("[I2C Bus Overrun]\n\r");
	}
}


int main( void )
{
    BYTE userData[2];
    BYTE counter = 0;
    
    InitialiseSystemClock();

    InitialiseUART();
	
    UARTPrintf("\r\n_________________________\n\r");
    UARTPrintf("STM8 I2C Slave...\n\r");

	I2C_Init();//Start Listening as a slave
	
	I2C_Slave_Configure(0x72,userData,2);
	
	userData[0] = 0x55;
	
	__enable_interrupt();
	
	while (1)
	{
		//__wait_for_interrupt();
		counter++;
		userData[1] = counter;
		UARTPrintf("slave loop counter: ");
		UARTPrintfHexLn(counter);
		delay_1ms_Count(5000);
	}
}
