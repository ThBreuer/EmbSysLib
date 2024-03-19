//*******************************************************************
/*!
\file   DigitalEncoder.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   26.01.2023

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _CTRL_DIGITAL_ENCODER_H
#define _CTRL_DIGITAL_ENCODER_H

//*******************************************************************
#include "Device/Task/TaskManager.h"
#include "Device/Digital/Digital.h"

//*******************************************************************
namespace EmbSysLib {
namespace Ctrl {

//*******************************************************************
/*!
\class   DigitalEncoder

Abstract class for encoder (user interface)

An encoder is a direction-sensitive incremental input device
(e.g. joystick, rotary knob). In addition, a central
confirmation button must be present. A DigitalEncoder object evaluates the
connected hardware and, if necessary, writes the event to a FIFO.
The FIFO can be read out asynchronously.

\example CtrlDigitalEncoder.cpp
*/
class DigitalEncoder : protected TaskManager::Task
{
  public:
    //---------------------------------------------------------------
    /*! \enum Event
        \brief Encoder event
    */
    typedef enum
    {
      NONE = 0, //!< No event
      LEFT,     //!< Left oriented operation
      RIGHT,    //!< Right oriented operation
      CTRL_DWN, //!< Activation of control/confirmation button
      CTRL_UP   //!< Release of control/confirmation button
    } Event;

  protected:
    //---------------------------------------------------------------
    // Konstruktor
    //
    // timer: HW-Timer, der update()-Mehode periodisch aufruft (optional)
    //
    DigitalEncoder( Digital     *btnCtrl,
                    TaskManager &taskManager );

  public:
    //---------------------------------------------------------------
    /*! Return encoder event
        \return Event
    */
    Event getEvent( void );

  protected:
    //---------------------------------------------------------------
    // update method called periodically by timer.
    // Checks the states of the encoder (e.g. joystick) and carries
    // event into FIFO
    virtual void update( void );

    //---------------------------------------------------------------
    // The filling of the FIFO is done in the superclass DigitalEncoder.
    // The derived class delivers the events with the
    // following get...() methods only.
    // Timing: Both get...() methods are called once in update()
    //
    //---------------------------------------------------------------
    // Checks Ctrl-Button.
    // The default implementation returns the event CTRL_DWN or
    // CTRL_UP if the button was pressed/released, otherwise NONE.
    // The method can be overridden.
    //
    virtual Event getCtrl( void );

    //---------------------------------------------------------------
    // Checks Increment (Right/Left).
    // Returns LEFT, RIGHT, NONE depending on the event.
    // Abstract method that must be implemented by derived class.
    virtual Event getIncrement( void ) = 0;

  private:
    //---------------------------------------------------------------
    Digital     *btnCtrl;
    Fifo<Event>  fifo;

}; //class DigitalEncoder

} } //namespace

#endif
