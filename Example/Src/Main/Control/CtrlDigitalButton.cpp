//*******************************************************************
/*!
\file   CtrlDigitalButton.cpp
\author Thomas Breuer
\date   26.01.2023
\brief  Sample of using class DigitalButton
*/

//*******************************************************************
/*
Usage:    Connect board with a terminal.

          Press and release the Button A:
           - A button event is indicated on the terminal
           - The LED indicates the state
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
  terminal.printf( "\r\n\nCtrlDigitalButton," __DATE__ "," __TIME__ "\r\n\n" );

  while( 1 )
  {
    switch( button.getAction() )
    {
      case 1: terminal.printf("\r\nactivated\r\n");
              break;

      case 2: terminal.printf("short press\r\n");
              indicator.trigger(500);
              break;

      case 3: terminal.printf("long press\r\n");
              indicator.blink(500,50);
              break;

      default: break;
    }
  }
}
