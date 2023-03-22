//*******************************************************************
/*!
\file   HwPort_PC8574.cpp
\author Thomas Breuer
\date   20.02.2023
\brief  Sample of using hardware related classes
*/

//*******************************************************************
/*
Usage:    Connect board with terminal.

          Type a hexadecimalnumber and confirm by pressing 'enter',
          the hexadecimalnumber represents the PC8574 Pinmask.
          I2C Device output will also be set to the hexadecimal number,
          this value is read back by the terminal.
*/

//*******************************************************************
#include <stdio.h>
#include "EmbSysLib.h"

//-------------------------------------------------------------------
#include "ReportHandler.h"
#include "configHw.h"

using namespace EmbSysLib::Dev;

Terminal   terminal( uart, 255,255, "erw" );


//==================================================
#include "Hardware/Peripheral/Port/Port_PCF8574.cpp"

Port_PCF8574 port( i2cBus, 0 /*sub address*/ ); 
//===================================================

//*******************************************************************
int main(void)
{
  terminal.printf( "\r\n\nHwPort_PC8574," __DATE__ "," __TIME__ "\r\n\n" );

  BYTE  valueIn  = 0;
  BYTE  valueOut = 0;
  char *str;

  port.setMode( 0xFF, Port::Out );

  while( 1 )
  {
    if( (str = terminal.getString()) )
    {
      sscanf( str, "%hhx", &valueOut );

      port.set( 0xFF, valueOut );

      valueIn = port.get();

      if( port.isError() )
      {
        terminal.printf( "\r\nI2C error\r\n" );
      }
      else
      {
        terminal.printf( "in: %02x   out:%02x  \r\n", valueIn, valueOut );
      }
    }
  }
}
