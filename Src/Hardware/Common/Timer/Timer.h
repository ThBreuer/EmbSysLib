//*******************************************************************
/*!
\file   Timer.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_TIMER_H
#define _HW_TIMER_H

//*******************************************************************
#include "Mcu_Types.h"
#include "Std/Report.h"
#include "Std/Sequence.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class Timer

\brief Abstract class supporting timer hardware

The timer is used to generate PWM signals and/or periodical
interrupts.

This base class contains an update-list, to which objects can be
added if derived from \a Timer::Task. The update()-methods of all
added objects are successively called by each timer interrupt.

\example HwTimer.cpp
*/
class Timer
{
  public:
    //---------------------------------------------------------------
    /*! \enum Mode
        \brief Define the operating modes of PWM generation
    */
    typedef enum
    {
      NORMAL  = 0x00, //!< Output polarity: active high
      INVERS  = 0x01  //!< Output polarity: active low
    } Mode;

  public:
    //*******************************************************************
    /*!
    \class Task

    \brief Base class for timer interrupt tasks
    */
    class Task : public Std::Sequence<Task>::Item
    {
      public:
        //---------------------------------------------------------
        /*! Method called by timer interrupt.
            The application has to override this method.
        */
        virtual void update( void ) = 0;

    }; //Timer::Task
    //*******************************************************************

  protected:
    //---------------------------------------------------------------
    // Constructor
    // moduleId: Id of this module, used for report
    //
    Timer( WORD moduleId );

  public:
    //---------------------------------------------------------------
    /*! Get cycle time of timer interrupt
        \return Time between interrupts, unit: micro seconds (us)
    */
    virtual DWORD getCycleTime( void );

    //---------------------------------------------------------------
    /*! Add a new object to the update-list.
        After executing this method, the objects update()-method
        will be called periodically by timer interrupt.
        \remark The update()-method of the Timer::Task may be called immediately, so call this add()-method only when the constructor of the Task-object is completed.
        \param ptr Pointer to the Timer::Task object
    */
    virtual void add( Task *ptr );

    //---------------------------------------------------------------
    /*! Enable a PWM-Channel, if PWM is supported by the timer
        \param channel  Channel identification (0,...)
        \param polarity PWM output polarity
    */
    virtual void enablePWM( BYTE channel,
                            Mode polarity = NORMAL) = 0;

    //---------------------------------------------------------------
    /*! Set the PWM duty cycle, if PWM is enabled
        \param channel Channel ID (0,...)
        \param value   Duty cycle, normalized to 16 bit range
    */
    virtual void setPWM( BYTE channel,
                         WORD value ) = 0;

  protected:
    //---------------------------------------------------------------
    // Interrupt Service Routine of timer interrupt
    // This method has to been called by the hardware driven
    // interrupt service routine only
    //
    virtual void isrHandler( void );

  protected:
    //---------------------------------------------------------------
    DWORD cycleTime; // cycle time in milisec

  private:
    //---------------------------------------------------------------
    Std::Sequence<Task> isrSeq; // list of callable objects

  public:
    //---------------------------------------------------------------
    Std::Report report;

}; //class Timer

}  } //namespace

#endif
