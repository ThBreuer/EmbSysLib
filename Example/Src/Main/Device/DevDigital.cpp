//*******************************************************************
/*!
\file   DevDigital.cpp
\author Thomas Breuer
\date   04.11.2022
\brief  Sample of using class Digital
*/

//*******************************************************************
/*
Usage:    Connect board with a terminal.

          Press and release the Button A:
          - The LED indicates the state
          - A button event is indicated on the terminal
*/

//*******************************************************************
#include <stdio.h>
#include "EmbSysLib.h"

//-------------------------------------------------------------------
#include "ReportHandler.h"
#include "config.h"

//*******************************************************************
int main( void )
{
  terminal.printf( "\r\n\nDevDigital," __DATE__ "," __TIME__ "\r\n\n" );

  while( 1 )
  {
    // The led indicates the button state
    led_A = btn_A;

    // Check and show button events
    // Remark: The button is not debounced, but because of the function
    // call terminal.printf() we have a sufficient delay
    switch( btn_A.getEvent() )
    {
      case Digital::ACTIVATED: terminal.printf( "Button A activated\r\n"); break;
      case Digital::RELEASED:  terminal.printf( "Button A released\r\n" ); break;
      default:                                                             break;
    }
  }
}
