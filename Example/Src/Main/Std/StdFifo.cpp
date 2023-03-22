//*******************************************************************
/*!
\file   StdFifo.cpp
\author Thomas Breuer
\date   24.08.2022
\brief  Sample
*/

//*******************************************************************
/*
Configuration:
          Uart
          Timer

Usage:    A Data object is sent periodically from the main task to a timer task via a fifo.
          Received Data objects are checked.

          Output:
            ok:    Number of correct transfers
            err:   Number of erroneous transfers
            empty: Number of accesses to an empty Fifo
            full:  Number of accesses to an full Fifo
*/

//*******************************************************************
#include <stdio.h>
#include "EmbSysLib.h"

//-------------------------------------------------------------------
#include "ReportHandler.h"
#include "config.h"

//*******************************************************************
class Data
{
  public:
    //---------------------------------------------------------------
    long long dataA;
    long long dataB;
};

//*******************************************************************
class Test : public Timer::Task
{
  public:
    //---------------------------------------------------------------
    Test( Timer &timer )
    : fifo( 10/* size */ )
    {
      expectedValue = 0;

      ok    = 0;
      err   = 0;
      empty = 0;

      timer.add( this );
    }

    //---------------------------------------------------------------
    virtual void update( void )
    {
      if( fifo.isEmpty() )
      {
        empty++;
      }
      else
      {
        //--------
        fifo >> Y; // pull data from fifo
        //--------

        // check, if data is correct
        expectedValue++;
        if( Y.dataB != (Y.dataA ^ (-1)) || Y.dataA != expectedValue)
        {
          err++;
          expectedValue = Y.dataA;
        }
        else
        {
          ok++;
        }
      }
    }

    //---------------------------------------------------------------
    Fifo<Data>   fifo;
    Data         Y;

    long long    expectedValue;

    int          ok;
    int          err;
    int          empty;
};

//*******************************************************************
int main(void)
{
  uart.set( "\r\n\nStdFifo," __DATE__ "," __TIME__ "\r\n\n" );
  uart.set( "running ...\r\n" );

  Data X;

  X.dataA  = 0;
  int cnt  = 0;
  int full = 0;

  Test test( timer ); // start timer task

  while( 1 )
  {
    if( test.fifo.isFull() )
    {
      full++;
    }
    else
    {
      // generate a new data set
      X.dataB = (++X.dataA) ^ (-1);

      //-------------
      test.fifo << X; // push data to fifo
      //-------------
    }

    // print statistics
    if( ((cnt++)%1000) == 0 )
    {
      char str[80];
      sprintf( str, "err:%d ok:%d empty:%d full:%d \r", test.err, test.ok, test.empty, full );
      uart.set( str );
    }
  }
}
