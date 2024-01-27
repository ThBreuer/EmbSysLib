//*******************************************************************
/*!
\file   AnalogIn.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   18.12.2023

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _DEV_ANALOG_IN_H
#define _DEV_ANALOG_IN_H

//*******************************************************************
#include "Analog.h"

//*******************************************************************
namespace EmbSysLib {
namespace Dev {

//*******************************************************************
/*!
\class   AnalogIn

\brief Abstract class for analog input devices

Uses the conversion
 - x = gain*raw + min

with
 - gain = (max - min)/0xFFFF
*/
class AnalogIn : public Analog
{
  protected:
    //---------------------------------------------------------------
    // Instatiate an analog input device
    // channel: Device input channel
    // max:     Analog value corresponding to maximum raw value
    // min:     Analog value corresponding to minimum raw value
    AnalogIn( BYTE  channel,
              float max,
              float min = 0.0f );

  public:
    //---------------------------------------------------------------
    /*! Get the physical quantity
        \return Calculated ADC value
    */
    virtual float get( void );

    //---------------------------------------------------------------
    /*! Get the physical quantity by type conversion
        \return Calculated ADC value
    */
    virtual operator float( void );

    //---------------------------------------------------------------
    /*! Two point calibration
    */
    void calibrate( float x1,
                    float y1,
                    float x2,
                    float y2
                   );

    //---------------------------------------------------------------
    /*! Get the raw input value
        \return Raw value in range of [0,..,0xFFFF]
    */
    virtual WORD getRaw( void ) = 0;

    //---------------------------------------------------------------
    /*! \todo add comment
    */
    virtual void enable() = 0;

}; //class AnalogIn

} } //namespace

#endif
