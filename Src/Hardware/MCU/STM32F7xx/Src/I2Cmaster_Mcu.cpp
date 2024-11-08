//*******************************************************************
/*!
\file   I2Cmaster_Mcu.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"
*/

//*******************************************************************
#include "I2Cmaster_Mcu.h"
#include "PinConfig.h"
#include "Hardware/ReportID_Hw.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// I2Cmaster_Mcu
//
//*******************************************************************
//-------------------------------------------------------------------
#if ( system_PCLK_I2C != 50000000 )
  #error "system_PCLK_I2C not defined or wrong value"
#endif

//-------------------------------------------------------------------
I2Cmaster_Mcu::I2Cmaster_Mcu( I2C_ID id,
                              WORD   fclk )

: I2Cmaster( ReportID_Hw::Module::I2C_MASTER_MCU )

{
  switch( id )
  {
    case I2C_1:
      ptr           = I2C1;
      RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
      PinConfig::set( PinConfig::I2C1_SCL, PinConfig::OPEN_DRAIN | PinConfig::PULL_UP );
      PinConfig::set( PinConfig::I2C1_SDA, PinConfig::OPEN_DRAIN | PinConfig::PULL_UP );
      break;

    case I2C_2:
      ptr           = I2C2;
      RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
      PinConfig::set( PinConfig::I2C2_SCL, PinConfig::OPEN_DRAIN | PinConfig::PULL_UP );
      PinConfig::set( PinConfig::I2C2_SDA, PinConfig::OPEN_DRAIN | PinConfig::PULL_UP );
      break;

    case I2C_3:
      ptr           = I2C3;
      RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;
      PinConfig::set( PinConfig::I2C3_SCL, PinConfig::OPEN_DRAIN | PinConfig::PULL_UP );
      PinConfig::set( PinConfig::I2C3_SDA, PinConfig::OPEN_DRAIN | PinConfig::PULL_UP );
      break;

    case I2C_4:
      ptr           = I2C4;
      RCC->APB1ENR |= RCC_APB1ENR_I2C4EN;
      PinConfig::set( PinConfig::I2C4_SCL, PinConfig::OPEN_DRAIN | PinConfig::PULL_UP );
      PinConfig::set( PinConfig::I2C4_SDA, PinConfig::OPEN_DRAIN | PinConfig::PULL_UP );
      break;

    default:
      report.error( ReportID_Hw::Event::WRONG_ID );
      break;
  }

  ptr->CR1 = 0x00; // disable
  while( ptr->CR1 & I2C_CR1_PE ) {}

  ptr->CR1 |=   (!I2C_CR1_ANFOFF)   // keep analog filter on
              | ( 0<<8);          // digital filter n*tI2CCLK

  /* Timing
     ======
       PRESC+1 = 16 (max), system_PCLK_I2C = 80 MHz
         => tpresc = 16*0.0125us = 0.2 us
         => tsclh,l <= 256*tpresc = 51.2us

       Fm with tsclh/tscll = 2:
         fclk = 1/(tsclh + tscll), tsclh = 2*tscll
           => fclk = 1/(3*tscll) and fclk = 2/(3*tsclh)
           => scll = 1/(3*fclk*tpresc) and sclh = 2/(3*fclk*tpresc)
  */
  fclk = RANGE( fclk, (WORD)1, (WORD)1000 );

  WORD sclh = MIN( (WORD)(3333/fclk - 1), (WORD)(0xFF) );
  WORD scll = MIN( (WORD)(1667/fclk - 1), (WORD)(0xFF) );

  ptr->TIMINGR =  (0x0F<<28)   // PRESC
                 |(0x00<<20)   // SCLDEL, 1*0.2us
                 |(0x00<<16)   // SDADEL
                 |(sclh<< 8)   // SCLH
                 |(scll<< 0);  // SCLL

  ptr->CR1 |= I2C_CR1_PE; // enable
}

//-------------------------------------------------------------------
inline void I2Cmaster_Mcu::start( BYTE size )
{
  ptr->CR2 = (ptr->CR2 & ~0xFF0000)|((size)<<16);
  ptr->CR2 = (ptr->CR2 & ~(1<<10));
  // set start condition AFTER setting of SADD in sendAddr
}

//-------------------------------------------------------------------
inline void I2Cmaster_Mcu::stop( void )
{
  ptr->CR2 |= I2C_CR2_STOP;
}

//-------------------------------------------------------------------
inline void I2Cmaster_Mcu::sendAddr( BYTE addr )
{
  if( isHardwareOK )
  {
    ptr->CR2 = (ptr->CR2 & ~0x2FF)|(addr&0xFE);
    if(addr & 0x01)
    {
      ptr->CR2 = (ptr->CR2 | (1<<10));
    }
    else
    {
      ptr->CR2 = (ptr->CR2 & ~(1<<10));
    }
    ptr->CR2 |= I2C_CR2_START;
  }
}

//-------------------------------------------------------------------
inline void I2Cmaster_Mcu::writeByte( BYTE byte )
{
  if( isHardwareOK )
  {
    ptr->TXDR = byte;
    waitForAckn( I2C_ISR_TXE );
  }
}

//-------------------------------------------------------------------
inline BYTE I2Cmaster_Mcu::readByteAck( void )
{
  if( isHardwareOK )
  {
    waitForAckn( I2C_ISR_RXNE );
    return( ptr->RXDR );
  }
  return( 0 );
}

//-------------------------------------------------------------------
inline BYTE I2Cmaster_Mcu::readByteNack( void )
{
  if( isHardwareOK )
  {
    waitForAckn( I2C_ISR_RXNE );
    return( ptr->RXDR );
  }
  return( 0 );
}

//-------------------------------------------------------------------
inline void I2Cmaster_Mcu::waitForAckn( MTYPE state )
{
  WORD i = 0xFFFF;

  if( isHardwareOK )
  {
    while( (--i) && !(ptr->ISR & state)  );    // wait for ACK
  }

  if( i == 0 )
  {
    isHardwareOK = false;
    stop();
  }
}

}  } //namespace

//EOF
