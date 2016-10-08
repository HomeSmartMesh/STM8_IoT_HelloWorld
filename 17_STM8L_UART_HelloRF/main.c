#include <iostm8l151f3.h>

#include "nRF_SPI.h"
//for nRF_SetMode_TX()
#include "nRF_Modes.h"

#include "nRF_Tx.h"

#include "ClockUartLed.h"

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

void PingColor()
{
      unsigned char Tx_Data[5];
      Tx_Data[0]=128;
      Tx_Data[1]=255;
      Tx_Data[2]=100;
      Tx_Data[3]=0x59;
      nRF_Transmit(Tx_Data,4);
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
  unsigned char index = 0;
    Initialise_STM8L_Clock();
    
    SYSCFG_RMPCR1_USART1TR_REMAP = 1; // Remap 01: USART1_TX on PA2 and USART1_RX on PA3
    InitialiseUART();//Tx only
    
    UARTPrintf("\r\n_________________________\n\r");
    UARTPrintf("Hello from STM8L RF Registers...\n\r");
    delay_1ms_Count(1000);

    //Applies the compile time configured parameters from nRF_Configuration.h
    nRF_Config();

    //The TX Mode is independently set from nRF_Config() and can be changed on run time
    nRF_SetMode_TX();
      
    
    Init_Magnet_PB0();
    Init_Magnet_PD0();
    //
    // Main loop
    //
    while (1)
    {
      //PingColor();
      PingUart(index++);
      LogMagnets();
      delay_1ms_Count(200);
      
    }
}
