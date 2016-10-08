
//Test Led is Port B Pin 5

//  currently linked to http://creativecommons.org/licenses/by-sa/3.0/legalcode

#include "ClockUartLed.h"
#include "commonTypes.h"

#include <iostm8s103f3.h>
#include <intrinsics.h>

void InitialiseSystemClock_LSI()
{
	CLK_ICKR = 0;                       //  Reset the Internal Clock Register.
        CLK_ICKR_REGAH = 1;     // -- 1:MVR regulator OFF in Active-Halt Mode (Longer wakeup / lower sleep power)
	CLK_ICKR_LSIEN = 1;                 //  Enable the LSI.
	CLK_ECKR = 0;                       //  Disable the external clock.
	while (CLK_ICKR_LSIRDY == 0);       //  Wait for the LSI to be ready for use.
	//CLK_CKDIVR = 0;                     //  Ensure the clocks are running at full speed.
	CLK_PCKENR1 = 0xFF;                 //  Enable all peripheral clocks.
	CLK_PCKENR2 = 0xFF;                 //  Ditto.
	CLK_CCOR = 0;                       //  Turn off CCO.
	CLK_HSITRIMR = 0;                   //  Turn off any HSIU trimming.
	CLK_SWIMCCR = 0;                    //  Set SWIM to run at clock / 2.
	
        //Clock Switching
        CLK_SWR = 0xD2;                     //  0xE1:HSI - 0xD2:LSI
	CLK_SWCR = 0;                       //  Reset the clock switch control register.
	CLK_SWCR_SWEN = 1;                  //  Enable switching.
	while (CLK_SWCR_SWBSY != 0);        //  Pause while the clock switch is busy.
}

void InitialiseSystemClock_HSI()
{
	CLK_ICKR = 0;                       //  Reset the Internal Clock Register.
        CLK_ICKR_REGAH = 1;     // -- 1:MVR regulator OFF in Active-Halt Mode (Longer wakeup / lower sleep power)
	CLK_ICKR_HSIEN = 1;                 //  Enable the HSI.
	CLK_ECKR = 0;                       //  Disable the external clock.
	while (CLK_ICKR_HSIRDY == 0);       //  Wait for the HSI to be ready for use.
	CLK_CKDIVR = 0;                     //  Ensure the clocks are running at full speed.
	//CLK_CKDIVR = 0x1F;                     //  Slowest clock speed
	CLK_PCKENR1 = 0x00;                 //  Disable all peripheral clocks.
	CLK_PCKENR2 = 0x04;                 //  0x04 Disable all peripheral clocks. - Enable AWU
	CLK_CCOR = 0;                       //  Turn off CCO.
	CLK_HSITRIMR = 0;                   //  Turn off any HSIU trimming.
	CLK_SWIMCCR = 0;                    //  Set SWIM to run at clock / 2.
	CLK_SWR = 0xe1;                     //  Use HSI as the clock source.
	CLK_SWCR = 0;                       //  Reset the clock switch control register.
	CLK_SWCR_SWEN = 1;                  //  Enable switching.
	while (CLK_SWCR_SWBSY != 0);        //  Pause while the clock switch is busy.
        
        FLASH_CR1_HALT = 1;    //Switch the Flash to power down in Active halt state, increse wakeup by few us
        FLASH_CR1_AHALT = 1;    //Switch the Flash to power down in Active halt state, increse wakeup by few us
}

void InitialiseAWU()
{
  AWU_CSR1_AWUEN = 0;     // Disable the Auto-wakeup feature.
  AWU_APR_APR = 11;             //0x0F 11 -> 6.3 sec
  AWU_TBR_AWUTB = 0x0F;// was 1
  AWU_CSR1_AWUEN = 1;     // Enable the Auto-wakeup feature.
}

BYTE LedState = 0;

#pragma vector = AWU_vector
__interrupt void AWU_IRQHandler(void)
{
  volatile unsigned char reg;
  
  if(LedState == 0)
  {
    Test_Led_On();
    LedState = 1;  
  }
  else
  {
    
    LedState++;
    if(LedState == 2)
    {
      Test_Led_Off();  
      LedState = 0;
    }
  }
  
  reg = AWU_CSR1;     // Reading AWU_CSR1 register clears the interrupt flag.
}

int main( void )
{
    
    __disable_interrupt();
    
    InitialiseSystemClock_HSI();

    Initialise_TestLed_GPIO_B5();
    
    InitialiseAWU();
    
    Test_Led_Off();

    Test_Led_On();
    delay_1ms_Count(500);//16MHz
    //delay_1ms_Count(1);//slowest
    Test_Led_Off();
    
    __enable_interrupt();
    
    while (1)
    {
      __halt();
    }
}
