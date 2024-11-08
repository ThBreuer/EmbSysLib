//*******************************************************************
/*!
\file   PinConfig.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   16.10.2022

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
void PinConfig::set( GPIO_TypeDef *gpio, BYTE pinId, DWORD mode, BYTE af )
{
  BYTE pin2 = 2*pinId;
  BYTE pin3 = pinId>>3;

  MTYPE bitmask1 = ~((MTYPE)0x01 <<     pinId    );
  MTYPE bitmask2 = ~((MTYPE)0x03 <<     pin2     );
  MTYPE bitmask3 = ~((MTYPE)0x0F << (4*(pinId%8)));

  MTYPE moder   = (MTYPE)((mode    )&0x03) << pin2;
  MTYPE pupdr   = (MTYPE)((mode>> 8)&0x03) << pin2;
  MTYPE otyper  = (MTYPE)((mode>> 4)&0x01) << pinId;
  MTYPE ospeedr = (MTYPE)((mode>>12)&0x03) << pin2;
  MTYPE afr     = (MTYPE)((af      )&0x0F) << (4*(pinId%8));

  Hw::System::disableInterrupt();
  gpio->MODER     = ( gpio->MODER     & bitmask2 ) | moder;
  gpio->PUPDR     = ( gpio->PUPDR     & bitmask2 ) | pupdr;
  gpio->OTYPER    = ( gpio->OTYPER    & bitmask1 ) | otyper;
  gpio->OSPEEDR   = ( gpio->OSPEEDR   & bitmask2 ) | ospeedr;
  gpio->AFR[pin3] = ( gpio->AFR[pin3] & bitmask3 ) | afr;
  Hw::System::enableInterrupt();
}

//-------------------------------------------------------------------
bool PinConfig::set( Function func, DWORD mode )
{
  BYTE port;
  BYTE pin;
  BYTE af;

  GPIO_TypeDef *gpio;

  for( WORD i = 0; table[i] != END_OF_TABLE; i++ )
  {
    if( (table[i] >> 16) == func )
    {
      port = (table[i]>>12)&0x0F;
      pin  = (table[i]>> 4)&0x0F;
      af   = (table[i]>> 0)&0x0F;

      if( (mode & 0x0F) != ANALOG )
      {
        mode |= ALTERNATE_FUNC;
      }

      switch( port )
      {
        case 0: gpio = (GPIO_TypeDef *)(GPIOA_BASE); RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; break;
        case 1: gpio = (GPIO_TypeDef *)(GPIOB_BASE); RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; break;
        case 2: gpio = (GPIO_TypeDef *)(GPIOC_BASE); RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; break;
        case 3: gpio = (GPIO_TypeDef *)(GPIOD_BASE); RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; break;
        case 4: gpio = (GPIO_TypeDef *)(GPIOE_BASE); RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN; break;
        case 5: gpio = (GPIO_TypeDef *)(GPIOF_BASE); RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN; break;
        case 6: gpio = (GPIO_TypeDef *)(GPIOG_BASE); RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN; break;
        case 7: gpio = (GPIO_TypeDef *)(GPIOH_BASE); RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN; break;
        case 8: gpio = (GPIO_TypeDef *)(GPIOI_BASE); RCC->AHB1ENR |= RCC_AHB1ENR_GPIOIEN; break;
        case 9: gpio = (GPIO_TypeDef *)(GPIOJ_BASE); RCC->AHB1ENR |= RCC_AHB1ENR_GPIOJEN; break;
        case 10:gpio = (GPIO_TypeDef *)(GPIOK_BASE); RCC->AHB1ENR |= RCC_AHB1ENR_GPIOKEN; break;

        default:report.error( ReportID_Hw::Event::HARDWARE_NOT_SUPPORTED );
          break;
      }
      set( gpio, pin, mode, af );
      return( true );
    }
  }
  // Error: The table does not contain the func
  report.error( ReportID_Hw::Event::CONFIG_NOT_SUPPORTED );
  return( false );
}

}  } //namespace
