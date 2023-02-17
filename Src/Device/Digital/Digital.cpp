//*******************************************************************
/*!
\file   Digital.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   04.11.2022

License: See file "LICENSE"
*/

//*******************************************************************
#include "Digital.h"

//*******************************************************************
namespace EmbSysLib {
namespace Dev {

//*******************************************************************
//
// Digital
//
//*******************************************************************
//-------------------------------------------------------------------
Digital::Digital( Port  &port,
                  BYTE   pinId,
                  Mode   mode,
                  bool   def )
: pin( port, pinId )
{
  valueIn    = 0;
  valueOut   = 0;
  pinDefault = def;

  pin.set( def );
  pin.setMode( (Port::Mode)mode );
}

//-------------------------------------------------------------------
void Digital::setMode( Mode mode )
{
  pin.setMode( (Port::Mode)mode );
}

//-------------------------------------------------------------------
void Digital::set( bool value )
{
  valueOut = value;
  pin.set( value ^ pinDefault );
}

//-------------------------------------------------------------------
bool Digital::get( void )
{
  return( pin.get() != pinDefault );
}

} } //namespace

//EOF
