//*******************************************************************
/*!
\file   AnalogInAdc.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   18.12.2023

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _DEV_ANALOG_IN_ADC_H
#define _DEV_ANALOG_IN_ADC_H

//*******************************************************************
#include "AnalogIn.h"
#include "Hardware/Common/Analog/Adc.h"

//*******************************************************************
namespace EmbSysLib {
namespace Dev {

//*******************************************************************
/*!
\class AnalogInAdc

\brief Analog input device, using Hardware/Adc

\example DevAnalog.cpp
*/
class AnalogInAdc : public AnalogIn
{
  public:
    //---------------------------------------------------------------
    /*! Instatiate a analog input device
        \param adc     Reference to ADC hardware
        \param channel Channel ID
        \param max     Analog value corresponding to maximum raw value
        \param min     Analog value corresponding to minimum raw value
    */
    AnalogInAdc( Adc    &adc,
                 BYTE    channel = 0,
                 float   max     = 1.0,
                 float   min     = 0.0,
                 BYTE    para    = 0 );

  public:
    //---------------------------------------------------------------
    /*! Get the raw input value
        \return Raw value in range of [0,..,0xFFFF]
    */
    virtual WORD getRaw( void );

    //---------------------------------------------------------------
    /*! \todo add comment
    */
    virtual void enable();

  private:
    //---------------------------------------------------------------
    Adc  &adc;
    BYTE  para;

}; //class AnalogInAdc

} } //namespace

#endif
