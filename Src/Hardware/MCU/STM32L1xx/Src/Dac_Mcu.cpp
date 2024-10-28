//*******************************************************************
/*!
\file   Dac_Mcu.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"
*/

//*******************************************************************
#include "Dac_Mcu.h"
#include "PinConfig.h"
#include "Hardware/ReportID_Hw.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// Dac_Mcu
//
//*******************************************************************
//-------------------------------------------------------------------
Dac_Mcu::Dac_Mcu( void )

: Dac( ReportID_Hw::Module::DAC_MCU )

{
  RCC->APB1ENR |= RCC_APB1ENR_DACEN;
}

//-------------------------------------------------------------------
inline void Dac_Mcu::set( BYTE ch, WORD value )
{
  switch( ch )
  {
    default:
      report.error( ReportID_Hw::Event::WRONG_CHANNEL );
      break;
    case CH1: DAC->DHR12L1 = value & 0xFFF0; break;
    case CH2: DAC->DHR12L2 = value & 0xFFF0; break;
  }
}

//-------------------------------------------------------------------
inline void Dac_Mcu::enable( BYTE ch )
{
  switch( ch )
  {
    default:
      report.error( ReportID_Hw::Event::WRONG_CHANNEL );
      break;

    case CH1:
      PinConfig::set( PinConfig::DAC1_OUT, PinConfig::ANALOG );
      DAC->CR |=   DAC_CR_EN1;
      break;

    case CH2:
      PinConfig::set( PinConfig::DAC2_OUT, PinConfig::ANALOG );
      DAC->CR |=   DAC_CR_EN2;
      break;
  }
}

}  } //namespace

//EOF
