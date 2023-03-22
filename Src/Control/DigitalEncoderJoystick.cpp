//*******************************************************************
/*!
\file   devControlEncoderJoystick.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   26.01.2023

License: See file "LICENSE"
*/

//*******************************************************************
#include "DigitalEncoderJoystick.h"

//*******************************************************************
namespace EmbSysLib {
namespace Ctrl {

//*******************************************************************
//
// DigitalEncoderJoystick
//
//*******************************************************************
//-------------------------------------------------------------------
DigitalEncoderJoystick::DigitalEncoderJoystick( Digital     *btnLeft,
                                                Digital     *btnRight,
                                                Digital     *btnCtrl,
                                                TaskManager &taskManager,
                                                WORD         repeatTime )
: DigitalEncoder( btnCtrl, taskManager )
, timerStart    ( 1000UL*repeatTime/taskManager.getCycleTime() )
, btnLeft       ( btnLeft )
, btnRight      ( btnRight )
{
  timerCnt = timerStart;
}

//-------------------------------------------------------------------
DigitalEncoder::Event DigitalEncoderJoystick::getIncrement( void )
{
  if( timerCnt )
  {
    timerCnt--;
  }

  if( btnLeft )
  {
    if(    btnLeft->getEvent() == Digital::ACTIVATED
        || (!timerCnt && timerStart && btnLeft->get()) )
    {
      timerCnt = timerStart;
      return( LEFT );
    }
  }

  if( btnRight )
  {
    if(   btnRight->getEvent() == Digital::ACTIVATED
       || (!timerCnt && timerStart && btnRight->get()) )
    {
      timerCnt = timerStart;
      return( RIGHT );
    }
  }
  return( NONE );
}

} } //namespace

//EOF
