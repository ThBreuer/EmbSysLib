//*******************************************************************
/*!
\file   HwUart.cpp
\author Thomas Breuer
\date   10.08.2022
\brief  Sample of using hardware related class 'UART'
*/

//*******************************************************************
/*
Usage:    UART receive and transmit
          Each character received is enclosed in brackets
          and sent back
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
  uart.set( "\r\n\nHwUart," __DATE__ "," __TIME__ "\r\n\n" );
  uart.set( "UART: \r\n" );

  while( 1 )
  {
    BYTE value;

    if( uart.get( value ) )
    {
      uart.set("<");
      uart.set( value );
      uart.set(">\r\n");
    }

    /// alternatively:
    value = uart.get();
    if( value != 0 )
    {
      uart.set("<");
      uart.set( value );
      uart.set(">\r\n");
    }
    /// -------------
  }
}
