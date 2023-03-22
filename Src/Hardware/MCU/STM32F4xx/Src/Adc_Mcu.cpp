//*******************************************************************
/*!
\file   Adc_Mcu.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   16.10.2022

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
Adc *Adc_Mcu::adc1 = 0;
Adc *Adc_Mcu::adc2 = 0;
Adc *Adc_Mcu::adc3 = 0;

//-------------------------------------------------------------------
Adc_Mcu::Adc_Mcu( AdcId adcId, Timer &timer )

: Adc( ReportID_Hw::Module::ADC_MCU, 16 /*num of channels*/, timer )

{
  id = adcId;

  switch( id )
  {
    case ADC_1: 
      RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
      adc1   = this;
      adcPtr = ADC1;
      break;

    case ADC_2: 
      RCC->APB2ENR |= RCC_APB2ENR_ADC2EN;
      adc2 = this; 
      adcPtr = ADC2;
      break;

    case ADC_3:
      RCC->APB2ENR |= RCC_APB2ENR_ADC3EN;
      adc3 = this;
      adcPtr = ADC3;
      break;

    default:
      report.error( ReportID_Hw::Event::WRONG_ID );
      break;
  }

  ADC->CCR |= (3<<16); // ADC prescaler

  adcPtr->CR2 =    ADC_CR2_ADON  // A/D Converter: ON
                | !ADC_CR2_CONT  // Continuous Conversion: OFF
                |  ADC_CR2_ALIGN;// Data alignment: left

  adcPtr->CR1 =    ADC_CR1_EOCIE; // End of conversion interrupt: enable

  NVIC_EnableIRQ( ADC_IRQn );
}

