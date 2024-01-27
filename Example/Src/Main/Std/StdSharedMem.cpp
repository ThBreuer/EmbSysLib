//*******************************************************************
/*!
\file   StdSharedMem.cpp
\author Thomas Breuer
\date   19.12.2023
\brief  Sample
*/

//*******************************************************************
/*
Configuration:
          Uart
          Timer

Usage:    \todo add usage description
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
    BYTE      dummy[1000];
    long long dataB;
};

//*******************************************************************
class Test : public Timer::Task
{
  public:
    //---------------------------------------------------------------
    Test( Timer &timer )
    {
      ok    = 0;
      err   = 0;

      Y.dataA = 0;
      Y.dataB = (++Y.dataA) ^ (-1);

      sharedMem << Y;

      timer.add( this );
    }

    //---------------------------------------------------------------
    virtual void update( void )
    {
      //--------
      sharedMem >> Y; // read data from sharedMem
      //--------

      // check, if data is correct
      if( Y.dataB != (Y.dataA ^ (-1)) )
      {
        err++;
      }
      else
      {
        ok++;
      }

      Y.dataB = (++Y.dataA) ^ (-1);

      //--------
      sharedMem << Y; // write back to sharedMem
      //--------
    }

    //---------------------------------------------------------------
    SharedMem<Data>   sharedMem;
    Data              Y;

    DWORD        ok;
    DWORD        err;
};

//*******************************************************************
int main(void)
{
  uart.set( "\r\n\nStdSharedMem," __DATE__ "," __TIME__ "\r\n\n" );
  uart.set( "running ...\r\n" );

  Data X;

  DWORD cnt  = 0;
  DWORD err  = 0;
  DWORD ok   = 0;

  Test test( timer ); // start timer task

  while( 1 )
  {
    //--------
    test.sharedMem >> X; // read data from sharedMem
    //--------

    // check, if data is correct
    if( X.dataB != (X.dataA ^ (-1)) )
    {
      err++;
    }
    else
    {
      ok++;
    }

    X.dataB = (++X.dataA) ^ (-1);

    //--------
    test.sharedMem << X; // write back to sharedMem
    //--------

    // print statistics
    if( ((cnt++)%1000) == 0 )
    {
      char str[80];
      sprintf( str, "err:%lu ok:%lu \r\n", (unsigned long)err+test.err, 
                                           (unsigned long)ok+test.ok );
      uart.set( str );
    }
  }
}
