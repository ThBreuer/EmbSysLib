//*******************************************************************
/*!
\file   AnalogOutPWM.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   18.12.2023

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _DEV_ANALOG_OUT_PWM_H
#define _DEV_ANALOG_OUT_PWM_H

//*******************************************************************
#include "AnalogOut.h"
//#include "Device/Digital/Digital.h"
#include "Hardware/Common/Timer/Timer.h"

//*******************************************************************
namespace EmbSysLib {
namespace Dev {

//*******************************************************************
/*!
\class AnalogOutPWM

\brief Analog output device (PWM), using cHwTimer

\example Analog.cpp
*/
class AnalogOutPWM : public AnalogOut
{
  public:
    //---------------------------------------------------------------
    /*! Instatiate a PWM output device
        \param timer   Reference to timer hardware
        \param mode    Operating mode of timer
        \param channel Channel ID
        \param max     Analog value corresponding to maximum raw value
        \param min     Analog value corresponding to minimum raw value
    */
    AnalogOutPWM( Timer       &timer,
                  Timer::Mode  mode    = Timer::NORMAL,
                  BYTE         channel = 0,
                  float        max     = 1.0f,
                  float        min     = 0.0f );

  public:
    //---------------------------------------------------------------
    /*! Set the raw output value [0,..,0xFFFF]
        \param value Value to set
    */
    virtual void setRaw( WORD value );

  private:
    //---------------------------------------------------------------
    Timer &timer;

  public:
    //---------------------------------------------------------------
    using AnalogOut::operator=;

}; //class AnalogOutPWM

} } //namespace

#endif