//-------------------------------------------------------------------
void Adc_Mcu::configCh( BYTE ch, BYTE para )
{
  DWORD paraLoc = para & 0x07; // see: Channel-wise programmable sampling time

  switch( id )
  {
    case ADC_1:
    case ADC_2:
      switch( ch )
      {
        case  0: PinConfig::set( PinConfig::ADC123_IN0,  PinConfig::ANALOG ); adcPtr->SMPR2 |= (paraLoc<<  0); break;
        case  1: PinConfig::set( PinConfig::ADC123_IN1,  PinConfig::ANALOG ); adcPtr->SMPR2 |= (paraLoc<<  3); break;
        case  2: PinConfig::set( PinConfig::ADC123_IN2,  PinConfig::ANALOG ); adcPtr->SMPR2 |= (paraLoc<<  6); break;
        case  3: PinConfig::set( PinConfig::ADC123_IN3,  PinConfig::ANALOG ); adcPtr->SMPR2 |= (paraLoc<<  9); break;
        case  4: PinConfig::set( PinConfig::ADC12_IN4,   PinConfig::ANALOG ); adcPtr->SMPR2 |= (paraLoc<< 12); break;
        case  5: PinConfig::set( PinConfig::ADC12_IN5,   PinConfig::ANALOG ); adcPtr->SMPR2 |= (paraLoc<< 15); break;
        case  6: PinConfig::set( PinConfig::ADC12_IN6,   PinConfig::ANALOG ); adcPtr->SMPR2 |= (paraLoc<< 18); break;
        case  7: PinConfig::set( PinConfig::ADC12_IN7,   PinConfig::ANALOG ); adcPtr->SMPR2 |= (paraLoc<< 21); break;
        case  8: PinConfig::set( PinConfig::ADC12_IN8,   PinConfig::ANALOG ); adcPtr->SMPR2 |= (paraLoc<< 24); break;
        case  9: PinConfig::set( PinConfig::ADC12_IN9,   PinConfig::ANALOG ); adcPtr->SMPR2 |= (paraLoc<< 27); break;
        case 10: PinConfig::set( PinConfig::ADC123_IN10, PinConfig::ANALOG ); adcPtr->SMPR1 |= (paraLoc<<  0); break;
        case 11: PinConfig::set( PinConfig::ADC123_IN11, PinConfig::ANALOG ); adcPtr->SMPR1 |= (paraLoc<<  3); break;
        case 12: PinConfig::set( PinConfig::ADC123_IN12, PinConfig::ANALOG ); adcPtr->SMPR1 |= (paraLoc<<  6); break;
        case 13: PinConfig::set( PinConfig::ADC123_IN13, PinConfig::ANALOG ); adcPtr->SMPR1 |= (paraLoc<<  9); break;
        case 14: PinConfig::set( PinConfig::ADC12_IN14,  PinConfig::ANALOG ); adcPtr->SMPR1 |= (paraLoc<< 12); break;
        case 15: PinConfig::set( PinConfig::ADC12_IN15,  PinConfig::ANALOG ); adcPtr->SMPR1 |= (paraLoc<< 15); break;
        default:
          report.error( ReportID_Hw::Event::WRONG_CHANNEL );
          break;
      }
      break;
 
    case ADC_3:
      switch( ch )
      {
        case  0: PinConfig::set( PinConfig::ADC123_IN0,  PinConfig::ANALOG ); adcPtr->SMPR2 |= (paraLoc<<  0); break;
        case  1: PinConfig::set( PinConfig::ADC123_IN1,  PinConfig::ANALOG ); adcPtr->SMPR2 |= (paraLoc<<  3); break;
        case  2: PinConfig::set( PinConfig::ADC123_IN2,  PinConfig::ANALOG ); adcPtr->SMPR2 |= (paraLoc<<  6); break;
        case  3: PinConfig::set( PinConfig::ADC123_IN3,  PinConfig::ANALOG ); adcPtr->SMPR2 |= (paraLoc<<  9); break;
        case  4: PinConfig::set( PinConfig::ADC3_IN4,    PinConfig::ANALOG ); adcPtr->SMPR2 |= (paraLoc<< 12); break;
        case  5: PinConfig::set( PinConfig::ADC3_IN5,    PinConfig::ANALOG ); adcPtr->SMPR2 |= (paraLoc<< 15); break;
        case  6: PinConfig::set( PinConfig::ADC3_IN6,    PinConfig::ANALOG ); adcPtr->SMPR2 |= (paraLoc<< 18); break;
        case  7: PinConfig::set( PinConfig::ADC3_IN7,    PinConfig::ANALOG ); adcPtr->SMPR2 |= (paraLoc<< 21); break;
        case  8: PinConfig::set( PinConfig::ADC3_IN8,    PinConfig::ANALOG ); adcPtr->SMPR2 |= (paraLoc<< 24); break;
        case  9: PinConfig::set( PinConfig::ADC3_IN9,    PinConfig::ANALOG ); adcPtr->SMPR2 |= (paraLoc<< 27); break;
        case 10: PinConfig::set( PinConfig::ADC123_IN10, PinConfig::ANALOG ); adcPtr->SMPR1 |= (paraLoc<<  0); break;
        case 11: PinConfig::set( PinConfig::ADC123_IN11, PinConfig::ANALOG ); adcPtr->SMPR1 |= (paraLoc<<  3); break;
        case 12: PinConfig::set( PinConfig::ADC123_IN12, PinConfig::ANALOG ); adcPtr->SMPR1 |= (paraLoc<<  6); break;
        case 13: PinConfig::set( PinConfig::ADC123_IN13, PinConfig::ANALOG ); adcPtr->SMPR1 |= (paraLoc<<  9); break;
        case 14: PinConfig::set( PinConfig::ADC3_IN14,   PinConfig::ANALOG ); adcPtr->SMPR1 |= (paraLoc<< 12); break;
        case 15: PinConfig::set( PinConfig::ADC3_IN15,   PinConfig::ANALOG ); adcPtr->SMPR1 |= (paraLoc<< 15); break;

        default:
          report.error( ReportID_Hw::Event::WRONG_CHANNEL );
          break;
      }
      break;

    default:
      report.error( ReportID_Hw::Event::WRONG_ID );
      break;
  }
}

//-------------------------------------------------------------------
inline WORD Adc_Mcu::getResult( void )
{
  return( adcPtr->DR & 0xFFFF );
}

//-------------------------------------------------------------------
inline void Adc_Mcu::startCh( BYTE ch )
{
  // select channel
  adcPtr->SQR3 = ch & 0x1F;

  adcPtr->CR2 |= ADC_CR2_SWSTART;
}

//-------------------------------------------------------------------
extern "C"
{
  void ADC_IRQHandler(void)
  {
    if( ADC1->SR & ADC_SR_EOC )
    {
      ADC1->SR &= ~ADC_SR_EOC;
      Adc_Mcu::adc1->isr();
    }
    if( ADC2->SR & ADC_SR_EOC )
    {
      ADC2->SR &= ~ADC_SR_EOC;
      Adc_Mcu::adc2->isr();
    }
    if( ADC3->SR & ADC_SR_EOC )
    {
      ADC3->SR &= ~ADC_SR_EOC;
      Adc_Mcu::adc3->isr();
    }
  }
}

}  } //namespace

//EOF
