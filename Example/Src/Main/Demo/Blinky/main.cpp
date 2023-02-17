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
        The LED is flashing with an initial frequency of 1 Hz.
        You can set the frequency by pressing '+' or '-' key on the terminal
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
    Test( Timer &timer, Digital &led )
    : timer( timer )
    , led  ( led   )
    {
      setFrequency( 1 );
      timer.add( this );
    }

    //---------------------------------------------------------------
    void setFrequency( float f )
    {
      if( f > 0 )
      {
        ticsPerPeriod = 1E6f / (f * timer.getCycleTime());
      }
      else
      {
        ticsPerPeriod = 0;
      }
    }

  private:
    //---------------------------------------------------------------
    virtual void update( void )
    {
      tics++;
      if( tics == ticsPerPeriod/2 )
      {
        led.clr();
      }
      else if( tics >= ticsPerPeriod )
      {
        led.set();
        tics = 0;
      }
    }

  private:
    //---------------------------------------------------------------
    Timer   &timer;
    Digital &led;
    DWORD    ticsPerPeriod;
    DWORD    tics;
};

//*******************************************************************
int main(void)
{
  uart.set( "\r\n\nDevDigital," __DATE__ "," __TIME__ "\r\n\n" );

  Test test( timer, led_A );

  int  freq = 1;
  char key  = 1;

  while( 1 )
  {
    if( key )
    {
      char text[20];
      sprintf( text, "Frequency=%d Hz\r\n", freq );
      uart.set( text );

      test.setFrequency( freq );
    }

    switch( key = uart.get() )
    {
      case '+': freq = MIN( 10, freq+1 ); break;
      case '-': freq = MAX(  0, freq-1 ); break;
      default:  key = 0;                  break;
    }
  }
}
