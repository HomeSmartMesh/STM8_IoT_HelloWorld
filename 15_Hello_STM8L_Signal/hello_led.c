#include <iostm8l151f3.h>

// Delay loop
//
// Actual delay depends on clock settings
// and compiler optimization settings.
//
void delay(unsigned int n)
{
    while (n-- > 0);
}

#define delay_1ms(); delay(1998);

#define Test_Led_PA2 PA_ODR_ODR2

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
  unsigned char cal = CLK_HSICALR-45;//default is 0x66 lowered by 45
  //Unlock the trimming
  CLK_HSIUNLCKR = 0xAC;
  CLK_HSIUNLCKR = 0x35;
  CLK_HSITRIMR = cal;

  delay_1ms();
}
//

void Initialise_Test_GPIO_B3()
{
    // Data Direction Register
    //
    // 0: Input
    // 1: Output
    //
    PB_DDR_bit.DDR3 = 1;

    // Control Register 1
    // Output mode:
    //   0: Pseudo open drain
    //   1: Push-pull
    //
    PB_CR1_bit.C13 = 1;
    
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


int main( void )
{
    Initialise_STM8L_Clock();

    Initialise_Test_GPIO_B3();

    delay(10);
    
    GPIO_B3_High();
    
    delay(10);
    
    Initialise_Test_GPIO_A2();
    PA_ODR_bit.ODR2 = 1;
    delay(10);
    PA_ODR_bit.ODR2 = 0;
    delay(10);
      
    //
    // Main loop
    //
    while (1)
    {
      PB_ODR_bit.ODR3 = 0;
      delay_1ms();
      PB_ODR_bit.ODR3 = 1;
      delay_1ms();
    }
}
