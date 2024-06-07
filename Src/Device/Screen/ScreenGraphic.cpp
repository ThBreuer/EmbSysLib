//*******************************************************************
/*!
\file   ScreenGraphic.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   01.02.2023

License: See file "LICENSE"
*/

//*******************************************************************
#include <stdio.h>
#include <stdarg.h>
#include <math.h>

#include "ScreenGraphic.h"

//*******************************************************************
namespace EmbSysLib {
namespace Dev {

//*******************************************************************
//
// ScreenGraphic
//
//*******************************************************************
//-------------------------------------------------------------------
ScreenGraphic::ScreenGraphic( DisplayGraphic &display )

: display( display )

{
  maxStrLength = display.getNumberOfColumns();
  str = new char[maxStrLength+1];
  clear();
}


//-------------------------------------------------------------------
void ScreenGraphic::clear( void )
{
  display.clear();
}

//-------------------------------------------------------------------
void ScreenGraphic::refresh( void )
{
  display.refresh();
}

//-------------------------------------------------------------------
void ScreenGraphic::printf( BYTE        line,
                            BYTE        column,
                            const char *format,
                            ... )
{
  va_list argzeiger;
  va_start( argzeiger, format );
  vsnprintf( str, maxStrLength+1, format, argzeiger );
  va_end  ( argzeiger );

  display.gotoTextPos( line, column );
  display.putString( str );
}

//-------------------------------------------------------------------
void ScreenGraphic::setFont( const Font &font, BYTE zoom )
{
  display.setFont( font, zoom );
};

//-------------------------------------------------------------------
void ScreenGraphic::setZoom( BYTE zoom )
{
  display.setZoom( zoom );
};

//-------------------------------------------------------------------
void ScreenGraphic::setBackColor( WORD color )
{
  display.setBackColor( color );
}

//-------------------------------------------------------------------
void ScreenGraphic::setTextColor( WORD color )
{
  display.setTextColor( color );
}

//-------------------------------------------------------------------
void ScreenGraphic::drawText( WORD        x,
                              WORD        y,
                              const char *format,
                              ... )
{
  va_list argzeiger;
  va_start( argzeiger, format );
  vsnprintf( str, maxStrLength+1, format, argzeiger );
  va_end  ( argzeiger );

  display.gotoPixelPos( x, y );
  display.putString( str );
}

//-------------------------------------------------------------------
void ScreenGraphic::drawPixel( WORD x, WORD y, WORD color )
{
   display.setPaintColor( color );
   display.putPixel( x, y );
}

//-------------------------------------------------------------------
void ScreenGraphic::drawRectangle( WORD x,
                                   WORD y,
                                   WORD w,
                                   WORD h,
                                   WORD color )
{
   display.setPaintColor( color );
   display.putRectangle( x, y, w, h );
}

//-------------------------------------------------------------------
void ScreenGraphic::drawRectangle( WORD x,
                                   WORD y,
                                   WORD w,
                                   WORD h,
                                   WORD th,
                                   WORD color )
{
  th = MIN( th, MIN(h,w) );

  // horizontal
  drawRectangle( x, y     , w, th, color );
  drawRectangle( x, y+h-th, w, th, color );

  // vertikal
  drawRectangle( x,      y, th, h, color );
  drawRectangle( x+w-th, y, th, h, color );
}

//-------------------------------------------------------------------
// See: https://de.wikipedia.org/wiki/Bresenham-Algorithmus (modified)
//
void ScreenGraphic::drawCircle( WORD x0,
                                WORD y0,
                                WORD r,
                                WORD th,
                                WORD color)
{
  if( th >= r ) // filled circle?
  {
    drawCircle( x0, y0, r, color );
    return;
  }

  display.setPaintColor( color );

  // Inner circle
  short fi  = 1 - (r-th);
  short dyi = -2*(r-th);
  short yi  = r-th;

  // Outer circle
  short f  = 1 - r;
  short dy = -2 * r;
  short y  = r;

  for( short x = 0, dx = 2; x <= y; x++, dx += 2 )
  {
                                                             // Sector:
    display.putRectangle(x0 - x,      y0 - y,      1,  th ); // N->NW
    display.putRectangle(x0 + x,      y0 - y,      1,  th ); // N->NE
    display.putRectangle(x0 + y-th+1, y0 - x,      th, 1  ); // E->NE
    display.putRectangle(x0 + y-th+1, y0 + x,      th, 1  ); // E->SE
    display.putRectangle(x0 + x,      y0 + y-th+1, 1,  th ); // S->SE
    display.putRectangle(x0 - x,      y0 + y-th+1, 1,  th ); // S->SW
    display.putRectangle(x0 - y,      y0 + x,      th, 1  ); // W->SW
    display.putRectangle(x0 - y,      y0 - x,      th, 1  ); // W->NW

    // Inner circle
    if (fi >= 0)
    {
      yi  -= 1;
      dyi += 2;
      fi  += dyi;
    }
    fi += dx + 1;

    // Outer circle
    if (f >= 0)
    {
      y  -= 1;
      dy += 2;
      f  += dy;
    }
    f += dx + 1;

    // length of line from inner to outer circle
    if( yi<x)
      th = y - x + 1;
    else
      th = y - yi;
    }
}

//-------------------------------------------------------------------
// See: https://de.wikipedia.org/wiki/Bresenham-Algorithmus (modified)
//
void ScreenGraphic::drawCircle( WORD x0,
                                WORD y0,
                                WORD r,
                                WORD color )
{
  short f  = 1 - r;
  short dy = -2 * r;
  short y  = r;

  display.setPaintColor( color );

  for( short x = 0, dx = 2; x <= y; x++, dx += 2 )
  {
    display.putRectangle ( x0+x, y0-y, 1, y+y );
    display.putRectangle ( x0-x, y0-y, 1, y+y );
    display.putRectangle ( x0+y, y0-x, 1, x+x );
    display.putRectangle ( x0-y, y0-x, 1, x+x );

    if (f >= 0)
    {
      y  -= 1;
      dy += 2;
      f  += dy;
    }
    f += dx + 1;
  }
}

//-------------------------------------------------------------------
void ScreenGraphic::drawLine( WORD x0,
                              WORD y0,
                              WORD x1,
                              WORD y1,
                              WORD th,
                              WORD color )
{
  long t;
  int  x;
  int  y;
  int  dx = (int)x1-x0;
  int  dy = (int)y1-y0;

  if( dy == 0 ) // horizontal
  {
    x0 = (x0 < x1) ? x0 : x1;
    y0 = (y0 < y1) ? y0 : y1;
    drawRectangle( x0, y0-th/2, abs(dx)+1, th, color );
  }
  else if( dx == 0 ) // vertikal
  {
    x0 = (x0 < x1) ? x0 : x1;
    y0 = (y0 < y1) ? y0 : y1;
    drawRectangle(x0-th/2, y0, th, abs(dy)+1, color );
  }
  else if( abs(dx) <= abs(dy) )
  {
    for( t = abs(dy); t >= 0; t-- )  // t = 1 downto 0
    {
      x = x0 + t*dx/abs(dy)-th/2;    // x = x0+dx downto x0
      y = y0 + ( (dy>0) ? +t : -t ); // y = y0+dy downto y0
      for( int i = th-1; i >= 0; i-- )
      {
        drawPixel( x+i, y, color );
      }
    }
  }
  else
  {
    for( t = abs(dx); t >= 0; t-- )  // t = 1 downto 0
    {
      x = x0 + ( (dx>0) ? +t : -t ); // x = x0+dx downto x0
      y = y0 + t*dy/abs(dx)-th/2;    // y = y0+dy downto y0
      for( int i = th-1; i >= 0; i-- )
      {
        drawPixel( x, y+i, color );
      }
    }
  }
}

//-------------------------------------------------------------------
void ScreenGraphic::drawBitmap( WORD    x,
                                WORD    y,
                                const Bitmap &bitmap )
{
  display.putBitmap( x, y, bitmap );
}

} } //namespace

//EOF
