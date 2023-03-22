//*******************************************************************
/*!
\file   I2Cmaster_Emul.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"
*/

//!< \todo check timing
//*******************************************************************
#include "I2Cmaster_Emul.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// I2Cmaster_Emul
//
//*******************************************************************
//-------------------------------------------------------------------
I2Cmaster_Emul::I2Cmaster_Emul( Port::Pin &pinSCL_In,
                                Port::Pin &pinSDA_In,
                                WORD       twait )

: I2Cmaster( ReportID_Hw::Module::I2C_MASTER_EMUL )
, pinSCL   ( pinSCL_In )
, pinSDA   ( pinSDA_In )

{
  pinSCL.set();
  pinSDA.set();
  pinSCL.setMode( Port::OutPU );
  pinSDA.setMode( Port::OutPU );

  this->twait = twait/2;
}

//-------------------------------------------------------------------
void I2Cmaster_Emul::wait( void )
{
  System::delayMicroSec( twait );
}

//-------------------------------------------------------------------
void I2Cmaster_Emul::waitForAckn( void )
{
  WORD i= 0xFFFF;

  while( (--i) && pinSDA.get() ); // wait for ACK

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
// START-Condition: Fallende Flanke SDA während SCL = High
void I2Cmaster_Emul::start( BYTE size )
{
  wait();
  pinSDA.set();
  wait();
  pinSCL.set();
  wait();
  pinSDA.clr();
  wait();
  pinSCL.clr();
  wait();
}

//-------------------------------------------------------------------
// STOP-Condition: Steigende Flanke SDA während SCL = High
void I2Cmaster_Emul::stop( void )
{
  wait();
  pinSDA.clr();
  wait();
  pinSCL.set();
  wait();
  pinSDA.set();
  wait();
  pinSCL.clr();
  wait();
  pinSCL.set();
}

//-------------------------------------------------------------------
void I2Cmaster_Emul::sendAddr( BYTE addr )
{
  writeByte( addr );
}

//-------------------------------------------------------------------
void I2Cmaster_Emul::writeByte( BYTE byte )
{
  wait();
  pinSCL.clr();
  for( BYTE mask = 0x80; mask; mask >>= 1 )
  {
    pinSDA.set( byte & mask );

    // raising edge on SCL  0 -> 1 -> 0
    wait();
    pinSCL.set();
    wait();
    pinSCL.clr();
  }

  wait();
  pinSDA.set();

  wait();
  waitForAckn();

  wait();
  pinSCL.set();
  wait();
  pinSCL.clr();
}

//-------------------------------------------------------------------
BYTE I2Cmaster_Emul::readByteNack(void) // read with no acknowledge
{
  BYTE byte = 0;

  wait();
  pinSDA.set();

  for (BYTE mask=0x80; mask ; mask >>= 1)
  {
    wait();
    pinSCL.set();
    wait();
    byte |= (pinSDA.get() ? mask : 0 );
    pinSCL.clr();
  }

  pinSDA.set(); // here: NAK

  // raising edge on SCL  0 -> 1 -> 0
  wait();
  pinSCL.set();
  wait();
  pinSCL.clr();
  wait();
  pinSDA.set();
  wait();

  return byte;
}

//-------------------------------------------------------------------
BYTE I2Cmaster_Emul::readByteAck(void) // read with acknowledge
{
  BYTE byte = 0;

  wait();
  pinSDA.set();

  for (BYTE mask=0x80; mask ; mask >>= 1)
  {
    wait();
    pinSCL.set();
    wait();
    byte |= (pinSDA.get() ? mask : 0 );
    wait();
    pinSCL.clr();
  }

  pinSDA.clr(); // here: ACKN

  // raising edge on SCL  0 -> 1 -> 0
  wait();
  pinSCL.set();

  wait();
  pinSCL.clr();
  pinSDA.set();

  return byte;
}

} } //namespace

//EOF
