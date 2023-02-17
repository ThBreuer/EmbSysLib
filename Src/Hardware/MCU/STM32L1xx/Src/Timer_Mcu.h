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

\brief Implementation of timer 2/3/4/5, 6/7, 9,10,11

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
      TIM_2,   //!< Timer 2
      TIM_3,   //!< Timer 3
      TIM_4,   //!< Timer 4
      TIM_5,   //!< Timer 5
      TIM_6,   //!< Timer 6
      TIM_7,   //!< Timer 7
      TIM_9,   //!< Timer 9
      TIM_10,  //!< Timer 10
      TIM_11   //!< Timer 11
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
    static Timer_Mcu *timerPtr_2;
    static Timer_Mcu *timerPtr_3;
    static Timer_Mcu *timerPtr_4;
    static Timer_Mcu *timerPtr_5;
    static Timer_Mcu *timerPtr_6;
    static Timer_Mcu *timerPtr_7;
    static Timer_Mcu *timerPtr_9;
    static Timer_Mcu *timerPtr_10;
    static Timer_Mcu *timerPtr_11;

  private:
    //---------------------------------------------------------------
    DWORD        tics;
    TimerId      timerId;
    TIM_TypeDef *ptr;

}; //class Timer_Mcu

}  } //namespace

#endif
