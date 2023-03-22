//*******************************************************************
/*!
\file   ScreenGraphic.cpp
\author Thomas Breuer
\date   26.02.2023
\brief  Sample Device Screen (graphic)
*/

//*******************************************************************
/*
Usage:    A graphics test output is displayed
*/

//*******************************************************************
#include <stdio.h>
#include "math.h"

#include "EmbSysLib.h"

//-------------------------------------------------------------------
#include "ReportHandler.h"
#include "config.h"

//*******************************************************************
int main( void )
{
  WORD cnt = 0;

  while( 1 )
  {
    // Begin of a new screen?
    if( cnt%(screenGraphic.getWidth() - bitmapBitmap_32x32.getWidth() ) == 0 )
    {
      // Setup
      screenGraphic.setTextColor( Color::White    );
      screenGraphic.setBackColor( Color::DarkBlue );

      screenGraphic.clear(); // clear with actuall back color

      // Draw frame with lines
      screenGraphic.drawLine(   0,  40, 319,  40, 1, Color::White );
      screenGraphic.drawLine(   0, 199, 319, 199, 1, Color::White );
      screenGraphic.drawLine(   0,  40,   0, 199, 1, Color::White );
      screenGraphic.drawLine( 319,  40, 319, 199, 1, Color::White );
      screenGraphic.drawLine( 160,  40, 160, 199, 1, Color::White );

      // Draw rectangles
      screenGraphic.drawRectangle(  10,  50, 140, 140,     Color::White      );
      screenGraphic.drawRectangle(  10,  50, 140, 140, 1,  Color::Red        );
      screenGraphic.drawRectangle(  20,  60, 120, 120, 10, Color::DarkGreen  );
      screenGraphic.drawLine     (   0,  40, 160, 199, 1,  Color::DarkYellow );
      screenGraphic.drawLine     (   0, 199, 160,  40, 1,  Color::DarkCyan   );

      // Draw circles
      screenGraphic.drawCircle( 240, 120,  70,         Color::White       );
      screenGraphic.drawCircle( 240, 120,  70,   1,    Color::Red         );
      screenGraphic.drawCircle( 240, 120,  50,  10,    Color::DarkGreen   );
      screenGraphic.drawLine  ( 160,  40, 319, 199, 3, Color::DarkMagenta );
      screenGraphic.drawLine  ( 160, 199, 319,  40, 3, Color::DarkGrey    );

      // Draw some texts
      screenGraphic.setBackColor(Color::Blue);
      screenGraphic.setFont ( fontFont_10x20, 1 );
      screenGraphic.drawText( 10, 5, "DevScreenGraphic" );

      screenGraphic.setFont ( fontFont_8x12 );
      screenGraphic.setZoom ( 1 );
      screenGraphic.drawText( 10, 25, __DATE__ "," __TIME__ );

      screenGraphic.setBackColor(Color::DarkBlue);
      screenGraphic.setFont( fontFont_10x20 );

      cnt = 0;
    }

    // Draw a bitmap and spome pixels
    screenGraphic.drawBitmap   ( cnt , 204, bitmapBitmap_32x32 );
    screenGraphic.drawPixel    ( cnt , 200   + (cnt%80)/2, Color::Yellow );
    screenGraphic.drawPixel    ( cnt , 239   - (cnt%80)/2, Color::Yellow );

    // Draw another text
    screenGraphic.drawText( 240, 10, "%3d", cnt );

    // Refresh screen and wait
    screenGraphic.refresh();
    System::delayMilliSec( 10 );

    cnt++;
  }

}
