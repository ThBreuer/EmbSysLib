//*******************************************************************
/*!
\file   Port.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"
*/

//*******************************************************************
#include "Port.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// Port
//
//*******************************************************************
//-------------------------------------------------------------------
Port::Port( WORD moduleId )
: report( moduleId )
{
}

//*******************************************************************
//
// Port::Pin
//
//*******************************************************************
//-------------------------------------------------------------------
Port::Pin::Pin( Port &port, BYTE pinId )
: pinId  ( pinId    )
, pinMask( 1<<pinId )
, port   ( port     )
{
}

//-------------------------------------------------------------------
Port::Pin::Pin( Port &port, BYTE pinId, Mode mode )
: pinId  ( pinId    )
, pinMask( 1<<pinId )
, port   ( port     )
{
  port.setPinMode( pinId, mode );
}

//-------------------------------------------------------------------
inline void Port::Pin::setMode( Mode mode )
{
  port.setPinMode( pinId, mode );
}

//-------------------------------------------------------------------
inline void Port::Pin::set( void )
{
  port.set( pinMask );
}

//-------------------------------------------------------------------
inline void Port::Pin::set( bool value )
{
  if( value )  port.set( pinMask );
  else         port.clr( pinMask );
}

//-------------------------------------------------------------------
inline void Port::Pin::clr( void )
{
  port.clr( pinMask );
}

//-------------------------------------------------------------------
inline bool Port::Pin::get( void )
{
  return( (port.get() & pinMask) != 0 );
}

}  } //namespace

//EOF
