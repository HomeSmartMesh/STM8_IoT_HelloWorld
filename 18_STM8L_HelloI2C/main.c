#include <iostm8l151f3.h>

#include "ClockUartLed.h"
#include "commonTypes.h"
#include "i2c.h"

#include <intrinsics.h>

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


void i2c_user_Rx_Callback(BYTE *userdata,BYTE size)
{
	UARTPrintf("I2C Transaction complete, received:\n\r");
	UARTPrintfHexTable(userdata,size);
	UARTPrintf("\n\r");
}

void i2c_user_Tx_Callback(BYTE *userdata,BYTE size)
{
	UARTPrintf("I2C Transaction complete, Transmitted:\n\r");
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
		UARTPrintf("[I2C no Acknowledge]\n\r");//this is ok for the slave
	}
	if(l_sr2 & 0x08)
	{
		UARTPrintf("[I2C Bus Overrun]\n\r");
	}
}


void SMT8L_Switch_ToHSI()
{
  CLK_SWCR_SWEN = 1;                  //  Enable switching.
  CLK_SWR = 0x01;                     //  Use HSI as the clock source.
  while (CLK_SWCR_SWBSY != 0);        //  Pause while the clock switch is busy.
}

void Initialise_STM8L_Clock()
{
  //Set Clock to full speed
  CLK_CKDIVR_CKM = 0; // Set to 0x00 => /1 ; Reset is 0x03 => /8
  //unsigned char cal = CLK_HSICALR-45;//default is 0x66 lowered by 45
  //Unlock the trimming
  /*CLK_HSIUNLCKR = 0xAC;
  CLK_HSIUNLCKR = 0x35;
  CLK_HSITRIMR = cal;
*/
  delay_1ms();
}
//

void Init_Magnet_PB0()
{
    PB_DDR_bit.DDR0 = 0;//  0: Input
    PB_CR1_bit.C10 = 0; //  0: Floating
}

void Init_Magnet_PD0()
{
    PD_DDR_bit.DDR0 = 0;//  0: Input
    PD_CR1_bit.C10 = 0; //  0: Floating
}


void Initialise_Test_GPIO_A2()
{
    PA_DDR_bit.DDR2 = 1;//  1: Output
    PA_CR1_bit.C12 = 1; //  1: Push-pull
}

void GPIO_B3_High()
{
    PB_ODR_bit.ODR3 = 1;
}

void GPIO_B3_Low()
{
    PB_ODR_bit.ODR3 = 0;
}

void PingUart(unsigned char index)
{
      UARTPrintf("Ping Color STM8L ");
      UARTPrintf_uint(index);
      UARTPrintf(" \n");
}

void LogMagnets()
{
      unsigned char Magnet_B0,Magnet_D0;
      Magnet_B0 = PB_IDR_IDR0;
      Magnet_D0 = PD_IDR_IDR0;
      UARTPrintf(" Magnets : LowVert D0 ");
      UARTPrintf_uint(Magnet_D0);
      UARTPrintf(" ; HighHor B0 ");
      UARTPrintf_uint(Magnet_B0);
      UARTPrintf("\n");
  
}

int main( void )
{
    BYTE userData[2];
    unsigned char index = 0;
    Initialise_STM8L_Clock();
    
    SYSCFG_RMPCR1_USART1TR_REMAP = 1; // Remap 01: USART1_TX on PA2 and USART1_RX on PA3
    InitialiseUART();//Tx only
    
    UARTPrintf("\r\n_________________________\n\r");
    UARTPrintf("Hello from STM8L I2C ...\n\r");
    delay_1ms_Count(1000);

	I2C_Init();
	
	
	__enable_interrupt();
    //
    // Main loop
    //
    BYTE FirmRevCmd[2];
    FirmRevCmd[0] = 0x84;
    FirmRevCmd[1] = 0xB8;
    while (1)
    {
      PingUart(index++);
      I2C_Write(0x40, FirmRevCmd,2);
      I2C_Read(0x40, userData,1);
      delay_1ms_Count(5000);
      
    }
}
