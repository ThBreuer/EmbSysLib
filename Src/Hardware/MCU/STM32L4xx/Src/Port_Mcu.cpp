//*******************************************************************
/*!
\file   Port_Mcu.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   16.10.2022

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
    case PA: gpio = (GPIO_TypeDef *)(GPIOA_BASE); RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; break;
    case PB: gpio = (GPIO_TypeDef *)(GPIOB_BASE); RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN; break;
    case PC: gpio = (GPIO_TypeDef *)(GPIOC_BASE); RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN; break;
    case PH: gpio = (GPIO_TypeDef *)(GPIOH_BASE); RCC->AHB2ENR |= RCC_AHB2ENR_GPIOHEN; break;
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
//<! \todo change BSRR to BSRRL/H in stm32*.h
  gpio->BSRR = mask;
}

//-------------------------------------------------------------------
inline void Port_Mcu::set( MTYPE mask, MTYPE value )
{
  gpio->BSRR = mask &  value;
  gpio->BSRR = (mask & ~value)<<16;
}

//-------------------------------------------------------------------
inline void Port_Mcu::clr( MTYPE mask )
{
  gpio->BSRR = mask<<16;
}

//-------------------------------------------------------------------
inline MTYPE Port_Mcu::get( void )
{
  return( gpio->IDR );
}

}  } //namespace

//EOF
