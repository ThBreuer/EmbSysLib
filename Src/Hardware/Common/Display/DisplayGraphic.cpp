//*******************************************************************
/*!
\file   DisplayGraphic.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   01.12.2021

License: See file "LICENSE"
*/

//*******************************************************************
#include "DisplayGraphic.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// DisplayGraphic
//
//*******************************************************************
//-------------------------------------------------------------------
DisplayGraphic::DisplayGraphic( WORD moduleId,
                                WORD width,
                                WORD height,
                                const Font &font,
                                BYTE zoom )

: DisplayChar( moduleId,
               height/((WORD)zoom*font.getCharHeight()),
               width /((WORD)zoom*font.getCharWidth() ) )
, font  ( font )
, zoom  ( zoom )
, report( moduleId )
{
  Hsize = width;
  Vsize = height;

  BackColor  = RGB2color(  0,  0,  0);  // Black
  PaintColor = RGB2color(128,128,128);  // Gray
  TextColor  = RGB2color(255,255,255);  // White

  xPos = 0;
  yPos = 0;
}

//-------------------------------------------------------------------
void DisplayGraphic::setFont( const Font &font,
                              BYTE zoom )
{
  this->font = font;
  this->zoom = zoom;
}

//-------------------------------------------------------------------
void DisplayGraphic::setZoom( BYTE zoom )
{
  this->zoom = zoom;
}

//---------------------------------------------------------------
void DisplayGraphic::setBackColor( WORD color )
{
  BackColor = color;
}

//---------------------------------------------------------------
void DisplayGraphic::setPaintColor( WORD color )
{
  PaintColor = color;
}

//---------------------------------------------------------------
void DisplayGraphic::setTextColor( WORD color )
{
  TextColor = color;
}

//---------------------------------------------------------------
void DisplayGraphic::gotoPixelPos( WORD x, WORD y )
{
  xPos = x;
  yPos = y;
}

//---------------------------------------------------------------
void DisplayGraphic::gotoTextPos( BYTE line, BYTE colum )
{
  xPos = (WORD)colum * zoom * font.getCharWidth();
  yPos = (WORD)line  * zoom * font.getCharHeight();
}

//---------------------------------------------------------------
void DisplayGraphic::putChar( char ch )
{
  font.setChar( ch );                                       // select char

  setArea( xPos,                                            // set painting area
           yPos,
           zoom*font.getCharWidth(),
           zoom*font.getCharHeight() );

  for( BYTE py = 0; py < font.getCharHeight(); py++ )       // scan vert.
  {
     for( BYTE zoomY = 0; zoomY < zoom; zoomY++ )
     {
        for( BYTE px = 0 ; px < font.getCharWidth(); px++ ) // scan hor.
        {
          BYTE pixel = font.getPixel( px, py );             // what color?
          for( BYTE zoomX = 0; zoomX < zoom; zoomX++ )
          {
            setPixel( (pixel) ? TextColor : BackColor );
          }
        }
     }
  }
  xPos += (WORD)zoom*font.getCharWidth();                   // emulate cursor increment
}

//---------------------------------------------------------------
void DisplayGraphic::putPixel( WORD x, WORD y )
{
  setPixel( PaintColor, x, y );
}

//---------------------------------------------------------------
void DisplayGraphic::putRectangle( WORD x,
                                   WORD y,
                                   WORD w,
                                   WORD h )
{
  setArea( x, y, w, h );             // set painting area

  for( WORD py = 0; py < h; py++ )   // scan vert.
  {
    for( WORD px = 0; px < w; px++ ) // scan hor.
    {
      setPixel( PaintColor );        // draw pixel
    }
  }
}

//---------------------------------------------------------------
void DisplayGraphic::putBitmap( WORD   x,
                                WORD   y,
                                const Bitmap &bitmap )
{
  WORD w = bitmap.getWidth();
  WORD h = bitmap.getHeight();

  setArea( x, y, w, h );                   // set painting area

  for( WORD py = 0; py < h; py++ )         // scan vert.
  {
    for( WORD px = 0; px < w; px++ )       // scan hor.
    {
      setPixel( bitmap.getPixel(px,py) );  // draw pixel and set to next
    }
  }
}

} } //namespace

//EOF
