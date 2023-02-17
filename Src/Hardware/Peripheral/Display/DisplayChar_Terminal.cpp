//*******************************************************************
/*!
\file   DisplayChar_Terminal.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   17.11.2022
License: See file "LICENSE"
*/

//*******************************************************************
#include "DisplayChar_Terminal.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// DisplayChar_Terminal
//
//*******************************************************************
//-------------------------------------------------------------------
DisplayChar_Terminal::DisplayChar_Terminal( Uart &uartIn,
                                            WORD numOfLines, 
                                            WORD numOfColumns )

: DisplayChar( ReportID_Hw::Module::DISPLAY_CHAR_TERMINAL,
               Std::MIN( numOfLines,   (WORD)256 ),
               Std::MIN( numOfColumns, (WORD)256 ) )
, uart( uartIn )
{
  clear();
}

//-------------------------------------------------------------------
inline void DisplayChar_Terminal::clear( void )
{
  sendESC("[2J");   // Clear
  sendESC("[?25l"); // Cursor off
}

//-------------------------------------------------------------------
inline void DisplayChar_Terminal::refresh( void )
{
}

//-------------------------------------------------------------------
inline void DisplayChar_Terminal::gotoTextPos( BYTE lineIn, BYTE columIn )
{
  char str[12];
  line  = lineIn;
  colum = columIn;

  // Startadresse fuer Display-Speicher schreiben
  if( line < numberOfLines && colum < numberOfColumns )
  {
    sprintf( str, "[%02d;%02df", (WORD)line  + 1,
                                 (WORD)colum + 1 );
    sendESC( str );
  }
}

//-------------------------------------------------------------------
inline void DisplayChar_Terminal::putChar( char c )
{
  if( line < numberOfLines && colum < numberOfColumns )
  {
    uart.set( c ); // putchar at current pos
    colum++;       // emulate cursor increment
  }
}

//-------------------------------------------------------------------
void DisplayChar_Terminal::sendESC( const char *seq )
{
  uart.set( 27 );
  for( BYTE i = 0; seq[i] != 0; i++ )
  {
    uart.set( seq[i] );
  }
}

}  } //namespace

//EOF
