//*******************************************************************
/*!
\file   Rtos.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   13.03.2023

License: See file "LICENSE"
*/

//*******************************************************************
#include "Rtos.h"
#include "Std/Report.h"
#include "Module/ReportID_Mod.h"

//*******************************************************************
namespace EmbSysLib {
namespace Mod {

using namespace Std;

//*******************************************************************
//
// Rtos
//
//*******************************************************************
//-------------------------------------------------------------------
Rtos *Rtos::ptr = NULL;

//*******************************************************************
//
// Rtos::Clock
//
//*******************************************************************
//-------------------------------------------------------------------
Rtos::Clock::Clock( WORD timeToWait_msec,
                    WORD timeToWait_usec )
{
  start( timeToWait_msec, timeToWait_usec ); // Don't use cTimer constructor
}

//-------------------------------------------------------------------
LWORD Rtos::Clock::getTics( void )
{
  return( Rtos_Mcu::getTics() );
}

//-------------------------------------------------------------------
DWORD Rtos::Clock::getTimeOfTic( void )
{
  return( Rtos_Mcu::getTimeOfTic() );
}

}  } //namespace
//EOF
