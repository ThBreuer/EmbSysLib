//*******************************************************************
/*!
\file   DevDigitalIndicator.cpp
\author Thomas Breuer
\date   26.01.2023
\brief  Sample of using class DigitalIndicator
*/

//*******************************************************************
/*
Usage:    Connect board with a terminal.

          Set time and duty with keys '+', '-', 'd' and 'D'.
          Start and clear flashing with keys 't', 'b' and 'c'.
*/

//*******************************************************************
#include <stdio.h>
#include "EmbSysLib.h"

//-------------------------------------------------------------------
#include "ReportHandler.h"
#include "config.h"

//*******************************************************************
int main(void)
{
  terminal.printf( "\r\n\nDevDigitalIndicator," __DATE__ "," __TIME__ "\r\n\n" );

  WORD time = 1000;
  WORD duty = 50;

  while( 1 )
  {
    switch( terminal.get() )
    {
      case '+':                 time += 100; break;
      case '-': if(time >   0 ) time -= 100; break;
      case 'D': if(duty <= 90 ) duty +=  10; break;
      case 'd': if(duty >   0 ) duty -=  10; break;

      case 't': indicator.trigger( time );       terminal.printf( "trigger\r\n" ); break;
      case 'b': indicator.blink  ( time, duty ); terminal.printf( "blink\r\n"   ); break;
      case 'c': indicator.clr();                 terminal.printf( "clear\r\n"   ); break;
      default: continue;
    }
    terminal.printf( "time:%d duty:%d\r\n", time, duty );
  }
}
