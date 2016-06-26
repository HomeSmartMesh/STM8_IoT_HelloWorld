
//Test Led is Port B Pin 5

#include <iostm8s103f3.h>
#include <intrinsics.h>

#include "ClockUartLed.h"

unsigned char rxIndex;
unsigned char rxTable[10];
unsigned char cmd;
unsigned char interruptJustHappened;
#define RX_FRAME_SIZE 4


void Initialise_ULN_Outputs()
{
  PD_DDR_bit.DDR1 = 1;// 1: Output : Rx Pin
  PD_CR1_bit.C11 = 1;//   0: Push Pull
      
  PD_DDR_bit.DDR2 = 1;// 1: Output : Rx Pin
  PD_CR1_bit.C12 = 1;//   0: Push Pull

  PD_DDR_bit.DDR3 = 1;// 1: Output : Rx Pin
  PD_CR1_bit.C13 = 1;//   0: Push Pull

  PC_DDR_bit.DDR3 = 1;// 1: Output : Rx Pin
  PC_CR1_bit.C13 = 1;//   0: Push Pull

  PC_DDR_bit.DDR4 = 1;// 1: Output : Rx Pin
  PC_CR1_bit.C14 = 1;//   0: Push Pull

  PC_DDR_bit.DDR5 = 1;// 1: Output : Rx Pin
  PC_CR1_bit.C15 = 1;//   0: Push Pull

  PC_DDR_bit.DDR6 = 1;// 1: Output : Rx Pin
  PC_CR1_bit.C16 = 1;//   0: Push Pull

  PC_DDR_bit.DDR7 = 1;// 1: Output : Rx Pin
  PC_CR1_bit.C17 = 1;//   0: Push Pull

}


void Reset_ULN_Output()
{
  PD_ODR_bit.ODR1 = 0;
  PD_ODR_bit.ODR2 = 0;
  PD_ODR_bit.ODR3 = 0;

  PC_ODR_bit.ODR3 = 0;
  PC_ODR_bit.ODR4 = 0;
  PC_ODR_bit.ODR5 = 0;
  PC_ODR_bit.ODR6 = 0;
  PC_ODR_bit.ODR7 = 0;
}

void Apply_Cmd_To_ULN_Output()
{
  PD_ODR_bit.ODR1 = (cmd>>5)&0x01;
  PD_ODR_bit.ODR2 = (cmd>>6)&0x01;
  PD_ODR_bit.ODR3 = (cmd>>7)&0x01;

  PC_ODR_bit.ODR3 = (cmd>>0)&0x01;
  PC_ODR_bit.ODR4 = (cmd>>1)&0x01;
  PC_ODR_bit.ODR5 = (cmd>>2)&0x01;
  PC_ODR_bit.ODR6 = (cmd>>3)&0x01;
  PC_ODR_bit.ODR7 = (cmd>>4)&0x01;
}


void check_Rx_Frame()
{
  if(rxTable[0] == 'G')
    if(rxTable[1] == 'o')
    {
      if((rxTable[2] ^ rxTable[3]) == 0xFF)
      {
        cmd = rxTable[2];
        Apply_Cmd_To_ULN_Output();
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

    Initialise_TestLed_GPIO_B5();
    
    Initialise_ULN_Outputs();
    
    Reset_ULN_Output();
    
    InitialiseUART();
    
    UARTPrintf("\r\n_________________________\n\r");
    UARTPrintf("STM8 UL IO - Hello\n\r");

    
    
    
    //unsigned char status = UART1_SR;//to clear error flags
    //unsigned char rx = UART1_DR;

    UART1_CR2_RIEN = 1;     // Enable UART Rx Interrupts
    __enable_interrupt();

 while (1)
    {
      if(interruptJustHappened == 1)//resetting the state machine after two loops
      {
        interruptJustHappened = 0;
      }
      else
      {
        if(rxIndex!=0)
        {
          rxIndex = 0;
          UARTPrintf("State Cleared\r\n");
        }
      }
      UARTPrintf("Ping ULN ");
      UARTPrintf_uint(index++);
      UARTPrintf(" \n");
      Test_Led_Off();
      delay_1ms_Count(2900);


      Test_Led_On();
      delay_1ms_Count(100);
    }
}
