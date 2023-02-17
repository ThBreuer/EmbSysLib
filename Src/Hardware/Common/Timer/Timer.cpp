//*******************************************************************
/*!
\file   Timer.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"
*/

//*******************************************************************
#include "Timer.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// Timer
//
//*******************************************************************
//-------------------------------------------------------------------
Timer::Timer( WORD moduleId )
: report( moduleId )
{
}

//-------------------------------------------------------------------
DWORD Timer::getCycleTime( void )
{
  return( cycleTime );
}

//-------------------------------------------------------------------
void Timer::add( Timer::Task *ptr )
{
  isrSeq.add( ptr );
}

//-------------------------------------------------------------------
inline void Timer::isrHandler( void )
{
  Task *ptr = isrSeq.getFirst();
  while( ptr )
  {
    ptr->update();
    ptr = ptr->getNext();
  }
}

}  } //namespace

//EOF
