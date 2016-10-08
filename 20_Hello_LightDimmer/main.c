
//Test Led is Port B Pin 5

#include <iostm8s103f3.h>
#include <intrinsics.h>

#include "ClockUartLed.h"

unsigned char rxIndex;
unsigned char rxTable[10];
unsigned char cmd;
unsigned char interruptJustHappened;
#define RX_FRAME_SIZE 4

#define PowerControl_PC5 PC_ODR_ODR5

void PowerControl_On()
{
  PC_DDR_DDR5 = 1;//1:output
  PowerControl_PC5 = 0;
}
void PowerControl_Off()
{
  PowerControl_PC5 = 1;
  PC_DDR_DDR5 = 0;//0:input
}

void Initialise_PowerControl_C5()
{
  PowerControl_Off();
  PC_DDR_DDR5 = 1;//1:output
  PC_CR1_C15 = 0;//   0: Pseudo open drain
}

void Initialise_Analog_D2()
{
  PD_DDR_DDR2 = 0;//0:input
  PD_CR1_C12 = 0;//   0: Floating input
}

void check_Rx_Frame()
{
  if(rxTable[0] == 'G')
    if(rxTable[1] == 'o')
    {
      if((rxTable[2] ^ rxTable[3]) == 0xFF)
      {
        cmd = rxTable[2];
        //Apply_Cmd_To_ULN_Output();
        UARTPrintf("Ack Rx ");
        UARTPrintfHex(rxTable[2]);
        UARTPrintf(" \n");
      }
    }
}

#pragma vector = UART1_R_RXNE_vector
__interrupt void IRQHandler_UART(void)
{
  unsigned char rx;
  while(UART1_SR_RXNE)
  {
    rx = UART1_DR;
    if(rxIndex<10)
    {
      rxTable[rxIndex++] = rx;
    }
    if(rxIndex == RX_FRAME_SIZE)
    {
      check_Rx_Frame();
      rxIndex = 0;
    }
  }
  if((UART1_SR&0x0F) != 0x00)//Any of the errors
  {
    UART1_SR = 0;
    rx = UART1_DR;//read DR to clear the errors
  }
  interruptJustHappened = 1;
}

int main( void )
{
    rxIndex = 0;
    unsigned char index = 0;
    interruptJustHappened = 0;
    InitialiseSystemClock();

    Initialise_Analog_D2();
    Initialise_PowerControl_C5();
    Initialise_TestLed_GPIO_B5();
    
    
    InitialiseUART();
    
    UARTPrintf("\r\n_________________________\n\r");
    UARTPrintf("STM8 Light Dimmer...\n\r");

    

 while (1)
    {
      UARTPrintf("Ping Dimmer");
      UARTPrintf_uint(index++);
      UARTPrintf(" \n");
      Test_Led_Off();
      //PowerControl_Off();
      delay_1ms_Count(3000);


      Test_Led_On();
      PowerControl_On();
      delay_1ms_Count(1000);
    }
}
