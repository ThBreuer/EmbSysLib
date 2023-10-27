//*******************************************************************
/*!
\file   StdFlag.cpp
\author Thomas Breuer
\date   13.09.2022
\brief  Sample
*/

//*******************************************************************
/*
Configuration:
          Uart
          Timer

Usage:    Send a character '0' to '9' via terminal to the µC.
          The flag A is set to the corresponding value and flag B is set to 'true'.

          Output:
            - "event", if flag B is set ( the current value is not 0 ) since last call
              This indicates a single event.

            - "unique", if flag A is changed ( the current value is neither equal to the previous value nor 0 ).
              This indicates a unique change of state.

            - The value of flag A is continuously printed
*/

//*******************************************************************
#include <stdio.h>
#include "EmbSysLib.h"

//-------------------------------------------------------------------
#include "ReportHandler.h"
#include "config.h"

//*******************************************************************
class Test : public Timer::Task
{
  public:
    //---------------------------------------------------------------
    Test( Timer &timer )
    {
      // call update() asynchronously
      timer.add( this );
    }

    //---------------------------------------------------------------
    virtual void update( void )
    {
      BYTE c;
      if( uart.get(c) )
      {
        if( '0' <= c && c <= '9' )
        {
          // set flagA and flagB in case of an input (keyboard hit)
          flagA = c - '0';
          flagB = true;
        }
      }
    }

    //---------------------------------------------------------------
    Flag<int>  flagA;
    Flag<bool> flagB;
    //---------------------------------------------------------------
};

//*******************************************************************
int main(void)
{
  uart.set( "\r\n\nStdFlag," __DATE__ "," __TIME__ "\r\n\n" );
  uart.set( "running ...\r\n" );

  char str[40];

  Test test( timer );

  while( 1 )
  {
    // check, if flagB is set since last call
    if( test.flagB.getEvent() )
    {
      sprintf( str, "\n\n event \r\n" );
      uart.set( str );
    }

    // check, if flagA is changed to a new value
    if( test.flagA.getUnique() )
    {
      sprintf( str, " unique \r\n" );
      uart.set( str );
    }

    // print current state of flagA
    sprintf( str, " flag: %d \r", (int)test.flagA );
    uart.set( str );
  }
}
