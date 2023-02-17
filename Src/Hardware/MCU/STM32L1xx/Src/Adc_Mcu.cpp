//*******************************************************************
/*!
\file   Adc_Mcu.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"
*/

//*******************************************************************
#include "Adc_Mcu.h"
#include "PinConfig.h"
#include "Hardware/ReportID_Hw.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// Adc_Mcu
//
//*******************************************************************
//-------------------------------------------------------------------
Adc *Adc_Mcu::adcPtr = 0;

//-------------------------------------------------------------------
Adc_Mcu::Adc_Mcu( Timer &timer )

: Adc( ReportID_Hw::Module::ADC_MCU, 16 /*num of channels*/, timer )

{
  adcPtr = this;

  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
  
  while( (ADC1->SR & ADC_SR_ADONS) );
  
  // Clock control register, ADC needs HSI
  RCC->CR |= RCC_CR_HSION;
  while( !(RCC->CR & RCC_CR_HSIRDY) ); // wait for HSI ready

  ADC1->CR2 |=    ADC_CR2_ADON  // A/D Converter: ON
               | !ADC_CR2_CONT  // Continuous Conversion: OFF
               |  ADC_CR2_ALIGN;// Data alignment: left

  ADC1->CR1 |=    ADC_CR1_EOCIE; // End of conversion interrupt: enable

  while( !(ADC1->SR & ADC_SR_ADONS) );

  NVIC_EnableIRQ( ADC1_IRQn );
}

//-------------------------------------------------------------------
void Adc_Mcu::configCh( BYTE ch, BYTE para )
{
  switch( ch )
  {
    default:
      report.error( ReportID_Hw::Event::WRONG_CHANNEL );
      break;
    case  0: PinConfig::set( PinConfig::ADC12_IN0,  PinConfig::ANALOG ); break;
    case  1: PinConfig::set( PinConfig::ADC12_IN1,  PinConfig::ANALOG ); break;
    case  2: PinConfig::set( PinConfig::ADC12_IN2,  PinConfig::ANALOG ); break;
    case  3: PinConfig::set( PinConfig::ADC12_IN3,  PinConfig::ANALOG ); break;
    case  4: PinConfig::set( PinConfig::ADC12_IN4,  PinConfig::ANALOG ); break;
    case  5: PinConfig::set( PinConfig::ADC12_IN5,  PinConfig::ANALOG ); break;
    case  6: PinConfig::set( PinConfig::ADC12_IN6,  PinConfig::ANALOG ); break;
    case  7: PinConfig::set( PinConfig::ADC12_IN7,  PinConfig::ANALOG ); break;
    case  8: PinConfig::set( PinConfig::ADC12_IN8,  PinConfig::ANALOG ); break;
    case  9: PinConfig::set( PinConfig::ADC12_IN9,  PinConfig::ANALOG ); break;
    case 10: PinConfig::set( PinConfig::ADC12_IN10, PinConfig::ANALOG ); break;
    case 11: PinConfig::set( PinConfig::ADC12_IN11, PinConfig::ANALOG ); break;
    case 12: PinConfig::set( PinConfig::ADC12_IN12, PinConfig::ANALOG ); break;
    case 13: PinConfig::set( PinConfig::ADC12_IN13, PinConfig::ANALOG ); break;
    case 14: PinConfig::set( PinConfig::ADC12_IN14, PinConfig::ANALOG ); break;
    case 15: PinConfig::set( PinConfig::ADC12_IN15, PinConfig::ANALOG ); break;
  }
}

//-------------------------------------------------------------------
inline WORD Adc_Mcu::getResult( void )
{
  return( ADC1->DR & 0xFFFF );
}

//-------------------------------------------------------------------
inline void Adc_Mcu::startCh( BYTE ch )
{
  // select channel
  ADC1->SQR5 = ch & 0x1F;

  ADC1->CR2 |= ADC_CR2_SWSTART;
}

//-------------------------------------------------------------------
extern "C"
{
  void ADC1_IRQHandler(void)
  {
    ADC1->SR &= ~ADC_SR_EOC;
    Adc_Mcu::adcPtr->isr();
  }
}

}  } //namespace

//EOF
