//*******************************************************************
/*!
\file   AnalogOutDac.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   18.12.2023

License: See file "LICENSE"
*/

//*******************************************************************
#include "AnalogOutDac.h"

//*******************************************************************
namespace EmbSysLib {
namespace Dev {

//*******************************************************************
//
// AnalogOutDac
//
//*******************************************************************
//-------------------------------------------------------------------
AnalogOutDac::AnalogOutDac( Dac   &dac,
                            BYTE   channel,
                            float   max,
                            float   min )

:AnalogOut( channel, max, min ),
 dac      ( dac )

{
  dac.enable( channel );
}

//-------------------------------------------------------------------
void AnalogOutDac::setRaw( WORD value )
{
  dac.set( channel, value );
}

} } //namespace

//EOF
