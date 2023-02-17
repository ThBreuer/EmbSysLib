//*******************************************************************
/*!
\file   Port_PCF8574.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   29.12.2022

License: See file "LICENSE"
*/

//*******************************************************************
#include "Port_PCF8574.h"
#include "Hardware/ReportID_Hw.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
// 
// Port_PCF8574
//
//*******************************************************************
//-------------------------------------------------------------------
Port_PCF8574::Port_PCF8574( I2Cmaster &i2cIn, 
                            BYTE       hwAddrIn)

: Port( ReportID_Hw::Module::PORT_PERIPHERAL )

, i2c(i2cIn, baseAdr | ( (hwAddrIn&maskAdr) << 1 ))

{
  valueIn  = 0;
  valueOut = 0;
  setMode( (MTYPE)(-1), Port::In );
}

//-------------------------------------------------------------------
void Port_PCF8574::setMode( const MTYPE mask, Mode mode )
{
  if( mode & Out )
    dir |= mask;
  else
    dir &= ~mask; 

  i2c.write( (BYTE)(valueOut | ~dir) );
}

//-------------------------------------------------------------------
void Port_PCF8574::setPinMode( const BYTE pinId, Mode mode )
{
  setMode( 1<<pinId, mode );
}

//-------------------------------------------------------------------
void Port_PCF8574::set( MTYPE mask )
{
  valueOut |= mask;
  i2c.write( (BYTE)(valueOut | ~dir) );
}

//-------------------------------------------------------------------
void Port_PCF8574::set( MTYPE mask, MTYPE value)
{
  valueOut |=  (mask &  value);  
  valueOut &= ~(mask & ~value);
  i2c.write( (BYTE)(valueOut | ~dir) );
}

//-------------------------------------------------------------------
void Port_PCF8574::clr( MTYPE mask )
{
  valueOut &= ~mask;
  i2c.write( (BYTE)(valueOut | ~dir) );
}

//-------------------------------------------------------------------
MTYPE Port_PCF8574::get( void )
{
  valueIn = i2c.read();
  return( valueIn );
}

}  } //namespace

//EOF
