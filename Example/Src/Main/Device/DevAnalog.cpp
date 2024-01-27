//*******************************************************************
/*!
\file   DevAnalog.cpp
\author Thomas Breuer
\date   18.12.2023
\brief  Sample of using classes AnalogIn and AnalogOut
*/

//*******************************************************************
/*
Usage:    Connect board with a terminal.

          \todo add usages
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
  terminal.printf( "\r\n\nDevAnalog," __DATE__ "," __TIME__ "\r\n\n" );

  while( 1 )
  {
    WORD  w = analogIn.getRaw();
    float f = analogIn;

    analogOut_A = analogIn;
    analogOut_B = f;

    terminal.printf( "Analog: %6.2f (%5u)\r", f, w );

    // \todo add test of calibration
    // \todo use min/max in constructor (config.h)
  }
}
