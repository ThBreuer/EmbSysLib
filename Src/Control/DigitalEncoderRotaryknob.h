//*******************************************************************
/*!
\file   DigitalEncoderRotaryknob.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   26.01.2023

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _CTRL_DIGITAL_ENCODER_ROTARYKNOB_H
#define _CTRL_DIGITAL_ENCODER_ROTARYKNOB_H

//*******************************************************************
#include "Control/DigitalEncoder.h"

//*******************************************************************
namespace EmbSysLib {
namespace Ctrl {

//*******************************************************************
/*!
\class   DigitalEncoderRotaryknob

Implementation of a rotary knob type encoder.

The rotary knob consists of two keys, which are actuated with a time
time-delayed during a rotation, as well as a central confirmation key.

\example CtrlDigitalEncoder.cpp
*/
class DigitalEncoderRotaryknob : public DigitalEncoder
{
  public:
    //---------------------------------------------------------------
    /*! Instantiate a rotary knob
        \param switchA     Pointer to encoder channel A
        \param switchB     Pointer to encoder channel B
        \param btnCtrl     Pointer to confirmation button
        \param taskManager TaskManager for background tasks
    */
    DigitalEncoderRotaryknob( Digital     *switchA,
                              Digital     *switchB,
                              Digital     *btnCtrl,
                              TaskManager &taskManager );

  private:
    //---------------------------------------------------------------
    virtual Event getIncrement( void );

  private:
    //---------------------------------------------------------------
    Digital *switchA;
    Digital *switchB;

}; //class DigitalEncoderRotaryknob

} } //namespace

#endif
