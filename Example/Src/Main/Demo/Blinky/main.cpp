//*******************************************************************
/*!
\file   main.cpp
\author Thomas Breuer
\date   07.11.2022
\brief  Sample
*/

//*******************************************************************
/*
Used Configuration (see config.h):
        uart, led_A

Usage:
        The LED is flashing with an initial period of 1000ms.
        You can set the period by pressing '+' or '-' key on the terminal
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
  terminal.printf( "\r\n\nDemo/Blinky," __DATE__ "," __TIME__ "\r\n\n" );

  int  duration = 1000;
  char key = 1;
  
  DigitalIndicator indicator( led_A, taskManager );

  while( 1 )
  {
    if( key )
    {
      terminal.printf( "Period=%d ms\r\n", duration );
      indicator.blink( duration, 50 );
    }

    switch( key = uart.get() )
    {
      case '+': duration = MIN( 1000, duration+100 ); break;
      case '-': duration = MAX(    0, duration-100 ); break;
      default:  key = 0;                break;
    }
  }
}
