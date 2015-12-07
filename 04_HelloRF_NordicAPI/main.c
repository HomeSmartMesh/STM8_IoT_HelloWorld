
//Test Led is Port B Pin 5


#include "ClockUartLed.h"

#include "nRF_SPI.h"

int main( void )
{
    InitialiseSystemClock();

    Initialise_TestLed_GPIO_B5();

    InitialiseUART();
     
    SPI_Init();
    
    UARTPrintf("\r\n_________________________\n\r");
    UARTPrintf("Hello from nRF STM8....\n\r");
    
    Test_Led_Off();

      for(int i=0;i<nRF24L01pRegNumber;i++)
      {
          UARTPrintf("reg ");
          UARTPrintfHex((unsigned char)i);
          UARTPrintf(" ");
          UARTPrintf(nRF24L01pRegNames[i]);
          BYTE reg;
          reg = SPI_Read(i);
          UARTPrintf(" : ");
          UARTPrintfHexLn(reg);
      }

    UARTPrintf("\r\n------------------------------\n\r");
    
    while (1)
    {
      Test_Led_Off();
	  
      for(int i=0;i<10;i++)
      {
        delay(60000);
      }
	  
      Test_Led_On();
	  
      delay(10000);
    }
}
