//*******************************************************************
/*!
\file   HwTimer.cpp
\author Thomas Breuer
\date   06.07.2022
\brief  Sample of using hardware related class "Timer"
*/

//*******************************************************************
/*
Usage:    Timer-task:
          The update()-Methods are executed within a timer interrupt
          with 100µs (fast) resp. 10ms (slow) period time. The number
          of executions are displayed.

          PWM:
          The LED is controlled by the PWM output.
          Send a command via terminal to the µC:
            '+' - Increase PWM output
            '-' - Decrease PWM output
          The the PWM output value is also displayed on the terminal
*/

//*******************************************************************
#include <stdio.h>
#include "EmbSysLib.h"

//-------------------------------------------------------------------
#include "ReportHandler.h"
#include "configHw.h"

//*******************************************************************
class cTest : public Timer::Task
{
  public:
    //---------------------------------------------------------------
    cTest( Timer &timer )
    {
      cnt = 0;
      timer.add( this );
    }

    //---------------------------------------------------------------
    virtual void update( void )
    {
      cnt = ((cnt < 9999999) ? (cnt + 1) : 0);
    }

    //---------------------------------------------------------------
    DWORD cnt;
};

//*******************************************************************
int main(void)
{
  uart.set( "\r\n\nHwTimer," __DATE__ "," __TIME__ "\r\n\n" );

  cTest     testSlow( timerSlow );
  cTest     testFast( timerFast );

  WORD value = 0;
  WORD step  = 0;
  char str[60];

  timerPWM.enablePWM( timerPwmCh, Timer::NORMAL );

  while( 1 )
  {
    sprintf( str, "fast: %5lu,  slow: %5lu,  pwm: %5u\r",
                  (unsigned long)testFast.cnt,
                  (unsigned long)testSlow.cnt,
                  value );
    uart.set( str );

    switch( uart.get() )
    {
      // set amplitude
      case '+': if( step < 16 ) step++; break;
      case '-': if( step >  0 ) step--; break;
    }
    value = (WORD)( ((DWORD)1 << step) - 1 );

    timerPWM.setPWM( timerPwmCh, value );
  }
}
