//*******************************************************************
/*!
\file   Analog.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   18.12.2023

License: See file "LICENSE"
*/

//*******************************************************************
#include "Analog.h"

//*******************************************************************
namespace EmbSysLib {
namespace Dev {

//*******************************************************************
//
// Analog
//
//*******************************************************************
//-------------------------------------------------------------------
Analog::Analog( BYTE  channelIn,
                float gainIn,
                float offsetIn )
{
  channel = channelIn;
  gain    = gainIn;
  offs    = offsetIn;
}

} } //namespace

//EOF
