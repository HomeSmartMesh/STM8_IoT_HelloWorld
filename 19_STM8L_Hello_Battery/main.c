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
      //UARTPrintf(" \n");
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


void ADC_Init()
{
    //Enable ADC1 Peripheral Clock
    CLK_PCKENR2_PCKEN20 = 1;
    ADC1_CR2_PRESC = 1;//div clock by 2
    
    ADC1_CR1_ADON = 1;//wakes the ADC from power down, now it will take some time to stabilise
    //ADC1_CR3_SMTP2 = 6;// = 110 = 192 ADC clock cycles => with 16MHz 62,5ns => 12 us clk/2 => 24us
    ADC1_CR3_SMTP2 = 7;// 384 ADC clock cycles clk/2 
    
    ADC1_TRIGR1_TSON = 0;//we need to enable the Temperature sensor at the same time as the ADC ON
    ADC1_TRIGR1_VREFINTON = 1;//we need to enable the internal reference voltage as well
}

void ADC_Select_VTemp()
{
    ADC1_CR3_CHSEL = 29;// = 11101 = ADC channel TS is selected
}
void ADC_Select_VRef()
{
    ADC1_CR3_CHSEL = 28;// = 11100 = ADC channel VRefInt is selected
}

int ADC_Read_Temp()
{
    ADC1_CR3_CHSEL = 29;// = 11101 = ADC channel TS is selected
    ADC1_CR1_START = 1;
    while(ADC1_SR_EOC == 0);
    int res;
    res = ADC1_DRH_CONV_DATA;
    res<<=8;
    res |= ADC1_DRL_CONV_DATA;
    
    return res;
}

int ADC_Read_VRef()
{
    ADC1_CR3_CHSEL = 28;// = 11100 = ADC channel VRefInt is selected
    ADC1_CR1_START = 1;
    while(ADC1_SR_EOC == 0);
    int res;
    res = ADC1_DRH_CONV_DATA;
    res<<=8;
    res |= ADC1_DRL_CONV_DATA;
    
    return res;
}

void ADC_Off()
{
    ADC1_TRIGR1_TSON = 0;
    ADC1_TRIGR1_VREFINTON = 0;
  
    ADC1_CR1_ADON = 0;//Power down ADC
    //Disable ADC1 Peripheral Clock
    CLK_PCKENR2_PCKEN20 = 0;
}

void Measure_VRef_Bat_Temp()
{
  int Temperature;
  int VRef;
  Temperature = ADC_Read_Temp(); 
  VRef = ADC_Read_VRef();
  //ADC_Off();
  
  UARTPrintf(" Temperature ( ");
  UARTPrintf_uint(Temperature);
  UARTPrintf(" ) ");
  UARTPrintf("VRef ( ");
  UARTPrintf_uint(VRef);
  UARTPrintf(" )\n");
}

int main( void )
{
  unsigned char index = 0;
    Initialise_STM8L_Clock();
    
    SYSCFG_RMPCR1_USART1TR_REMAP = 1; // Remap 01: USART1_TX on PA2 and USART1_RX on PA3
    InitialiseUART();//Tx only
    
    delay_1ms_Count(200);
    UARTPrintf("\r\n_________________________\n\r");
    UARTPrintf("Hello from STM8L Battery Logger...\n\r");
    //Applies the compile time configured parameters from nRF_Configuration.h
    nRF_Config();

    //Init_Magnet_PB0();
    //Init_Magnet_PD0();
    
    ADC_Init();
    delay_1ms_Count(800);
    //
    // Main loop
    //
    while (1)
    {
      //PingColor();
      PingUart(index++);
      //LogMagnets();
      Measure_VRef_Bat_Temp();
      
      delay_1ms_Count(1000);
      
    }
}
