//*******************************************************************
/*!
\file   AalogOut.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   18.12.2023

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _DEV_ANALOG_OUT_H
#define _DEV_ANALOG_OUT_H

//*******************************************************************
#include "Analog.h"

//*******************************************************************
namespace EmbSysLib {
namespace Dev {

//*******************************************************************
/*!
\class AnalogOut

\brief Abstract class for analog output devices

Uses the conversion
 - raw = gain*(x - min)

with
 - gain = 0xFFFF/(max - min)
*/
class AnalogOut : public Analog
{
  protected:
    //---------------------------------------------------------------
    // Instatiate a analog output device
    // channel: Device output channel
    // max:     Analog value corresponding to maximum raw value
    // min:     Analog value corresponding to minimum raw value
    AnalogOut( BYTE  channel,
               float max,
               float min = 0.0f );

  public:
    //---------------------------------------------------------------
    /*! Set the physical quantity
        \param value Value to set
    */
    virtual void set( float value );

    //---------------------------------------------------------------
    /*! Set the physical quantity
        \param value Value to set
        \return value
    */
    virtual float operator=( float value );

    //---------------------------------------------------------------
    /*! Set the raw output value [0,..,0xFFFF]
        \param value Value to set
    */
    virtual void setRaw( WORD value ) = 0;

}; //class AnalogOut

} } //namespace

#endif
