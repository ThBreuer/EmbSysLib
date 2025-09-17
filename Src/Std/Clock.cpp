//*******************************************************************
/*!
\file   Clock.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   27.01.2024

License: See file "LICENSE"
*/

//*******************************************************************
#include "Clock.h"

//*******************************************************************
namespace EmbSysLib {
namespace Std {

//*******************************************************************
//
// Clock
//
//*******************************************************************
//-------------------------------------------------------------------
Clock::Clock( void )
{
  timeToGo  = 0;
  startTime = 0;
  running   = false;
}

//-------------------------------------------------------------------
void Clock::start( DWORD timeToWait_msec,
                   WORD  timeToWait_usec )
{
  timeToGo  = toTics( timeToWait_msec, timeToWait_usec );
  start();
}

//-------------------------------------------------------------------
void Clock::start( void )
{
  startTime = getTics();
  running   = (bool)timeToGo;
}

//-------------------------------------------------------------------
bool Clock::trigger( void )
{
  if( running )
  {
    if(getTics() - startTime >= timeToGo )
    {
      start();
      return( true );
    }
  }
  return( false );
}

//-------------------------------------------------------------------
bool Clock::timeout( void )
{
  if( running )
  {
    if(getTics() - startTime >= timeToGo )
    {
      stop();
      return( true );
    }
  }
  return( false );
}

//-------------------------------------------------------------------
void Clock::stop( void )
{
  running = false;
}

//-------------------------------------------------------------------
LWORD Clock::get( void )
{
  return( (getTics() - startTime ) * getTimeOfTic() );
}

//-------------------------------------------------------------------
LWORD Clock::toTics( DWORD timeToWait_msec,
                     WORD  timeToWait_usec )
{
  return( ( (LWORD)timeToWait_msec*1000L + timeToWait_usec )
          / getTimeOfTic() );
}

}  } //namespace

// EOF
