//*******************************************************************
/*!
\file   SPImaster_Mcu.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   20.03.2023

License: See file "LICENSE"
*/

//*******************************************************************
#include "SPImaster_Mcu.h"
#include "PinConfig.h"
#include "Hardware/ReportID_Hw.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// cHwSPImaster_N
//
//*******************************************************************
//-------------------------------------------------------------------
#if ( system_PCLK_SPI != 32000000 )
  #error "system_PCLK_SPI not defined or wrong value"
#endif

//-------------------------------------------------------------------
SPImaster_Mcu::SPImaster_Mcu( SPI_ID         id, 
                                SPI_Baudrate baudrate,
                                ClockPolPha  clockPolPha )

: SPImaster() 

{
  switch( id )
  {
    default:
      // error!
      break;
      
    case SPI_1:
      ptr           = SPI1;
      RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
      PinConfig::set( PinConfig::SPI1_SCK,  PinConfig::MEDIUM_SPEED );
      PinConfig::set( PinConfig::SPI1_MISO );
      PinConfig::set( PinConfig::SPI1_MOSI );
      PinConfig::set( PinConfig::SPI1_NSS  );
      break;

    case SPI_2:
      ptr           = SPI2;
      RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
      PinConfig::set( PinConfig::SPI2_SCK,  PinConfig::MEDIUM_SPEED  );
      PinConfig::set( PinConfig::SPI2_MISO );
      PinConfig::set( PinConfig::SPI2_MOSI );
      PinConfig::set( PinConfig::SPI2_NSS  );
      break;

    case SPI_3:
      ptr           = SPI3;
      RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;
      PinConfig::set( PinConfig::SPI3_SCK,  PinConfig::MEDIUM_SPEED  );
      PinConfig::set( PinConfig::SPI3_MISO );
      PinConfig::set( PinConfig::SPI3_MOSI );
      PinConfig::set( PinConfig::SPI3_NSS  );
      break;
  }

  WORD br;
  
  switch( baudrate )
  {
    default:
    // Baud rate control, prescaler: clock rate = PCLK_SPI / 2^(br+1)
    // Valid for PCLK_SPI = 32MHz
    case CR_250kHz:  br = 6; /* 32MHz/128 */ break;
    case CR_500kHz:  br = 5; /* 32MHz/64  */ break;
    case CR_1000kHz: br = 4; /* 32MHz/32  */ break;
    case CR_2000kHz: br = 3; /* 32MHz/16  */ break;
    case CR_4000kHz: br = 2; /* 32MHz/8   */ break;
    case CR_8000kHz: br = 1; /* 32MHz/4   */ break;
  } 

  ptr->CR2 |=  !SPI_CR2_FRF       // Frame format: Motorola
             |  SPI_CR2_SSOE;     // Slave Select output enable

  ptr->CR1  =  !SPI_CR1_DFF       // Data frame format: 8-bit
             | !SPI_CR1_SSM       // Slave management: SW
             |  SPI_CR1_SSI       //
             |  SPI_CR1_LSBFIRST  // LSB first: disable
             | !SPI_CR1_SPE       // SPI disable
             |  (br<<3)           // Baud rate
             |  SPI_CR1_MSTR      // Master mode
             |  ((clockPolPha & 0x01)?SPI_CR1_CPOL:0)
             |  ((clockPolPha & 0x10)?SPI_CR1_CPHA:0);

  ptr->CR1 |= SPI_CR1_SPE;

  // Dummy-Read to push SCK to high
  transceiveByte( 0x00 );
}

//-------------------------------------------------------------------
BYTE SPImaster_Mcu::transceiveByte( BYTE data )
{
  volatile BYTE x=0;
  volatile BYTE ret=0;

  // wait until transmission buffer empty
  while( !(ptr->SR & SPI_SR_TXE) )
  {
    asm("NOP");
  };

  // start transmission
  ptr->DR = data;

  // wait until transmission completed
  while( !(ptr->SR & SPI_SR_RXNE))
  {
    asm("NOP");
  };

  ret = ptr->DR;
  // wait until data received
  while( (ptr->SR & SPI_SR_RXNE) )
  {
     x = ptr->DR; // dummy read
  }

  return( ret );
}

}  } //namespace

//EOF
