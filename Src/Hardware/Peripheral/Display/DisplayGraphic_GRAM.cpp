//*******************************************************************
/*!
\file   DisplayGraphic_GRAM.cpp
\author Thomas Breuer
\date   13.03.2024
*/

//*******************************************************************
#include "DisplayGraphic_GRAM.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// DisplayGraphic_GRAM
//
//*******************************************************************
//---------------------------------------------------------------
DisplayGraphic_GRAM::DisplayGraphic_GRAM( WORD *ptr,
                             WORD width,
                             WORD height,
                             Font         font,
                             BYTE         zoom  )

    : DisplayGraphic( 0, /// todo ReportID_Hw::Module::DISPLAY_GRAPHIC_GRAM,
                      width,
                      height,
                      font,
                      zoom )
    {
      this->ptrStart = ptr;
      this->ptr      = ptrStart;
      
      Hsize = width;
      Vsize = height;

      ax = 0;
      ay = 0;
      aw = 0;
      ah = 0;
      xpos = 0;
      ypos = 0;
      init( );
}

//---------------------------------------------------------------
void DisplayGraphic_GRAM::init( void )
{
//  hwDSI.DisplayRefresh();
}

//---------------------------------------------------------------
void DisplayGraphic_GRAM::refresh( void )
{
//  hwDSI.DisplayRefresh();
}

//---------------------------------------------------------------
void DisplayGraphic_GRAM::clear( void )
{
  ptr = ptrStart;
  __IO uint16_t *ptrEnd = ptr + Hsize*Vsize*sizeof(uint16_t);
  while( ptr < ptrEnd )
    *ptr++ = BackColor;
}

//---------------------------------------------------------------
void DisplayGraphic_GRAM::setArea( WORD x,
                                          WORD y,
                                          WORD w,
                                          WORD h )
{
  ax = x;
  ay = y;
  aw = w;
  ah = h;
  xpos   = ax;
  ypos   = ay*Hsize;
  offset = (DWORD)y*Hsize + x;

  ptr = ptrStart + offset;
}

//---------------------------------------------------------------
void DisplayGraphic_GRAM::setPixel( WORD color,
                                           WORD x,
                                           WORD y )
{
   ptr = ptrStart + (DWORD)y*Hsize + x;
  *ptr = color;
}

//---------------------------------------------------------------
void DisplayGraphic_GRAM::setPixel( WORD color )
{
  *ptr++ = color;
  xpos++;
  if( xpos >= ax+aw)
  {
    xpos = ax;
    ypos++;
    offset += Hsize;
    ptr = ptrStart + offset;
  }
}

}  } //namespace

//EOF
