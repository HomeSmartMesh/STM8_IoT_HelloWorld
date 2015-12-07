#include <iostm8s103f3.h>

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

#define Test_Led_PB5 PB_ODR_ODR5

void Initialise_TestLed_GPIO_B5()
{
      //D3 output
     //PD_DDR_bit.DDR3 = 1;
     //push pull
    //PD_CR1_bit.C13 = 1;
    //PD_ODR_bit.ODR3 = 1;

    //
    // Data Direction Register
    //
    // 0: Input
    // 1: Output
    //
    PB_DDR_bit.DDR5 = 1;
    //PC_DDR = 0xFF;
    
    //
    // Control Register 1
    //
    // Input mode:
    //   0: Floating input
    //   1: Input with pull-up
    //
    // Output mode:
    //   0: Pseudo open drain
    //   1: Push-pull
    //
    PB_CR1_bit.C15 = 0;
    //PC_CR1 = 0x00;
    
    //
    // Control Register 2
    //
    // Input mode:
    //   0: External interrupt disabled
    //   1: External interrupt enabled
    //
    // Output mode:
    //   0: Output speed up to  2 MHz
    //   1: Output speed up to 10 MHz
    //
    //PB_CR2_bit.C25 = 0;
}

int main( void )
{
    InitialiseSystemClock();

    Initialise_TestLed_GPIO_B5();

    //Led Off
    Test_Led_PB5 = 1;
    
    //
    // Main loop
    //
    while (1)
    {
      //Off
      Test_Led_PB5 = 1;
      for(int i=0;i<10;i++)
      {
        delay(60000);
      }
      //On
      Test_Led_PB5 = 0;
      delay(10000);
    }
}
