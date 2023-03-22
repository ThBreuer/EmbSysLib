//*******************************************************************
/*!
\file   Adc_Mcu.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

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

\brief Implementation of the analg to digital converter (ADC)

\example HwAdc.cpp
*/
class Adc_Mcu : public Adc
{
  public:
    //---------------------------------------------------------------
    /*! \enum AdcId
        \brief ADC identification
    */
    typedef enum
    {
      ADC_1 = 1, //!< ADC 1
      ADC_2,     //!< ADC 2
      ADC_3      //!< ADC 3
    } AdcId;

  public:
    //---------------------------------------------------------------
    /*! \enum Para
        \brief ...
        \todo comment and/or redesign
    */
      typedef enum
      {
          SAMPLING_TIME_3 = 0,
          SAMPLING_TIME_15,
          SAMPLING_TIME_28,
          SAMPLING_TIME_56,
          SAMPLING_TIME_84,
          SAMPLING_TIME_112,
          SAMPLING_TIME_144,
          SAMPLING_TIME_480
      } Para;

  public:
    //---------------------------------------------------------------
    /*! Initialize the ADC hardware
        \param adcId Used ADC unit
        \param timer Reference to a timer object for auto trigger conversion

    */
    Adc_Mcu( AdcId adcId, Timer &timer );

  private:
    //---------------------------------------------------------------
    virtual WORD getResult( void );

    //---------------------------------------------------------------
    virtual void startCh( BYTE ch );

    //---------------------------------------------------------------
    virtual void configCh( BYTE ch, BYTE para = 0 );

  public:
    //---------------------------------------------------------------
    static Adc *adc1;
    static Adc *adc2;
    static Adc *adc3;
  
  private:
    //---------------------------------------------------------------
    AdcId         id;
    ADC_TypeDef  *adcPtr;
    
}; //class Adc_Mcu

}  } //namespace

#endif
