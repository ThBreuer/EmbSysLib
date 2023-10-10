//*******************************************************************
/*!
\file   Timer_Mcu.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_TIMER_MCU_H
#define _HW_TIMER_MCU_H

//*******************************************************************
#include "Hardware/Common/Timer/Timer.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class Timer_Mcu

\brief Implementation of timer 2, 6/7

\example HwTimer.cpp
*/
class Timer_Mcu : public Timer
{
  public:
    //---------------------------------------------------------------
    /*! \enum TimerId
        \brief Timer identification
    */
    typedef enum
    {
      TIM_1,   //!< Timer 1
      TIM_2,   //!< Timer 2
      TIM_6,   //!< Timer 6
      TIM_7,   //!< Timer 7
    } TimerId;

    //---------------------------------------------------------------
    /*! \enum ChannelId
        \brief Channel identification
    */
    typedef enum
    {
      CH1 = 0,   //!< Channel 1
      CH2 = 1,   //!< Channel 2
      CH3 = 2,   //!< Channel 3
      CH4 = 3    //!< Channel 4
    } ChannelId;

  public:
    //---------------------------------------------------------------
    /*! Instantiate a timer
        \param timerId   Timer hardware selection
        \param cycleTime Interrupt cycle time in micro seconds (us)
    */
    Timer_Mcu( TimerId timerId, DWORD cycleTime );

    //---------------------------------------------------------------
    virtual void enablePWM( BYTE channel, Mode polarity = NORMAL );

    //---------------------------------------------------------------
    virtual void setPWM( BYTE channel, WORD value );

    //---------------------------------------------------------------
    // Interrupt Service Routine (ISR) of timer interrupt.
    // This function must be public, because it is called
    // in extern "C"
    void isr( void );

  public:
    //---------------------------------------------------------------
    static Timer_Mcu *timerPtr_1;
    static Timer_Mcu *timerPtr_2;
    static Timer_Mcu *timerPtr_6;
    static Timer_Mcu *timerPtr_7;

  private:
    //---------------------------------------------------------------
    DWORD        tics;
    TimerId      timerId;
    TIM_TypeDef *ptr;

}; //class Timer_Mcu

}  } //namespace

#endif
