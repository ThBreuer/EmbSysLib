//*******************************************************************
/*!
\file   DigitalEncoderJoystick.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   26.01.2023

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _CTRL_DIGITAL_ENCODER_JOYSTICK_H
#define _CTRL_DIGITAL_ENCODER_JOYSTICK_H

//*******************************************************************
#include "Control/DigitalEncoder.h"

//*******************************************************************
namespace EmbSysLib {
namespace Ctrl {

//*******************************************************************
/*!
\class DigitalEncoderJoystick

Implementation of a joystick type encoder.

The joystick consists of two directional keys, which can be pressed 
alternativley, and a central confirmation button.

\example CtrlDigitalEncoder.cpp
*/
class DigitalEncoderJoystick : public DigitalEncoder
{
  public:
    //---------------------------------------------------------------
    /*! Initialisiert joystic
        After the \b repeatTime has elapsed, the event is repeated 
        if the button is pressed continuously (0: no repetition).
        \param btnLeft     Pointer to left button
        \param btnRight    Pointer to right buttom
        \param btnCtrl     Pointer to confirmation button
        \param taskManager TaskManager for background tasks
        \param repeatTime  Duration of repeat [ms], (optional)
    */
	  DigitalEncoderJoystick( Digital     *btnLeft,
                            Digital     *btnRight,
                            Digital     *btnCtrl,
                            TaskManager &taskManager,
                            WORD         repeatTime = 0 );

  private:
    //---------------------------------------------------------------
 	  virtual Event getIncrement( void );

  private:
    //---------------------------------------------------------------
    const WORD  timerStart;
    Digital    *btnLeft;
    Digital    *btnRight;
    WORD        timerCnt;

}; //class DigitalEncoderJoystick

} } //namespace

#endif
