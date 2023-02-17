//*******************************************************************
/*!
\file   Touch.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   11.03.2013

License: See file "LICENSE"
*/

//*******************************************************************
#include "Touch.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// Touch
//
//*******************************************************************
//-------------------------------------------------------------------
Touch::Touch( WORD moduleId, WORD widthIn, WORD heightIn )
: report( moduleId )
{
  width  = widthIn;
  height = heightIn;
  xPos   = 0;
  yPos   = 0;
  isTouchedFlag = false;
}

//-------------------------------------------------------------------
inline WORD Touch::getPosX( void )
{
  return( xPos );
}

//-------------------------------------------------------------------
inline WORD Touch::getPosY( void )
{
  return( yPos );
}

//-------------------------------------------------------------------
inline BYTE Touch::isTouched( void )
{
  return( isTouchedFlag );
}

}  } //namespace

//EOF
