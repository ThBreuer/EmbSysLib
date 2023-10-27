//*******************************************************************
/*!
\file   PinConfig.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"
*/

//*******************************************************************
#include "PinConfig.h"
#include "Hardware/MCU/System.h"
#include "Hardware/ReportID_Hw.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// PinConfig
//
//*******************************************************************
//-------------------------------------------------------------------
Std::Report PinConfig::report( ReportID_Hw::Module::SYSTEM_PIN_CONFIG );

//-------------------------------------------------------------------
/*
DDR 0 -> input
    1 -> output

Input: PORTR = 1 -> PU on
               0 -> PU off
  
Output: PORTR = 0/1 push pull

                     DDR   PORT
Input  floating       0      0
Input  PU             0      1
Output push pull      1      data
*/
void PinConfig::set( volatile uint8_t *port, BYTE pinId, DWORD mode, BYTE af )
{
  Hw::System::disableInterrupt();

  BYTE mask = 1<<pinId;

  switch( mode )
  {
    case OUTPUT:
      *(port+1) |=  mask;// set DDR
      *(port+2) &= ~mask;// clr PORT
    break;
    case INPUT:
      *(port+1) &= ~mask;// clr DDR
      *(port+2) &= ~mask;// clr PORT
      break;
    case INPUT_PULLUP:
      *(port+1) &= ~mask;// clr DDR
      *(port+2) |=  mask;// set PORT
    break;
  }
  Hw::System::enableInterrupt();
}

}  } //namespace
