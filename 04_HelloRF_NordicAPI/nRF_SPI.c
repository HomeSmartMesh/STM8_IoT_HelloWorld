/** @file nRF_SPI.h
 *
 * @author Wassim FILALI taken over from  Runar Kjellhaug out of nRF24L01P-EK
 *
 * @compiler IAR STM8
 *
 *
 * $Date: 20.09.2015
 * $Revision:
 *
 */

#include "nRF_SPI.h"
#include "nRF_LowLevel.h"


const unsigned char nRF24L01pRegNumber = 0x18;

const char* nRF24L01pRegNames[] = { "CONFIG",
                                    "EN_AA",
                                    "EN_RXADDR",
                                    "SETUP_AW",
                                    "SETUP_RETR",
                                    "RF_CH",
                                    "RF_SETUP",
                                    "STATUS",
                                    "OBSERVE_TX",
                                    "CD",
                                    "RX_ADDR_P0",
                                    "RX_ADDR_P1",
                                    "RX_ADDR_P2",
                                    "RX_ADDR_P3",
                                    "RX_ADDR_P4",
                                    "RX_ADDR_P5",
                                    "TX_ADDR ",
                                    "RX_PW_P0",
                                    "RX_PW_P1",
                                    "RX_PW_P2",
                                    "RX_PW_P3",
                                    "RX_PW_P4",
                                    "RX_PW_P5",
                                    "FIFO_STATUS"};

//**********************************************************//
void SPI_Init()
{
	SPI_Init_ChipSelect_Pin();//Starts Disabled
	
	SPI_Init_ChipEnable_Pin();//Starts Disabled
	
	SPI_Init_Peripheral();
	
}


//**********************************************************//
//
//  Function: SPI_RW
//
//  Description:
//  Writes one byte to nRF24L01, and return the byte read
//  from nRF24L01 during write, according to SPI protocol
//
//  In/Out parameters:
//  In: 'byte', current byte to be written
//  Out: 'SPI0DAT', HW SPI mode, 'byte' SW SPI mode,
//
//  Author: RSK   Date: 28.11.05
//**********************************************************//
BYTE SPI_RW(BYTE byte)
{
    return(SPI_WriteRead(byte));                         // Perform HW SPI operation
}


//**********************************************************//
//
//  Function: SPI_Read
//
//  Description:
//  Read one byte from nRF24L01 register, 'reg'
//
//
//  In/Out parameters:
//  In: reg, register to read
//  Out: return reg_val, register value.
//
//
//  Author: RSK   Date: 28.11.05
//**********************************************************//
/*
  uint8_t result;
  //Chip select
  GPIO_WriteLow(GPIOC, GPIO_PIN_4);
  
  //Send address and read command
  while (SPI_GetFlagStatus(SPI_FLAG_TXE)== RESET);
  SPI_SendData(RF24L01_command_R_REGISTER | register_addr);
  while (SPI_GetFlagStatus(SPI_FLAG_BSY)== SET);
  while (SPI_GetFlagStatus(SPI_FLAG_RXNE)== RESET);
  SPI_ReceiveData();
  
  //Get data
  while (SPI_GetFlagStatus(SPI_FLAG_TXE)== RESET);
  SPI_SendData(0x00);
  while (SPI_GetFlagStatus(SPI_FLAG_BSY)== SET);
  while (SPI_GetFlagStatus(SPI_FLAG_RXNE) == RESET);
  result = SPI_ReceiveData();
  
  //Chip select
  GPIO_WriteHigh(GPIOC, GPIO_PIN_4);
  
  return result;

*/
BYTE SPI_Read(BYTE reg)
{
	BYTE reg_val;

	// CSN low, initialize SPI communication...
	CSN_Pin_LowSelect();
	SPI_RW(reg);
	reg_val = SPI_RW(0);     // ..then read registervalue
	CSN_Pin_HighDisable();   // CSN high, terminate SPI communication

	return(reg_val);         // return register value
}


//**********************************************************//
//
//  Function: SPI_RW_Reg
//
//  Description:
//  Writes value 'value' to register 'reg'
//
//
//  In/Out parameters:
//  In: 'reg' register to write value 'value' to.
//  Return status byte.
//
//  Author: RSK   Date: 28.11.05
//**********************************************************//
BYTE SPI_RW_Reg(BYTE reg, BYTE value)
{
BYTE status;

	CSN_Pin_LowSelect();	// CSN low, init SPI transaction
	status = SPI_RW(reg);   // select register
	SPI_RW(value);          // ..and write value to it..
	CSN_Pin_HighDisable();	// CSN high again

  return(status);           // return nRF24L01 status byte
}


//**********************************************************//
//
//  Function: SPI_Write_Buf
//
//  Description:
//  Writes contents of buffer '*pBuf' to nRF24L01
//  Typically used to write TX payload, Rx/Tx address
//
//
//  In/Out parameters:
//  In: register 'reg' to write, buffer '*pBuf*' contains
//  data to be written and buffer size 'buf_size' is #of
//  bytes to be written
//  Out: return nRF24L01 status byte.
//
//  Author: RSK   Date: 28.11.05
//**********************************************************//
BYTE SPI_Write_Buf(BYTE reg, BYTE *pBuf, BYTE bytes)
{
BYTE status,byte_ctr;

  CSN_Pin_LowSelect();                                     // Set CSN low, init SPI tranaction
  status = SPI_RW(reg);                           // Select register to write to and read status byte

  for(byte_ctr=0; byte_ctr<bytes; byte_ctr++)     // then write all byte in buffer(*pBuf)
    SPI_RW(*pBuf++);

  CSN_Pin_HighDisable();                                     // Set CSN high again

  return(status);                                 // return nRF24L01 status byte
}


//**********************************************************//
//
//  Function: SPI_Read_Buf
//
//  Description:
//  Reads 'bytes' #of bytes from register 'reg'
//  Typically used to read RX payload, Rx/Tx address
//
//
//  In/Out parameters:
//  In: 'reg', register to read from, '*pBuf' are buffer
//  the read bytes are stored to and 'bytes' are #of bytes
//  to read.
//  Out: return nRF24L01 status byte.
//
//  Author: RSK   Date: 28.11.05
//**********************************************************//
BYTE SPI_Read_Buf(BYTE reg, BYTE *pBuf, BYTE bytes)
{
BYTE status,byte_ctr;

  CSN_Pin_LowSelect();                                     // Set CSN low, init SPI tranaction
  status = SPI_RW(reg);                           // Select register to write to and read status byte

  for(byte_ctr=0;byte_ctr<bytes;byte_ctr++)
    pBuf[byte_ctr] = SPI_RW(0);                   // Perform SPI_RW to read byte from nRF24L01

  CSN_Pin_HighDisable();                                     // Set CSN high again

  return(status);                                 // return nRF24L01 status byte
}

