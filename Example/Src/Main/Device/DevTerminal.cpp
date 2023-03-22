//*******************************************************************
/*!
\file   DevTerminal.cpp
\author Thomas Breuer
\date   15.02.2013, 18.03.2016
\brief  Sample code, class Terminal
*/

//*******************************************************************
/*
Usage:    Connect board with a terminal.

          Edit a text or press cursor keys.
          Watch the terminal output
*/

//*******************************************************************
#include "EmbSysLib.h"
#include "config.h"

//*******************************************************************
int main( void )
{
  terminal.printf( "\r\n\nDevTerminal," __DATE__ "," __TIME__ "\r\n\n" );

  while( 1 )
  {
    if( char *str = terminal.getString() )
    {
      if( str[0] <= 0x07 )
      {
        WORD c = (WORD)str[0]<<8 | str[1];

        switch( c & 0x1FF )
        {
          case Terminal::UP:      terminal.printf( "UP"      );        break;
          case Terminal::DOWN:    terminal.printf( "DOWN"    );        break;
          case Terminal::LEFT:    terminal.printf( "LEFT"    );        break;
          case Terminal::RIGHT:   terminal.printf( "RIGHT"   );        break;
          case Terminal::PGUP:    terminal.printf( "PGUP"    );        break;
          case Terminal::PGDOWN:  terminal.printf( "PGDOWN"  );        break;
          default:                terminal.printf( "<0x%x>", str[1] ); break;
        }

        if( c & Terminal::ALT )
        {
           terminal.printf( " + alt" );
        }
        if( c & Terminal::SHIFT_CTRL )
        {
           terminal.printf( " + shift/ctrl" );
        }
        terminal.printf( "\r\n" );
      }
      else
      {
        terminal.printf("\r\n<%s>\r\n",str);
      }
    }
  }
}
