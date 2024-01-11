//*******************************************************************
/*!
\file   DigitalIndicator.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   26.01.2023

License: See file "LICENSE"
*/

//*******************************************************************
#include "DigitalIndicator.h"

//*******************************************************************
namespace EmbSysLib {
namespace Ctrl {

//*******************************************************************
//
// DigitalIndicator
//
//*******************************************************************
//-------------------------------------------------------------------
DigitalIndicator::DigitalIndicator( Digital     &digital,
                                    TaskManager &taskManager )
: digital( digital )
{
  clr();
  cycleTime = taskManager.getCycleTime();
  taskManager.add(this);
}

//-------------------------------------------------------------------
void DigitalIndicator::update( void )
{
  if( runUpdate )
  {
    if( t >= maximum )
    {
      if( repeat && maximum )
      {
        t = 0;
      }
      else
      {
        clr();
      }
    }
    else
    {
      if( t == 0 )
      {
        digital.set();
      }
      else if( t >= limit )
      {
        digital.clr();
      }
      t++;
    }
  }
}

//-------------------------------------------------------------------
void DigitalIndicator::clr( void )
{
  runUpdate = false;
  maximum   = 0;
  repeat    = false;
  t         = 0;
  digital.clr();
}

//-------------------------------------------------------------------
void DigitalIndicator::trigger( WORD time )
{
  runUpdate = false;
  repeat    = false;
  maximum   = ((DWORD)time*1000)/cycleTime;
  limit     = maximum;
  t         = 0;
  runUpdate = true;
}

//-------------------------------------------------------------------
void DigitalIndicator::blink( WORD time, WORD duty )
{
  runUpdate = false;
  repeat    = true;
  maximum   = ((DWORD)time*1000)/cycleTime;
  limit     = ((DWORD)MIN(duty,(WORD)100))*maximum/100;
  runUpdate = true;
}

} } //namespace

//EOF
