//*******************************************************************
/*!
\file   Pointer.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   01.02.2023

License: See file "LICENSE"
*/

//*******************************************************************
#include "Pointer.h"

//*******************************************************************
namespace EmbSysLib {
namespace Dev {

//*******************************************************************
//
// Pointer
//
//*******************************************************************
//-------------------------------------------------------------------
Pointer::Pointer( Touch &touchIn )

: touch( touchIn )

{
  data.flags = Data::NONE;
  data.posX  = 0;
  data.posY  = 0;
  data.delta = 0;
}

//-------------------------------------------------------------------
Pointer::Data Pointer::get( void )
{
  touch.update();

  int x = RANGE( (short)touch.getPosX(), (short)0, (short)0x7FFF );
  int y = RANGE( (short)touch.getPosY(), (short)0, (short)0x7FFF );
  BYTE isTouched = touch.isTouched();

  if( isTouched && !isTouchedPrev )
  {
    data.flags = Data::CTRL_DWN;
    data.posX  = x;
    data.posY  = y;
    data.delta = 0;
    posXprev   = x;
    posYprev   = y;
  }
  else if( !isTouched  && isTouchedPrev )
  {
    data.flags = Data::CTRL_UP;
    data.posX  = x;
    data.posY  = y;
    data.delta = 0;
  }
  else if( isTouched  && isTouchedPrev )
  {
    data.flags = Data::PRESSED;
    data.posX  = x;
    data.posY  = y;
    data.delta = 0;

    if( x != posXprev || y != posYprev )
    {
      if( abs(posXprev-x) > 15 || abs(posYprev-y) > 15 )
      {
        int deltaX = x-posXprev;
        int deltaY = y-posYprev;
        if( abs(deltaX) > abs(deltaY) )
        {
          data.flags = Data::MOVE_X;
          data.delta = deltaX;
        }
        else
        {
          data.flags = Data::MOVE_Y;
          data.delta = -deltaY;
        }
        posXprev = x;
        posYprev = y;
      }
    }
  }
  else
  {
    data.flags = Data::NONE;
    data.delta = 0;
  }
  isTouchedPrev = isTouched ;
  return( data );
}

} } //namespace

//EOF
