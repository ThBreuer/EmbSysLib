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
Touch::Touch( WORD        moduleId, 
              WORD        width, 
              WORD        height, 
              Orientation orientation )
: report( moduleId )
{
  this->width       = width;
  this->height      = height;
  this->orientation = orientation;
  
  xPos          = 0;
  yPos          = 0;
  isTouchedFlag = false;
}

//-------------------------------------------------------------------
inline WORD Touch::getPosX( void )
{
  switch( orientation )
  {
    default:
    case Touch::ROTATION_0  : return( xPos );
    case Touch::ROTATION_90 : return( yPos );
    case Touch::ROTATION_180: return( width-xPos );
    case Touch::ROTATION_270: return( height-yPos );
  }
}

//-------------------------------------------------------------------
inline WORD Touch::getPosY( void )
{
  switch( orientation )
  {
    default:
    case Touch::ROTATION_0  : return( yPos );
    case Touch::ROTATION_90 : return( width-xPos );
    case Touch::ROTATION_180: return( height-yPos );
    case Touch::ROTATION_270: return( xPos );
  }
}

//-------------------------------------------------------------------
inline BYTE Touch::isTouched( void )
{
  return( isTouchedFlag );
}

}  } //namespace

//EOF
