//*******************************************************************
/*!
\file   Port_Mcu.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   14.02.2011

License: See file "LICENSE"
*/

//*******************************************************************
#include "Port_Mcu.h"
#include "Hardware/ReportID_Hw.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// Port_Mcu
//
//*******************************************************************
//-------------------------------------------------------------------
Port_Mcu::Port_Mcu( PortId portId )
  : Port( ReportID_Hw::Module::PORT_MCU )
{
  switch( portId )
  {
    case PortB: ptr = &PINB; break;
    case PortC: ptr = &PINC; break;
    case PortD: ptr = &PIND; break;
    case PortE: ptr = &PINE; break;
    default:    report.error( ReportID_Hw::Event::WRONG_ID );
                break;
  }
}

//-------------------------------------------------------------------
void Port_Mcu::setMode( const MTYPE mask, Mode mode )
{
  BYTE m = mask;
  for( BYTE i = 0; i < 8; i++, m >>= 1 )
  {
    if( m & 0x01 )
    {
      setPinMode( i, mode );
    }
  }
}

//-------------------------------------------------------------------
void Port_Mcu::setPinMode( const BYTE pinId, Mode mode )
{
  DWORD config = 0;

  if( mode & Port::Out ) config = PinConfig::OUTPUT;
  else 
  { 
    if( mode & Port::PU )
          config = PinConfig::INPUT;
  else
      config = PinConfig::INPUT_PULLUP;
  }
  PinConfig::set( (BYTE*)ptr, pinId, config );
}

//-------------------------------------------------------------------
void Port_Mcu::set( const MTYPE mask )
{
  System::disableInterrupt();  // not needed, if toggle function or SBI used?

  *(ptr+2) |= mask;

  System::enableInterrupt();
}

//-------------------------------------------------------------------
void Port_Mcu::set( const MTYPE mask, MTYPE value )
{
  System::disableInterrupt(); // not needed, if toggle function or SBI  used?

  *(ptr+2) &= ~(mask & ~value);
  *(ptr+2) |=  (mask &  value);

  System::enableInterrupt();
}

//-------------------------------------------------------------------
void Port_Mcu::clr( const MTYPE mask )
{
  System::disableInterrupt(); // not needed, if toggle function  or SBI used?

  *(ptr+2) &= ~mask;

  System::enableInterrupt();
}

//-------------------------------------------------------------------
MTYPE Port_Mcu::get( void )
{
  System::disableInterrupt(); // not needed

  BYTE ret = *ptr;

  System::enableInterrupt();

  return(ret);
}

}  } //namespace

// EOF
