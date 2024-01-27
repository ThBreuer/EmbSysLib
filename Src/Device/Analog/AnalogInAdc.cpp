//*******************************************************************
/*!
\file   AnalogInAdc.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   18.12.2023

License: See file "LICENSE"
*/

//*******************************************************************
#include "AnalogInAdc.h"

//*******************************************************************
namespace EmbSysLib {
namespace Dev {

//*******************************************************************
//
// AnalogInAdc
//
//*******************************************************************
//-------------------------------------------------------------------
AnalogInAdc::AnalogInAdc( Adc   &adcIn,
                          BYTE   channel,
                          float  max,
                          float  min,
                          BYTE   para )

: AnalogIn( channel, max, min ),
  adc     ( adcIn )

{
  this->para = para;
  adc.enable( channel, para );
}

//-------------------------------------------------------------------
WORD AnalogInAdc::getRaw( void )
{
  return( adc.get( channel ) );
}

//-------------------------------------------------------------------
void AnalogInAdc::enable()
{
    adc.enable( channel, para );
}

} } //namespace

//EOF
