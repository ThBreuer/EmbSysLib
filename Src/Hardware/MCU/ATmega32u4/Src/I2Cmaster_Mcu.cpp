//*******************************************************************
/*!
\file   I2Cmaster_Mcu.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   02.03.2011

License: See file "LICENSE"
*/

//*******************************************************************
#include "I2Cmaster_Mcu.h"
#include "Hardware/ReportID_Hw.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// I2Cmaster_Mcu
//
//*******************************************************************
#ifndef OSC_FREQ
  #error "OSC_FREQ not defined"
#endif

//-------------------------------------------------------------------
// Default register setting of TWCR:
//
#define TWCR_DEFAULT ( (1<<TWINT)   /* Interrupt Flag              */  \
                      |(0<<TWEA )   /* Enable Acknowledge Bit      */  \
                      |(0<<TWSTA)   /* START Condition Bit         */  \
                      |(0<<TWSTO)   /* STOP Condition Bit          */  \
                      |(0<<TWWC )   /* Write Coll. Flag (read only)*/  \
                      |(1<<TWEN )   /* Enable Bit                  */  \
                      |(0<<TWIE ) ) /* Interrupt Enable            */

//-------------------------------------------------------------------
I2Cmaster_Mcu::I2Cmaster_Mcu( WORD fclk )

: I2Cmaster( ReportID_Hw::Module::I2C_MASTER_MCU )

{
  // SCL frequency calculation
  //
  //     fclk = fosc/(16+2*TWBR*4^TWPS)
  // <=>
  //     TWBR*4^TWPS = (fosc/fscl - 16 )/2
  //
  // example: calculated TWBR
  //
  //                  +-----+-----+-----+
  //                  |     fosc/MHz    |
  //  +---------------+-----+-----+-----+
  //  | fclk/kHz TWSR |  12 |  16 |  20 |
  //  +---------------+-----+-----+-----+
  //  |    10     1   | 148 | 198 | 248 |
  //  |   100     0   |  52 |  72 |  92 |
  //  |   400     0   |   7 |  12 |  17 |
  //  |  1000     0   |   1 |   1 |   2 |
  //  +---------------+-----+-----+-----+
  //
  int rate = (OSC_FREQ/fclk/1000 - 16)/2;
 
  if( rate <= 0 )
  {
    TWBR = 1;
    TWSR = 0;
  }
  else if( rate <= 255 )
  {
    TWBR = rate;
    TWSR = 0;
  }
  else if( rate <= 1023 )
  {
    TWBR = rate/4;
    TWSR = 1;
  }
  else
  {
    TWBR = 255;
    TWSR = 1;
  }
}

//-------------------------------------------------------------------
void I2Cmaster_Mcu::start( BYTE size )
{
  TWCR =  TWCR_DEFAULT | (1<<TWSTA);  // START Condition Bit
  waitForAckn(0);
}

//-------------------------------------------------------------------
void I2Cmaster_Mcu::stop( void )
{
  TWCR = TWCR_DEFAULT | (1<<TWSTO);  // STOP Condition Bit
}

//-------------------------------------------------------------------
void I2Cmaster_Mcu::sendAddr( BYTE addr )
{
  TWDR = addr;

  TWCR = TWCR_DEFAULT;
  waitForAckn(0);
}

//-------------------------------------------------------------------
void I2Cmaster_Mcu::writeByte( BYTE byte )
{
  TWDR = byte;

  TWCR = TWCR_DEFAULT;
  waitForAckn(0);
}

//-------------------------------------------------------------------
BYTE I2Cmaster_Mcu::readByteAck( void ) // read with no acknowledge
{
  TWCR = TWCR_DEFAULT;
  waitForAckn(0);

  return TWDR;
}

//-------------------------------------------------------------------
BYTE I2Cmaster_Mcu::readByteNack( void ) // read with no acknowledge
{
  TWCR = TWCR_DEFAULT;
  waitForAckn(0);

  return TWDR;
}

//-------------------------------------------------------------------
void I2Cmaster_Mcu::waitForAckn( MTYPE state )
{
  WORD i = 0xFFF;

  while( (--i) && !(TWCR & (1<<TWINT)) ); // wait for ACK

  if( i == 0 )
  {
    isHardwareOK = false;
    stop();
  }
  else
  {
    isHardwareOK = true;
  }
}

//-------------------------------------------------------------------
#undef TWCR_DEFAULT

}  } //namespace

//EOF
