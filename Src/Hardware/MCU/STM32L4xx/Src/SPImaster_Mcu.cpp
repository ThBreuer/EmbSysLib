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
#if ( system_PCLK_SPI != 80000000 )
  #error "system_PCLK_SPI not defined or wrong value"
#endif

//-------------------------------------------------------------------
SPImaster_Mcu::SPImaster_Mcu( SPI_ID       id,
                              SPI_Baudrate baudrate,
                              ClockPolPha  clockPolPha,
                              bool         lsbFirst )

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
      PinConfig::set( PinConfig::SPI1_SCK,  PinConfig::LOW_SPEED );
      PinConfig::set( PinConfig::SPI1_MISO );
      PinConfig::set( PinConfig::SPI1_MOSI );
      PinConfig::set( PinConfig::SPI1_NSS  );
      break;

    case SPI_3:
      ptr           = SPI3;
      RCC->APB1ENR1 |= RCC_APB1ENR1_SPI3EN;
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
    // Valid for PCLK_SPI = 80MHz
    case CR_312kHz:  br = 7; /* 80MHz/256 */ break;
    case CR_625kHz:  br = 6; /* 80MHz/128 */ break;
    case CR_1250kHz: br = 5; /* 80MHz/64  */ break;
    case CR_2500kHz: br = 4; /* 80MHz/32  */ break;
    case CR_5MHz:    br = 3; /* 80MHz/16  */ break;
    case CR_10MHz:   br = 2; /* 80MHz/8   */ break;
    case CR_20MHz:   br = 1; /* 80MHz/4   */ break;
    case CR_40MHz:   br = 0; /* 80MHz/2   */ break;
  }

  ptr->CR1 &= ~SPI_CR1_SPE;

  ptr->CR2  =  !SPI_CR2_FRXTH
              | ((8-1)<<8)         // Data Size: 8-bit
             | !SPI_CR2_FRF       // Frame format: Motorola
             | !SPI_CR2_SSOE;     // Slave Select output disable

  ptr->CR1  =   SPI_CR1_SSM       // Slave management: HW
             |  SPI_CR1_SSI       //
             |  (lsbFirst?SPI_CR1_LSBFIRST:0)  // LSB first
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
  // \see: https://community.st.com/t5/stm32-mcus-products/stm32l496-spi-16-clock-cycles-instead-of-8/td-p/231346
 *((__IO uint8_t *)&ptr->DR) = data;
//ptr->DR = data;

  // wait until transmission completed
  while( (ptr->SR & SPI_SR_BSY))
  {
    asm("NOP");
  };

    ret = *((__IO uint8_t *)&ptr->DR);
  // wait until data received
  while( (ptr->SR & SPI_SR_FRLVL) )
  {
     x = ptr->DR; // dummy read
  }

  return( ret );
}

}  } //namespace

//EOF
