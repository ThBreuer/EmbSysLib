//*******************************************************************
/*!
\file   AnalogIn.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   18.12.2023

License: See file "LICENSE"
*/

//*******************************************************************
#include "AnalogIn.h"

//*******************************************************************
namespace EmbSysLib {
namespace Dev {

//*******************************************************************
//
// AnalogIn
//
//*******************************************************************
//-------------------------------------------------------------------
AnalogIn::AnalogIn( BYTE  channel,
                    float max,
                    float min )

: Analog( channel,
          (max-min)/(float)0xFFFF,
          min )
{
}

//-------------------------------------------------------------------
inline float AnalogIn::get( void )
{
  return( gain*getRaw() + offs );
}

//-------------------------------------------------------------------
inline AnalogIn::operator float( void )
{
  return( get() );
}

//-------------------------------------------------------------------
void AnalogIn::calibrate( float x1, 
                          float y1,
                          float x2,
                          float y2 )
{
  if( x1 != x2 ) 
  {
    gain = (y2-y1)/(x2-x1);
    offs = y1 - gain*x1;
  }
}


} } //namespace

//EOF
