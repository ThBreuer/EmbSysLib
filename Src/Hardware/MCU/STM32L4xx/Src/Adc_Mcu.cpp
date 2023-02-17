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
Adc *Adc_Mcu::adcPtr = 0;

//-------------------------------------------------------------------
Adc_Mcu::Adc_Mcu( Timer &timer )

: Adc( ReportID_Hw::Module::ADC_MCU, 17 /*num of channels*/, timer )

{
  adcPtr = this;

  RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;
  ADC1_COMMON->CCR |= 2<<16;
  
  ADC1->CR  = !ADC_CR_DEEPPWD;  
  ADC1->CR |=  ADC_CR_ADVREGEN; 
  
  System::delayMicroSec( 100 );
  
  // <<< ADC calibration 
  ADC1->CR |= ADC_CR_ADCAL;         // start calibration
  while( ADC1->CR & ADC_CR_ADCAL ); // wait for end of calibration
  // >>>
  
  ADC1->ISR |= ADC_ISR_ADRDY;
  
  ADC1->SQR1 = 0; // = number of channels - 1

  ADC1->CFGR |= ADC_CFGR_ALIGN; // left alignment
  
  ADC1->CR |=  ADC_CR_ADEN;  // A/D Converter: ON

  while( !(ADC1->ISR & ADC_ISR_ADRDY) );

  ADC1->IER |=    ADC_IER_EOCIE; // End of conversion interrupt: enable

  NVIC_EnableIRQ( ADC1_IRQn );
}

//-------------------------------------------------------------------
void Adc_Mcu::configCh( BYTE ch, BYTE para )
{
  DWORD paraLoc = para & 0x07; // see: Channel-wise programmable sampling time
  
  switch( ch )
  {
    default:
      report.error( ReportID_Hw::Event::WRONG_CHANNEL );
      break;
    case  5: PinConfig::set( PinConfig::ADC1_IN5,  PinConfig::ANALOG ); ADC1->SMPR1 |= (paraLoc<< 15); break;
    case  6: PinConfig::set( PinConfig::ADC1_IN6,  PinConfig::ANALOG ); ADC1->SMPR1 |= (paraLoc<< 18);  break;
    case  7: PinConfig::set( PinConfig::ADC1_IN7,  PinConfig::ANALOG ); ADC1->SMPR1 |= (paraLoc<< 21);  break;
    case  8: PinConfig::set( PinConfig::ADC1_IN8,  PinConfig::ANALOG ); ADC1->SMPR1 |= (paraLoc<< 24);  break;
    case  9: PinConfig::set( PinConfig::ADC1_IN9,  PinConfig::ANALOG ); ADC1->SMPR1 |= (paraLoc<< 27);  break;
    case 10: PinConfig::set( PinConfig::ADC1_IN10, PinConfig::ANALOG ); ADC1->SMPR2 |= (paraLoc<<  0);  break;
    case 11: PinConfig::set( PinConfig::ADC1_IN11, PinConfig::ANALOG ); ADC1->SMPR2 |= (paraLoc<<  3);  break;
    case 12: PinConfig::set( PinConfig::ADC1_IN12, PinConfig::ANALOG ); ADC1->SMPR2 |= (paraLoc<<  6);  break;
    case 15: PinConfig::set( PinConfig::ADC1_IN15, PinConfig::ANALOG ); ADC1->SMPR2 |= (paraLoc<< 15);  break;
    case 16: PinConfig::set( PinConfig::ADC1_IN16, PinConfig::ANALOG ); ADC1->SMPR2 |= (paraLoc<< 18);  break;
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
  ADC1->SQR1 &= ~(0x1F<<6);
  ADC1->SQR1 |= (ch & 0x1F)<<6;

  ADC1->CR |= ADC_CR_ADSTART;
}

//-------------------------------------------------------------------
extern "C"
{
  void ADC1_IRQHandler(void)
  {
    ADC1->ISR |= ADC_ISR_EOC;
    Adc_Mcu::adcPtr->isr();
  }
}

}  } //namespace

//EOF
