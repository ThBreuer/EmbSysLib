//*******************************************************************
/*!
\file   DigitalEncoderRotaryknob.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   26.01.2023

License: See file "LICENSE"
*/

//*******************************************************************
#include "DigitalEncoderRotaryknob.h"

//*******************************************************************
namespace EmbSysLib {
namespace Ctrl {

//*******************************************************************
//
// DigitalEncoderRotaryknob
//
//*******************************************************************
//-------------------------------------------------------------------
DigitalEncoderRotaryknob::DigitalEncoderRotaryknob( Digital     *switchA,
                                                    Digital     *switchB,
                                                    Digital     *btnCtrlIn,
                                                    TaskManager &taskManager )
: DigitalEncoder( btnCtrlIn, taskManager )
, switchA( switchA )
, switchB( switchB )
{
}

//-------------------------------------------------------------------
DigitalEncoder::Event DigitalEncoderRotaryknob::getIncrement( void )
{
  if( switchA && switchB )
  {
    switch( switchA->getEvent() )
    {
      case Digital::ACTIVATED: return( switchB->get()?NONE:LEFT );
      case Digital::RELEASED:  return( switchB->get()?LEFT:NONE );
      default:                 break;
    }

    switch( switchB->getEvent() )
    {
      case Digital::ACTIVATED: return( switchA->get()?NONE:RIGHT );
      case Digital::RELEASED:  return( switchA->get()?RIGHT:NONE );
      default:                 break;
    }
  }
  return( NONE );
}

} } //namespace

//EOF
