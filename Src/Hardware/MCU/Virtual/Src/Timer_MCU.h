//*******************************************************************
/*!
\file   Timer_Mcu.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.02.2012

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_TIMER_MCU_H
#define _HW_TIMER_MCU_H

//*******************************************************************
#include "Hardware/Common/Timer/Timer.h"
#include <windows.h>

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class Timer_Mcu

\brief Implementation of a timer

\example HwTimer.cpp
*/
class Timer_Mcu : public Timer
{
  public:
    //---------------------------------------------------------------
    /*! Initialize timer 0
        \param cycleTime  Cycle time in microseconds (us)
    */
    Timer_Mcu( DWORD cycleTime );

    //---------------------------------------------------------------
    ~Timer_Mcu( void );

    //---------------------------------------------------------------
    virtual void enablePWM( BYTE ch, Mode pol = NORMAL );

    //---------------------------------------------------------------
    virtual void setPWM( BYTE ch, WORD value );

  private:
    //---------------------------------------------------------------
    static DWORD loop( Timer_Mcu *ptr );

    //---------------------------------------------------------------
    static long long GetSysTimeMicroSec( void );

  private:
    //---------------------------------------------------------------
    DWORD   threadID;
    HANDLE  threadHandle;

}; //class Timer_Mcu

}  } //namespace

#endif
