#include <iostm8s103f3.h>


#include "ClockUartLed.h"


//release is input
#define OneWire_Release(); PA_DDR_bit.DDR3 = 0;

//down is output, and low
#define OneWire_Down(); PA_DDR_bit.DDR3 = 1;PA_ODR_bit.ODR3 = 0;

#define OneWire_State() PA_IDR_bit.IDR3



#define OneWire_WriteOne();      OneWire_Down();delay_10us();OneWire_Release();delay_50us();
#define OneWire_WriteZero();     OneWire_Down();delay_10us();delay_50us();OneWire_Release();


#define DS18B20_ROM_SearchRom   0xF0
#define DS18B20_ROM_ReadRom     0x33
#define DS18B20_ROM_MatchRom    0x55
#define DS18B20_ROM_SkipRom     0xCC
#define DS18B20_ROM_AlarmSearch 0xEC

#define DS18B20_Func_Convert            0x44
#define DS18B20_Func_WriteScratchPad    0x4E
#define DS18B20_Func_ReadScratchPad     0xBE
#define DS18B20_Func_CopyScratchPad     0x48
#define DS18B20_Func_RecallE2           0xB8
#define DS18B20_Func_ReadPowerSupply    0xB4

//Write one is a pulse shorter than 15us


unsigned char DS18B20_ROM[8];

unsigned char DS18B20_ScratchPad[9];



void OneWire_Reset()
{
  OneWire_Down();
  for(int i=0;i<5;i++)//delay a bit more than 500 us 
  {
    delay_100us();
  }

  OneWire_Release();
}

unsigned char OneWire_CheckPresence()
{
  unsigned char res = 0;
  
  for(int i=0;i<10;i++)//delay a bit more than 100 us 
  {
    delay_10us();
  }

  if(OneWire_State() == 0)
  {
    res = 1;//present
  }
  else
  {
    res = 0;//not present
  }

  //wait 200 us  to have total 300 us = max of 60(response time) and 240(pulse time)
  for(int i=0;i<14;i++)//delay a bit more than 60 us 
  {
    delay_10us();
  }
  
  return res;
}

unsigned char OneWire_Init()
{
  OneWire_Reset();
  return OneWire_CheckPresence();
}


unsigned char OneWire_ReadBit()
{
  unsigned char res;

  OneWire_Down();
  delay_1us();delay_1us();//min 1us
  OneWire_Release();
  delay_10us();
  //must read within 15 us from edge low start
  
  if(OneWire_State() == 0)
  {
    res = 0;//Pull low for 0
  }
  else
  {
    res = 1;//up for 1
  }
  
  //delay 50us to reach more than 60 us bit time + 1 us inter bit time
  for(int i=0;i<5;i++)//delay a bit more than 100 us 
  {
    delay_10us();
  }
  
  return res;
}


void OneWire_WriteByte(unsigned char byte)
{
  for(int i=0;i<8;i++)
  {
    if(byte & 0x01)
    {
      OneWire_WriteOne();
    }
    else
    {
      OneWire_WriteZero();
    }
    byte>>=1;
  }
}

unsigned char OneWire_ReadByte()
{
  unsigned char res = 0;
  
  for(int i=0;i<7;i++)
  {
    if(OneWire_ReadBit())
    {
      res |= 0x80;
    }
    res >>=1;
  }
  // no shift on the last loop
  if(OneWire_ReadBit())
  {
    res |= 0x80;
  }
  return res;
}


unsigned char DS18B20_Single_ReadRom(unsigned char * rom)
{
  unsigned char res = 0;
  
  if(OneWire_Init())
  {
    OneWire_WriteByte(DS18B20_ROM_ReadRom);
    
    for(int i=0;i<8;i++)
    {
      rom[7-i] = OneWire_ReadByte();
      
    }
    
    res = 1;
  }
  
  return res;
}

unsigned char DS18B20_Single_ReadScratchPad(unsigned char * sp,unsigned char length)
{
  unsigned char res = 0;

  if(OneWire_Init())
  {
    OneWire_WriteByte(DS18B20_ROM_SkipRom);
    
    OneWire_WriteByte(DS18B20_Func_ReadScratchPad);

    for(int i=0;i<length;i++)
    {
        sp[i] = OneWire_ReadByte();
    }
    
    res = 1;
  }
  
  return res;
}

unsigned char DS18B20_Single_ReadTemperature(unsigned char * data)
{
  unsigned char res = 0;
  
  if(OneWire_Init())
  {
    OneWire_WriteByte(DS18B20_ROM_SkipRom);
    
    OneWire_WriteByte(DS18B20_Func_Convert);

    //keeps checking while the sensor is converting
    while(OneWire_ReadBit() == 0);
    
    
    if(DS18B20_Single_ReadScratchPad(data,2))
    {
      res = 1;
    }
    
  }
  
  
  return res;
}

void UARTPrint_DS18B20_Temperature(unsigned char * data)
{
  
  signed int trunc = data[1];
  
  trunc = trunc * 16;
  
  trunc = trunc + (data[0] >> 4);

  unsigned int frac = 0;
  if(data[0] & 0x01)
  {
    frac+=625;
  }
  if(data[0] & 0x02)
  {
    frac+=1250;
  }
  if(data[0] & 0x04)
  {
    frac+=2500;
  }
  if(data[0] & 0x08)
  {
    frac+=5000;
  }
  UARTPrintf_sint(trunc);
  UARTPrintf(".");
  UARTPrintf_uint(frac);
}


int main( void )
{
    InitialiseSystemClock();

    Initialise_TestLed_GPIO_B5();

    InitialiseUART();

    UARTPrintf("Hello from my Temperature DS18B20....\n\r");

    PB_DDR_bit.DDR5 = 1;
    PA_DDR_bit.DDR3 = 1;//output

    PB_CR1_bit.C15 = 0;
    PA_CR1_bit.C13 = 0;//open Drain
    
    //   0: Output speed up to  2 MHz
    //   1: Output speed up to 10 MHz
    //
    PA_CR2_bit.C23 = 1;
    //PB_CR2_bit.C25 = 0;

    
    unsigned char temperature_bytes[2];
    
    if(DS18B20_Single_ReadRom(DS18B20_ROM))
    {
      UARTPrintf("ROM = ");
      UARTPrintfHexTable(DS18B20_ROM,8);
      UARTPrintfLn("");
      
      
    }
    else
    {
      UARTPrintfLn("no sensor available");
    }
    
    
    Test_Led_Off();
    
    
    
    //
    // Main loop
    //

    while (1)
    {
      
      if(DS18B20_Single_ReadTemperature(temperature_bytes))
      {
        UARTPrintf("Temperature = ");
        UARTPrintfHex(temperature_bytes[1]);
        UARTPrintf(" ");
        UARTPrintfHex(temperature_bytes[0]);
        UARTPrintf(" ");
        UARTPrint_DS18B20_Temperature(temperature_bytes);
        UARTPrintfLn("");
      }
      
      
      delay(10000);
      
      Test_Led_Off();
      for(int i=0;i<10;i++)
      {
        delay(60000);
      }
      Test_Led_On();
      delay(10000);
    }
}
