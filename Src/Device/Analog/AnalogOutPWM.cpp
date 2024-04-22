//*******************************************************************
/*!
\file   AnalogOutPWM.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   18.12.2023

License: See file "LICENSE"
*/

//*******************************************************************
#include "AnalogOutPWM.h"

//*******************************************************************
namespace EmbSysLib {
namespace Dev {

//*******************************************************************
//
// AnalogOutPWM
//
//*******************************************************************
//-------------------------------------------------------------------
AnalogOutPWM::AnalogOutPWM( Timer       &timer,
                            Timer::Mode  mode,
                            BYTE         channel,
                            float        max,
                            float        min )

: AnalogOut( channel, max, min ),
  timer    ( timer )

{
  timer.enablePWM( channel, mode );
}

//-------------------------------------------------------------------
void AnalogOutPWM::setRaw( WORD value )
{
  timer.setPWM( channel, value );
}

} } //namespace

//EOF
