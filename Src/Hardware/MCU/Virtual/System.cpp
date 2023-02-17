//*******************************************************************
/*!
\file   System.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"

\brief  Controller specific implementation of class cSystem
*/

//*******************************************************************
#include <windows.h>
#include <stdio.h>
#include <ctype.h>

//*******************************************************************
static CRITICAL_SECTION _critSection;

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
void SystemInit( void )
{
}

//*******************************************************************
long int GetSysTimeMicroSec( void )
{
  LARGE_INTEGER ticksPerSecond;
  LARGE_INTEGER tick;
  long long      time;

  QueryPerformanceFrequency(&ticksPerSecond);
  QueryPerformanceCounter(&tick);

  time = ((tick.QuadPart/*%1000 000 000*/)*1000000)/ticksPerSecond.QuadPart;

  return(time);
}

//*******************************************************************
long int GetSysTimeMilliSec( void )
{
  LARGE_INTEGER ticksPerSecond;
  LARGE_INTEGER tick;
  long long      time;

  QueryPerformanceFrequency(&ticksPerSecond);
  QueryPerformanceCounter(&tick);

  time = ((tick.QuadPart/*%1000 000 000*/)*1000)/ticksPerSecond.QuadPart;

  return(time);
}

}  } //namespace

//*******************************************************************
//*******************************************************************
namespace EmbSysLib {
namespace Hw {
//*******************************************************************
//
// System
//
//*******************************************************************
//-------------------------------------------------------------------
static long int timeLast = 0;
static System sys;
static bool isInitialized = false;

//-------------------------------------------------------------------
System::System( bool disableInterrupts )
{
//  InitializeCriticalSection (& _critSection);
  SystemInit();

  disableWatchdog();

  timeLast = GetSysTimeMicroSec();

  if ( disableInterrupts )
  {
    disableInterrupt();
  }
}

//-------------------------------------------------------------------
void System::start( void )
{
  enableInterrupt();
}

//-------------------------------------------------------------------
void System::disableInterrupt( void )
{
  if( !isInitialized )
  {
      InitializeCriticalSection (& _critSection);
      isInitialized = true;
  }
  EnterCriticalSection (& _critSection);
}

//-------------------------------------------------------------------
void System::enableInterrupt( void )
{
  LeaveCriticalSection (& _critSection);
}

//-------------------------------------------------------------------
void System::enableWatchdog( WORD resetTime )
{
  //<! \remark watchdog isn't implemented yet
}

//-------------------------------------------------------------------
void System::disableWatchdog( void )
{
  //<! \remark watchdog isn't implemented yet
}

//-------------------------------------------------------------------
void System::feedWatchdog( void )
{
  //<! \remark watchdog isn't implemented yet
}

//-------------------------------------------------------------------
void System::reset( void )
{
  exit( 1 );
}

//-------------------------------------------------------------------
void System::delayMicroSec( unsigned short delay )
{
  long int now = GetSysTimeMicroSec();

  while( GetSysTimeMicroSec() < now + delay )
  {
  }
}

//-------------------------------------------------------------------
void System::delayMilliSec( unsigned short delay )
{
  Sleep( delay );
}

}  } //namespace
