//*******************************************************************
/*!
\file   StdClock.cpp
\author Thomas Breuer
\date   27.01.2024
\brief  Sample
*/

//*******************************************************************
/*
Configuration:
          Uart
          Timer

Usage:    ...

          // todo add usage
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
  uart.set( "\r\n\nStdTimer," __DATE__ "," __TIME__ "\r\n\n" );
  uart.set( "running ...\r\n" );

  char str[80];
  WORD timeOut = 1000;

  TaskManager::Clock clock( taskManager, timeOut );

  while( 1 )
  {
    DWORD t = clock.get();

    if( clock.trigger() )
    {
      clock.start( timeOut+=100 );

      sprintf( str, "Time: %8lu \n", (unsigned long)t );
      uart.set( str );

      if( timeOut > 2000 )
      {
        clock.stop();

        uart.set( "timeout stopped.\r\n" );
        break;
      }
    }
  }

  while( 1 )
  {
    sprintf( str, "Time: %8lu \r", (DWORD)clock.get() );
    uart.set( str );
  }
}
