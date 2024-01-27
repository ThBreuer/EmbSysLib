//*******************************************************************
/*!
\file   AnalogOut.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   18.12.2023

License: See file "LICENSE"
*/

//*******************************************************************
#include "AnalogOut.h"

//*******************************************************************
namespace EmbSysLib {
namespace Dev {

//*******************************************************************
//
// AnalogOut
//
//*******************************************************************
//-------------------------------------------------------------------
AnalogOut::AnalogOut( BYTE  channel,
                      float max,
                      float min )

: Analog( channel,
         (float)0xFFFF*( (max != min) ? 1.0F/(max-min) : 1.0F ),
          min )

{
}

//-------------------------------------------------------------------
void AnalogOut::set( float value )
{
  setRaw( (WORD)RANGE( gain*(value - offs),
                      (float)0,
                      (float)0xFFFF) );
}

//-------------------------------------------------------------------
inline float AnalogOut::operator=( float value )
{
  set( value );
  return( value );
}

} } //namespace

//EOF
