//*******************************************************************
/*!
\file   HwPortPin.cpp
\author Thomas Breuer
\date   10.08.2022
\brief  Sample of using hardware related classes
*/

//*******************************************************************
/*
Usage:    GPIO-Port-Pin
          The two leds light up as long as the button is pressed.
*/

//*******************************************************************
#include <stdio.h>
#include "EmbSysLib.h"

//-------------------------------------------------------------------
#include "ReportHandler.h"
#include "configHw.h"

//*******************************************************************
int main(void)
{
  uart.set( "\r\n\nHwPortPin," __DATE__ "," __TIME__ "\r\n\n" );

  while(1)
  {
    // Get input from port
    bool value = btnA_Pin.get();

    // Set output to port
    if( value )
      ledA_Pin.set();
    else
      ledA_Pin.clr();

    // alternatively:
    ledB_Pin.set( btnA_Pin.get() );

    // Debug:
    char str[80];
    sprintf( str, "Button: %d LED: %d,%d\r", btnA_Pin.get(),
                                             ledA_Pin.get(),
                                             ledB_Pin.get() );
    uart.set( str );
  }
}
