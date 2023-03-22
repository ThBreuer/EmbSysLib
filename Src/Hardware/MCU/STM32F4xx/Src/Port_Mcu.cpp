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
    case PA: gpio = (GPIO_TypeDef *)(GPIOA_BASE); RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; break;
    case PB: gpio = (GPIO_TypeDef *)(GPIOB_BASE); RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; break;
    case PC: gpio = (GPIO_TypeDef *)(GPIOC_BASE); RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; break;
    case PD: gpio = (GPIO_TypeDef *)(GPIOD_BASE); RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; break;
    case PE: gpio = (GPIO_TypeDef *)(GPIOE_BASE); RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN; break;
    case PF: gpio = (GPIO_TypeDef *)(GPIOF_BASE); RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN; break;
    case PG: gpio = (GPIO_TypeDef *)(GPIOG_BASE); RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN; break;
    case PH: gpio = (GPIO_TypeDef *)(GPIOH_BASE); RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN; break;
    case PI: gpio = (GPIO_TypeDef *)(GPIOI_BASE); RCC->AHB1ENR |= RCC_AHB1ENR_GPIOIEN; break;
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

}  } //namespace

//EOF
