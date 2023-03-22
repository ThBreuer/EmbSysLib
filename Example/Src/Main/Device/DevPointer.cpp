//*******************************************************************
/*!
\file   DevPointer.cpp
\author Thomas Breuer
\date   01.02.2023
\brief  Sample of using class Pointer
*/

//*******************************************************************
/*
Usage:    Connect board with a terminal.

          Move mouse over the simulated display and press mouse button.
          Mouse events and positions are display on the terminal.
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
  terminal.printf( "\r\n\nDevPointer," __DATE__ "," __TIME__ "\r\n\n" );

  while( 1 )
  {
    // Check and show pointer events
    Pointer::Data point = pointer.get();

    if( point.flags  )
    {
      if( point.flags & Pointer::Data::CTRL_DWN )
      {
        uart.set("\r\n");
      }

      terminal.printf( "Pos:%4d,%4d Delta: %4d Event:0x%02x\r\n",
                       point.posX, point.posY, point.delta, point.flags );
    }

    System::delayMilliSec( 100 );
  }
}
