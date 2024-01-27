//*******************************************************************
/*!
\file   TaskManager.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   31.01.2023

License: See file "LICENSE"
*/

//*******************************************************************
#include "TaskManager.h"

//*******************************************************************
namespace EmbSysLib {
namespace Dev {

//*******************************************************************
//
// TaskManager
//
//*******************************************************************
//-------------------------------------------------------------------
//
// nothing to do ...
//

//*******************************************************************
//
// TaskManager::Clock
//
//*******************************************************************
//-------------------------------------------------------------------
TaskManager::Clock::Clock( TaskManager &taskManager,
                           DWORD       timeToWait_msec,
                           WORD        timeToWait_usec )

: taskManager( taskManager )

{
  start( timeToWait_msec, timeToWait_usec ); // Don't use cTimer constructor
}

//-------------------------------------------------------------------
LWORD TaskManager::Clock::getTics( void )
{
  return( taskManager.getTics() );
}

//-------------------------------------------------------------------
DWORD TaskManager::Clock::getTimeOfTic( void )
{
  return( taskManager.getCycleTime() );
}

} } //namespace

//EOF
