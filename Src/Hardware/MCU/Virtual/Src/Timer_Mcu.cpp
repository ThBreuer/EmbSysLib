//*******************************************************************
/*!
\file   Timer_Mcu.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.02.2012

License: See file "LICENSE"
*/

//*******************************************************************
#include "Timer_Mcu.h"
#include "Hardware/ReportID_Hw.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// Timer_Mcu
//
//*******************************************************************
//-------------------------------------------------------------------
Timer_Mcu::Timer_Mcu( DWORD cycleTimeIn )

: Timer( ReportID_Hw::Module::TIMER_MCU )

{
  cycleTime    = cycleTimeIn; // unit: us
  threadID     = 0;
  threadHandle = CreateThread( NULL,
                               10000,                        // stack size
                               (LPTHREAD_START_ROUTINE)loop, // thread function,
                               this,                         // argument to thread function
                               0,                            // creation flags
                               &threadID );
  if( threadHandle == NULL )
  {
    report.error( ReportID_Hw::Event::THREAD_NOT_CREATED );
  }
}

//-------------------------------------------------------------------
Timer_Mcu::~Timer_Mcu( void )
{
  TerminateThread( threadHandle, 0 );
  CloseHandle    ( threadHandle );
}

//-------------------------------------------------------------------
void Timer_Mcu::enablePWM( BYTE ch, Mode pol )
{
  report.alert( ReportID_Hw::Event::FUNCTION_NOT_SUPPORTED );
}

//-------------------------------------------------------------------
void Timer_Mcu::setPWM( BYTE ch, WORD value )
{
  report.alert( ReportID_Hw::Event::FUNCTION_NOT_SUPPORTED );
}

//-------------------------------------------------------------------
// static
DWORD Timer_Mcu::loop( Timer_Mcu *ptr )
{
  long long timeNow  = 0;
  long long timeNext = GetSysTimeMicroSec();

  // todo set thread prio with 'SetThreadPriority(GetCurrentThread(), THREAD_MODE_BACKGROUND_BEGIN)'

  while(1) // The task should never end
  {
    if( (timeNow = GetSysTimeMicroSec()) >= timeNext )
    {
      timeNext += ptr->cycleTime;
      ptr->isrHandler();
    }
    else if( ptr->cycleTime >= 2000 )
    {
      Sleep(1);
    }
  }
  return(0);
}

//-------------------------------------------------------------------
// static
long long Timer_Mcu::GetSysTimeMicroSec( void )
{
  LARGE_INTEGER ticksPerSecond;
  LARGE_INTEGER tick;
  long long     time;

  QueryPerformanceFrequency( &ticksPerSecond );
  QueryPerformanceCounter  ( &tick );

  time = (tick.QuadPart*1000000)/ticksPerSecond.QuadPart;

  return( time );
}

}  } //namespace

//EOF
