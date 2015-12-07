#include <iostm8s103f3.h>

void InitialiseUART()
{
	//
	//  Clear the Idle Line Detected bit in the status register by a read
	//  to the UART1_SR register followed by a Read to the UART1_DR register.
	//
	unsigned char tmp = UART1_SR;
	tmp = UART1_DR;
	//
	//  Reset the UART registers to the reset values.
	//
	UART1_CR1 = 0;
	UART1_CR2 = 0;
	UART1_CR4 = 0;
	UART1_CR3 = 0;
	UART1_CR5 = 0;
	UART1_GTR = 0;
	UART1_PSCR = 0;
	//
	//  Now setup the port to 115200,n,8,1.
	//
	UART1_CR1_M = 0;        //  8 Data bits.
	UART1_CR1_PCEN = 0;     //  Disable parity.
	UART1_CR3_STOP = 0;     //  1 stop bit.
	UART1_BRR2 = 0x0a;      //  Set the baud rate registers to 115200 baud
	UART1_BRR1 = 0x08;      //  based upon a 16 MHz system clock.
	//
	//  Disable the transmitter and receiver.
	//
	UART1_CR2_TEN = 0;      //  Disable transmit.
	UART1_CR2_REN = 0;      //  Disable receive.
	//
	//  Set the clock polarity, lock phase and last bit clock pulse.
	//
	UART1_CR3_CPOL = 1;
	UART1_CR3_CPHA = 1;
	UART1_CR3_LBCL = 1;
	//
	//  Turn on the UART transmit, receive and the UART clock.
	//
	UART1_CR2_TEN = 1;
	UART1_CR2_REN = 1;
	UART1_CR3_CKEN = 1;
}

//
//  Send a message to the debug port (UART1).
//
void UARTPrintf(char *message)
{
	char *ch = message;
	while (*ch)
	{
	UART1_DR = (unsigned char) *ch;     //  Put the next character into the data transmission register.
	while (UART1_SR_TXE == 0);          //  Wait for transmission to complete.
	ch++;                               //  Grab the next character.
	}
}

void InitialiseSystemClock()
{
	CLK_ICKR = 0;                       //  Reset the Internal Clock Register.
	CLK_ICKR_HSIEN = 1;                 //  Enable the HSI.
	CLK_ECKR = 0;                       //  Disable the external clock.
	while (CLK_ICKR_HSIRDY == 0);       //  Wait for the HSI to be ready for use.
	CLK_CKDIVR = 0;                     //  Ensure the clocks are running at full speed.
	CLK_PCKENR1 = 0xff;                 //  Enable all peripheral clocks.
	CLK_PCKENR2 = 0xff;                 //  Ditto.
	CLK_CCOR = 0;                       //  Turn off CCO.
	CLK_HSITRIMR = 0;                   //  Turn off any HSIU trimming.
	CLK_SWIMCCR = 0;                    //  Set SWIM to run at clock / 2.
	CLK_SWR = 0xe1;                     //  Use HSI as the clock source.
	CLK_SWCR = 0;                       //  Reset the clock switch control register.
	CLK_SWCR_SWEN = 1;                  //  Enable switching.
	while (CLK_SWCR_SWBSY != 0);        //  Pause while the clock switch is busy.
}
//
// Delay loop
//
// Actual delay depends on clock settings
// and compiler optimization settings.
//
void delay(unsigned int n)
{
    while (n-- > 0);
}

void RGB_One()//with the ret and jmp => 1.25us
{
  PA_ODR_bit.ODR3 = 1;
  //delay_800ns(); nCount++; is 3 instructions => 187,5
  asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
  asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
  asm("nop");asm("nop");//(12+1 set) x 62.5 = 812.5ns
  PA_ODR_bit.ODR3 = 0;
  //delay_450ns();
  asm("nop");asm("nop");asm("nop");asm("nop");//(4+3 ret,jmp,set)x62.5 = 437.5ns
}

void RGB_Zero()//with the ret and jmp => 1.25us
{
  PA_ODR_bit.ODR3 = 1;
  //delay_400ns();
  asm("nop");asm("nop");asm("nop");
  asm("nop");asm("nop");//(5+1 set) x 62.5 = 375ns
  PA_ODR_bit.ODR3 = 0;
  //delay_850ns();
  asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
  asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
  asm("nop");//(11 + 3 ret,jmp,set) x 62.5 = 875ns
}

void LedDataOut_000()
{
  RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();
  RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();
  RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();
}
void LedDataOut_001()
{
  RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();
  RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();
  RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();
}
void LedDataOut_010()
{
  RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();
  RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();
  RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();
}
void LedDataOut_100()
{
  RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();
  RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();
  RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();
}
void LedDataOut_011()
{
  RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();
  RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();
  RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();
}
void LedDataOut_110()
{
  RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();
  RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();
  RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();
}
void LedDataOut_101()
{
  RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();
  RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();RGB_Zero();
  RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();
}
void LedDataOut_111()
{
  RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();
  RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();
  RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();RGB_One();
}

int main( void )
{
    InitialiseSystemClock();

   InitialiseUART();

    UARTPrintf("Hello from my RGB Leds....\n\r");

    PB_DDR_bit.DDR5 = 1;
    PA_DDR_bit.DDR3 = 1;//output

    PB_CR1_bit.C15 = 0;
    PA_CR1_bit.C13 = 0;//open Drain
    
    //   0: Output speed up to  2 MHz
    //   1: Output speed up to 10 MHz
    //
    //PB_CR2_bit.C25 = 0;
    
    //
    // Output Data Register
    //
    // Output value
    //
    PB_ODR_bit.ODR5 = 1;
    //PC_ODR =0xFF;
    
    //
    // Main loop
    //
    while (1)
    {
      LedDataOut_000();
      LedDataOut_001();
      LedDataOut_010();
      LedDataOut_100();
      LedDataOut_011();
      LedDataOut_110();
      LedDataOut_101();
      LedDataOut_111();
      delay(60000);
      //Off
      /*PB_ODR_bit.ODR5 = 1;
      for(int i=0;i<10;i++)
      {
        delay(60000);
      }
      //On
      PB_ODR_bit.ODR5 = 0;
      delay(10000);*/
    }
}
