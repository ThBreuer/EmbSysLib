//*******************************************************************
/*!
\file   Adc_Mcu.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   30.04.2023

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_ADC_MCU_H
#define _HW_ADC_MCU_H

//*******************************************************************
#include "Hardware/Common/Analog/Adc.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class Adc_Mcu

\brief Implementation of the analog to digital converter (ADC)

\example HwAdc.cpp
*/
class Adc_Mcu : public Adc
{
  public:
    //---------------------------------------------------------------
    /*! Initialize the ADC hardware
        \param timer Reference to a timer object for auto trigger conversion
    */
    Adc_Mcu( Timer &timer );

  private:
    //---------------------------------------------------------------
    virtual WORD getResult( void );

    //---------------------------------------------------------------
    virtual void startCh( BYTE ch );

    //---------------------------------------------------------------
    virtual void configCh( BYTE ch, BYTE para = 0 );

  public:
    //---------------------------------------------------------------
    static Adc *adcPtr;

}; //class Adc_Mcu

}  } //namespace

#endif
