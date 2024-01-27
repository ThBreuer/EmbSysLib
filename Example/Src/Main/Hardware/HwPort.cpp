//*******************************************************************
/*!
\file   HwPort.cpp
\author Thomas Breuer
\date   10.08.2022
\brief  Sample of using hardware related classes
*/

//*******************************************************************
/*
Usage:    GPIO-Port
          The two leds light up as long as the button is pressed.
          In- and outputs of the port are displayed as hex numbers.
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
  uart.set( "\r\n\nHwPort," __DATE__ "," __TIME__ "\r\n\n" );

  // Example setting GPIO-Mode: Set LED pin as output
  // alternatively: Set correct mode in configHw.h
  ledPort.setMode( ledPinMask, Port::Out );

  while( 1 )
  {
    // Get input from port
    bool value = (btnPort.get() & btnPinMask)?true:false;

    // Set output to port
    if( value )
      ledPort.set( ledPinMask );
    else
      ledPort.clr( ledPinMask );

    // Debug:
    char str[80];
    sprintf( str, "Input: %08lX  Output: %08lX\r", (unsigned long)btnPort.get() & btnPinMask,
                                                   (unsigned long)ledPort.get() & ledPinMask );
    uart.set( str );
  }
}
