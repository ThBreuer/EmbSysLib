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
#include "configDev.h"

//*******************************************************************
int main(void)
{
  uart.set( "\r\n\nDevDigital," __DATE__ "," __TIME__ "\r\n\n" );

  while( 1 )
  {
    // The led indicates the button state
    led_A = btn_A;

    // Check and show button events
    // Remark: The button is not debounced, but because of the function
    // call uart.set() we have a sufficient delay
    switch( btn_A.getEvent() )
    {
      case Digital::ACTIVATED: uart.set( "Button A activated\r\n"); break;
      case Digital::RELEASED:  uart.set( "Button A released\r\n" ); break;
      default:                                                      break;
    }
  }
}
