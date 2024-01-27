//*******************************************************************
/*!
\file   AnalogOutDac.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   18.12.2023

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _DEV_ANALOG_OUT_DAC_H
#define _DEV_ANALOG_OUT_DAC_H

//*******************************************************************
#include "AnalogOut.h"
#include "Hardware/Common/Analog/Dac.h"

//*******************************************************************
namespace EmbSysLib {
namespace Dev {

//*******************************************************************
/*!
\class AnalogOutDac

\brief Analog input device, using Hardware/Dac

\example Analog.cpp
*/
class AnalogOutDac : public AnalogOut
{
  public:
    //---------------------------------------------------------------
    /*! Instatiate a analog output device
        \param dac     Reference to DAC hardware
        \param channel Channel ID
        \param max     Analog value corresponding to maximum raw value
        \param min     Analog value corresponding to minimum raw value
    */
    AnalogOutDac( Dac    &dac,
                  BYTE    channel = 0,
                  float   max     = 1.0f,
                  float   min     = 0.0f );

  public:
    //---------------------------------------------------------------
    /*! Set the raw output value [0,..,0xFFFF]
        \param value Value to set
    */
    virtual void setRaw( WORD value );

  private:
    //---------------------------------------------------------------
    Dac &dac;

  public:
    //---------------------------------------------------------------
    using AnalogOut::operator=;

}; //class AnalogOutDac

} } //namespace

#endif
