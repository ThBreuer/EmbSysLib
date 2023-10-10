//*******************************************************************
/*!
\file   Adc_Mcu.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   30.04.2023

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

: Adc( ReportID_Hw::Module::ADC_MCU, 12 /*num of channels*/, timer )

{
  adcPtr = this;

  // ADC Multiplexer Register
  ADMUX  =  (0x01<<REFS0)  // Reference selection bits:
                           //   AVCC with external capacitor at AREF pin
           |(   0<<ADLAR); // ADC left adjust result

  // ADC Control and Status Register A
  ADCSRA =  (   1<<ADEN ) // ADC enable
           |(   0<<ADSC ) // ADC start conversion
           |(   0<<ADATE) // ADC auto trigger enable
           |(   1<<ADIE ) // Interrupt enable
           |(0x07<<ADPS0);// ADC prescaler selections:
                          //   Division factor = 128
                          //   ADC-Clock = XTAL/Division factor
}

//-------------------------------------------------------------------
WORD Adc_Mcu::getResult( void )
{
  return( ADC<<6 );
}

//-------------------------------------------------------------------
void Adc_Mcu::configCh( BYTE ch, BYTE para )
{
  switch( ch )
  {
    case  0: PinConfig::set( &PINF, 0, PinConfig::INPUT ); break;
    case  1: PinConfig::set( &PINF, 1, PinConfig::INPUT ); break;
    // ch 2 and 4 not connected
    case  4: PinConfig::set( &PINF, 4, PinConfig::INPUT ); break;
    case  5: PinConfig::set( &PINF, 5, PinConfig::INPUT ); break;
    case  6: PinConfig::set( &PINF, 6, PinConfig::INPUT ); break;
    case  7: PinConfig::set( &PINF, 7, PinConfig::INPUT ); break;
    
    case  8: PinConfig::set( &PIND, 4, PinConfig::INPUT ); break;
    case  9: PinConfig::set( &PIND, 6, PinConfig::INPUT ); break;
    case 10: PinConfig::set( &PIND, 7, PinConfig::INPUT ); break;

    case 11: PinConfig::set( &PINB, 4, PinConfig::INPUT ); break;
    case 12: PinConfig::set( &PINB, 5, PinConfig::INPUT ); break;
    case 13: PinConfig::set( &PINB, 6, PinConfig::INPUT ); break;
    default:
      report.error( ReportID_Hw::Event::WRONG_CHANNEL );
    break;
  }
}

//-------------------------------------------------------------------
void Adc_Mcu::startCh( BYTE ch )
{
  // ADC Multiplexer Register
  ADMUX   =  (ADMUX & 0xE0)
            |(ch);           // Single ended input channel

  // ADC Control and Status Register A
  ADCSRA |= (1<<ADSC);       // ADC start conversion
}

//-------------------------------------------------------------------
ISR( ADC_vect )
{
  // allow other interrupts
//  SREG |= (1<<7); // global interrupt enable

  System::enterISR();
  Adc_Mcu::adcPtr->isr();
  System::leaveISR();
}

}  } //namespace

//EOF
