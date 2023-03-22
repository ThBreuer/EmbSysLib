//*******************************************************************
/*!
\file   DigitalButton.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   26.01.2023

License: See file "LICENSE"
*/

//*******************************************************************
#include "DigitalButton.h"

//*******************************************************************
namespace EmbSysLib {
namespace Ctrl {

//*******************************************************************
//
// DigitalButton
//
//*******************************************************************
//-------------------------------------------------------------------
DigitalButton::DigitalButton( Digital     &digital,
                              TaskManager &taskManager,
                              WORD         timeDebounce,
                              WORD         timeLong )
: digital     ( digital )
, timeDebounce( (DWORD)timeDebounce*1000/taskManager.getCycleTime() )
, timeLong    ( (DWORD)timeLong    *1000/taskManager.getCycleTime() )
{
  timeOut = 0;
  time    = 0;
  taskManager.add(this);
}

//-------------------------------------------------------------------
void DigitalButton::update( void )
{
  // debounce: get new value after timeOut only
  if( timeOut == 0 )
  {
    bool in = digital.get();
    if( in != value )
    {
      value   = in;
      timeOut = timeDebounce;
    }
  }
  else
  {
    timeOut--;
  }

  // Edge detection
  if( value != valuePre )
  {
    if( value ) // rising edge
    {
      time = 0;
      action = ACTIVATED;
    }
    else // falling edge
    {
      if( time < timeLong )
      {
        action = SHORT;
      }
    }
    valuePre = value;
  }

  // time measurement
  if( value )
  {
    if( time < timeLong )
    {
      time++;
      if( time == timeLong )
      {
        action = LONG;
      }
    }
  }
}

//-------------------------------------------------------------------
DigitalButton::Action DigitalButton::getAction(  )
{
  return( action.getUnique() );
}

} } //namespace

//EOF
