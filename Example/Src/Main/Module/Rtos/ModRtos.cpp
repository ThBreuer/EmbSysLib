//*******************************************************************
/*!
\file   ModRtos.cpp
\author Thomas Breuer
\date   13.03.2023
\brief  Sample of using hardware related class Rtos
*/

//*******************************************************************
/*
Usage:    The main-task as an idle task and two additinal tasks are 
          startet. All tasks increment their counter, the counters 
          are displayed one time per seceond.
            main:    1 increment per second
            task1: 250 increments per second
            task2: 250 increments per second, stopped and restarted 
                       every 2nd second

          Target "Virtual": approx. 50 increments per second. 
          All targets: The number of increments may vary slightly.
*/

//*******************************************************************
#include <stdio.h>
#include "EmbSysLib.h"
#include "Module/Rtos/Rtos.h"

//-------------------------------------------------------------------
#include "ReportHandler.h"
#include "config.h"

//*******************************************************************
class myTask : public Rtos::Task
{
  public:
    //---------------------------------------------------------------
    myTask( Rtos &rtos )
    : Rtos::Task( rtos, 500/* stack size*/ )
    {
      cnt = 0;
    }

  private:
    //---------------------------------------------------------------
    virtual void update( void )
    {
      while(1)
      {
        cnt++;
        pause();  // pause the task until next time slot
      }
    }

  public:
    //---------------------------------------------------------------
    DWORD cnt;

}; //class myTask

//*******************************************************************
Rtos    rtos (    4,   // max num of tasks
               1000 ); // time slice in us
               
myTask  task1( rtos );
myTask  task2( rtos );

//*******************************************************************
int main(void)
{
  DWORD cnt     = 0;
  DWORD timeout = 0; /*ms*/

  terminal.printf( "\r\n\nHwRtos," __DATE__ "," __TIME__ "\r\n\n" );

  task1.start();

  while( 1 )
  {
    DWORD time = Rtos_Mcu::getSysTime();

    if( time >= timeout )
    {
      terminal.printf( "time:%6lu, main:%6lu, task1:%6lu, task2:%6lu\r\n",
                       time,
                       cnt++,
                       task1.cnt,
                       task2.cnt );
      
      timeout += 1000;

      if( task2.isRunning() )
      {
        task2.stop();
        task2.cnt = 0;
      }
      else
      {
        task2.start();
      }
    }
  }
}
