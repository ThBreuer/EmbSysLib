//*******************************************************************
/*!
\file   HwDisplayGraphic.cpp
\author Thomas Breuer
\date   10.08.2022
\brief  Sample of using hardware related classes
*/

//*******************************************************************
/*
Usage: A test output is displayed
         Send a command via terminal to the µC:
         'n' - Select next font
         'p' - Select previous font
         '+' - Increase zoom
         '-' - Decrease zoom
         Press enter to start diplay test.
*/

//*******************************************************************
#include <stdio.h>
#include "EmbSysLib.h"

//-------------------------------------------------------------------
#include "ReportHandler.h"
#include "configHw.h"

//*******************************************************************
WORD colorTable[] =
{
  Color::DarkGrey,
  Color::Grey,
  Color::LightGrey,
  Color::White,
  Color::Red,
  Color::Green,
  Color::Blue,
  Color::Yellow,
  Color::Cyan,
  Color::Magenta,
  Color::DarkRed,
  Color::DarkGreen,
  Color::DarkBlue,
  Color::DarkYellow,
  Color::DarkCyan,
  Color::DarkMagenta
};

WORD numOfColors = sizeof(colorTable)/sizeof(WORD);

Font fontTable[] =
{
  fontFont_16x24,
  fontFont_10x20,
  fontFont_8x12,
  fontFont_8x8
};

WORD numOfFonts = sizeof(fontTable)/sizeof(Font);

Bitmap bmp( bitmapBitmap_320x240 );

char txt[21];
WORD cnt  = 0;
BYTE font = 0;
BYTE zoom = 1;
char c    = ' ';

//*******************************************************************
int main(void)
{
  uart.set( "\r\n\nHwDisplayGraphic," __DATE__ "," __TIME__ "\r\n\n" );
	
  #ifdef HAS_LCD_ENABLE_PIN
  lcdDisp.set(1);
  #endif

  WORD w = dispGraphic.getWidth();
  WORD h = dispGraphic.getHeight();

  dispGraphic.setTextColor( Color::White );
  dispGraphic.setBackColor( Color::DarkBlue );
  dispGraphic.clear();

  dispGraphic.putString( "Press any key on terminal to start ..." );
  dispGraphic.refresh();

  // Waiting for user input
  while( !uart.get() );

  dispGraphic.clear();

  // Draw a Bitmap and wait
  dispGraphic.putBitmap( (w - bmp.getWidth() )/2,
                         (h - bmp.getHeight())/2,
                          bmp );
  dispGraphic.refresh();

  // Waiting for user input
  while( !uart.get() );

  // Drawing ...
  for( BYTE i=0; i<numOfColors; i++ )
  {
    // color bar
    dispGraphic.setPaintColor( colorTable[i] );
    dispGraphic.putRectangle ( w*i/numOfColors, 0, w/numOfColors, h );
  }

  dispGraphic.setPaintColor( DisplayGraphic::RGB2color( 192,  192, 192 ) );
  for( WORD x = 0; x < w; x++ )
  {
    dispGraphic.putPixel( x,   h*x/w ); // Diagonale oben,links  --> unten,rechts
    dispGraphic.putPixel( x, h-h*x/w ); // Diagonale unten,links --> oben,rechts
  }
  dispGraphic.refresh();

  // Waiting for user input
  while( !uart.get() );

  // Display actual font or font size
  while( 1 )
  {
    if( c )
    {
      // select font and zoom
      switch( c )
      {
        case 'p': if( font > 0            ) font--; break;
        case 'n': if( font < numOfFonts-1 ) font++; break;
        case '-': if( zoom > 1            ) zoom--; break;
        case '+': if( zoom < 2            ) zoom++; break;
      }
      dispGraphic.setFont( fontTable[font], zoom );
      dispGraphic.clear();

      sprintf( txt, "Font:%dx%d", fontTable[font].getCharWidth(),
                                  fontTable[font].getCharHeight() );
      dispGraphic.gotoPixelPos( 0, 20 );
      dispGraphic.putString( txt );

      sprintf( txt, "Zoom:%d", zoom );
      dispGraphic.gotoPixelPos( 0, 20+2*fontTable[font].getCharHeight()*zoom );
      dispGraphic.putString( txt );
    }

    cnt = (cnt+1)%10000;
    sprintf( txt, "cnt: %4d", cnt );
    dispGraphic.gotoPixelPos( 0, 20+3*fontTable[font].getCharHeight()*zoom );
    dispGraphic.putString( txt );

    dispGraphic.refresh();

    c = uart.get();
  }
}
