//*******************************************************************
/*!
\file   Port_Mcu.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"
*/

//*******************************************************************
#include "Port_Mcu.h"
#include "PinConfig.h"
#include "Hardware/ReportID_Hw.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// Port_Mcu
//
//*******************************************************************
//-------------------------------------------------------------------
Port_Mcu::Port_Mcu( PortId portId )
  : Port( ReportID_Hw::Module::PORT_MCU )
{
  switch( portId )
  {
    case PA: gpio = GPIOA; RCC->AHBENR |= RCC_AHBENR_GPIOAEN; break;
    case PB: gpio = GPIOB; RCC->AHBENR |= RCC_AHBENR_GPIOBEN; break;
    case PC: gpio = GPIOC; RCC->AHBENR |= RCC_AHBENR_GPIOCEN; break;
    case PD: gpio = GPIOD; RCC->AHBENR |= RCC_AHBENR_GPIODEN; break;
    case PE: gpio = GPIOE; RCC->AHBENR |= RCC_AHBENR_GPIOEEN; break;
    case PF: gpio = GPIOF; RCC->AHBENR |= RCC_AHBENR_GPIOFEN; break;
    case PG: gpio = GPIOG; RCC->AHBENR |= RCC_AHBENR_GPIOGEN; break;
    case PH: gpio = GPIOH; RCC->AHBENR |= RCC_AHBENR_GPIOHEN; break;
    default:
      report.error( ReportID_Hw::Event::WRONG_ID );
      break;
  }
}

//-------------------------------------------------------------------
void Port_Mcu::setMode( MTYPE mask, Mode mode )
{
  for( BYTE i = 0; i < 16; i++, mask >>= 1 )
  {
    if( mask & 0x01 )
    {
      setPinMode( i, mode );
    }
  }
}

//-------------------------------------------------------------------
void Port_Mcu::setPinMode( BYTE pinId, Mode mode )
{
  DWORD config = 0;

  if( mode & Port::Out ) config |= PinConfig::OUTPUT;
  if( mode & Port::PU  ) config |= PinConfig::PULL_UP;
  if( mode & Port::PD  ) config |= PinConfig::PULL_DOWN;
  if( mode & Port::OD  ) config |= PinConfig::OPEN_DRAIN;

  PinConfig::set( gpio, pinId, config );
}

//-------------------------------------------------------------------
inline void Port_Mcu::set( MTYPE mask )
{
  gpio->BSRRL = mask;
}

//-------------------------------------------------------------------
inline void Port_Mcu::set( MTYPE mask, MTYPE value )
{
  gpio->BSRRL = mask &  value;
  gpio->BSRRH = mask & ~value;
}

//-------------------------------------------------------------------
inline void Port_Mcu::clr( MTYPE mask )
{
  gpio->BSRRH = mask;
}

//-------------------------------------------------------------------
inline MTYPE Port_Mcu::get( void )
{
  return( gpio->IDR );
}

}  } //namespace

//EOF
