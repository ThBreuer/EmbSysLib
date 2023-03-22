//*******************************************************************
/*!
\file   DigitalEncoder.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   26.01.2023

License: See file "LICENSE"
*/

//*******************************************************************
#include "DigitalEncoder.h"

//*******************************************************************
namespace EmbSysLib {
namespace Ctrl {

//*******************************************************************
//
// DigitalEncoder
//
//*******************************************************************
//-------------------------------------------------------------------
DigitalEncoder::DigitalEncoder( Digital *btnCtrlIn,
                          TaskManager    &taskManager )

: btnCtrl( btnCtrlIn )
, fifo   ( 20 )

{
  taskManager.add(this);
}

//-------------------------------------------------------------------
DigitalEncoder::Event DigitalEncoder::getEvent( void )
{
  if( fifo.isEmpty() )
  {
    return( NONE );
  }
  else
  {
    Event event = NONE;
    fifo >> event;
    return( event );
  }
}

//-------------------------------------------------------------------
void DigitalEncoder::update( void )
{
  if( !fifo.isFull() )
  {
    switch( getIncrement() )
    {
      case LEFT:  fifo << LEFT;    break;
      case RIGHT: fifo << RIGHT;   break;
      default:                     break;
    }
  }

  if( !fifo.isFull() )
  {
    switch( getCtrl() )
    {
      case CTRL_DWN:  fifo << CTRL_DWN;  break;
      case CTRL_UP:   fifo << CTRL_UP;   break;
      default:                           break;
    }
  }
}

//-------------------------------------------------------------------
DigitalEncoder::Event DigitalEncoder::getCtrl( void )
{
  if( btnCtrl )
  {
    switch( btnCtrl->getEvent() )
    {
      case Digital::ACTIVATED: return( CTRL_DWN );
      case Digital::RELEASED:  return( CTRL_UP );
      default:                 break;
    }
  }
  return( NONE );
}

} } //namespace

//EOF
