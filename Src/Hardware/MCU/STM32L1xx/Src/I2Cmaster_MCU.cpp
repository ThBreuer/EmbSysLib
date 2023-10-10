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
#if ( system_PCLK_I2C != 32000000 )
  #error "system_PCLK_I2C not defined or wrong value"
#endif

//!< \todo check timing
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

    default:
      report.error( ReportID_Hw::Event::WRONG_ID );
      break;
  }

  // FREQ (Peripheral clock frequency)
  ptr->CR2 &= ~0x3F; 
  ptr->CR2 |= RANGE( system_PCLK_I2C/1000000, 2, 32 ); // peripheral input clock

  // CCR, FS and DUTY
  //
  //         | Sm   Fm   Fm&DUTY
  //   ------|------------------
  //   tlow  | 1*T  2*T  16*T
  //   thigh | 1*T  1*T   9*T
  //   Tges  | 2*T  3*T  25*T
  //
  //   T = CCR*Tpclk1
  //   Tges = n*T = n*CCR*Tpclk1 => CCR = fpclk/(n*fges)
  //   Sm => up to 100kHz, Fm => up to 400kHz
  // here: Fm only, no DUTY
  fclk = RANGE( fclk, (WORD)1, (WORD)1000 );
  ptr->CCR =   (system_PCLK_I2C/(3000*fclk)) 
             | I2C_CCR_FS 
             | !I2C_CCR_DUTY;
 
  ptr->TRISE = (WORD)(0.1e-6 * system_PCLK_I2C) + 1; // Trise = 0.1µs

  ptr->CR1 |= I2C_CR1_PE; // enable
}

//-------------------------------------------------------------------
inline void I2Cmaster_Mcu::start( BYTE size )
{
  if( isHardwareOK )
  {
    ptr->CR1 &= ~(I2C_CR1_STOP | I2C_CR1_START);
    ptr->CR1 |=  (               I2C_CR1_START);
    waitForAckn( I2C_SR1_SB );
  }
}

//-------------------------------------------------------------------
inline void I2Cmaster_Mcu::stop( void )
{
  ptr->CR1 |= ( I2C_CR1_STOP );
}

//-------------------------------------------------------------------
inline void I2Cmaster_Mcu::sendAddr( BYTE addr )
{
  if( isHardwareOK )
  {
    volatile BYTE tmp = ptr->SR1;
    ptr->DR = addr;   
    if( addr & 0x01 ) // master read
    {
      ptr->CR1 |= (I2C_CR1_ACK );
    }
    waitForAckn( I2C_SR1_ADDR | I2C_SR1_TXE );
    tmp = ptr->SR2;
  }
}

//-------------------------------------------------------------------
inline void I2Cmaster_Mcu::writeByte( BYTE byte )
{
  if( isHardwareOK )
  {
    ptr->DR = byte; 
    waitForAckn( I2C_SR1_TXE );
    volatile BYTE tmp = ptr->SR2;
  }
}

//-------------------------------------------------------------------
inline BYTE I2Cmaster_Mcu::readByteAck( void )
{
  if( isHardwareOK )
  {
    ptr->CR1 |= (I2C_CR1_ACK );
    waitForAckn( I2C_SR1_RXNE ); 
    return( ptr->DR );
  }
  return( 0 );
}

//-------------------------------------------------------------------
inline BYTE I2Cmaster_Mcu::readByteNack( void )
{
  if( isHardwareOK )
  {
    ptr->CR1 &= ~(I2C_CR1_ACK );
    waitForAckn( I2C_SR1_RXNE ); 
    return( ptr->DR );
  }
  return( 0 );
}

//-------------------------------------------------------------------
inline void I2Cmaster_Mcu::waitForAckn( MTYPE state )
{
  WORD i = 0xFFFF;

  if( isHardwareOK )
  {
    while( (--i) && !(ptr->SR1 & state) );    // wait for ACK
  }

  if( i == 0 ) 
  {
    isHardwareOK = false; 
    stop();
  }
  //!< \todo : else isHardwareOK = true; ???

}

}  } //namespace

//EOF
