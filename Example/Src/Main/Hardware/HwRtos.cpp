//*******************************************************************
/*!
\file   HwRtos.cpp
\author Thomas Breuer
\date   07.03.2023
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

\remark   This module is for test purpose only! Don't use it as a
          template!
*/

//*******************************************************************
#include <stdio.h>
#include "EmbSysLib.h"

//-------------------------------------------------------------------
#include "ReportHandler.h"
#include "configHw.h"

//*******************************************************************
#define NUM_OF_TASKS    4
#define STACKSIZE     500

//*******************************************************************
class myRTOS
{
  public:
    //---------------------------------------------------------------
    class Task
    {
      public:
        //-----------------------------------------------------------
        Task( myRTOS &rtos )
        : rtos( rtos )
        {
          rtos.add( *this );
        }

        //-----------------------------------------------------------
        static void starter( void *ptr )
        {
          ((myRTOS::Task*)ptr)->update();
          while(1); // The task should never end
        }

        //-----------------------------------------------------------
        virtual void update( void ) = 0;

        //-----------------------------------------------------------
        BYTE    stack[STACKSIZE];
        BYTE    id;
        myRTOS &rtos;

    }; // class Task
    //---------------------------------------------------------------

  public:
    //---------------------------------------------------------------
    myRTOS( WORD us )
    {
      ptr         = this;
      currentTask = 0;
      lastId      = 1;

      for( BYTE i = 1; i < NUM_OF_TASKS; i++ )
      {
        stackPointer[i] = 0;
      }

      Rtos_Mcu::init( us, schedule );
    }

    //---------------------------------------------------------------
    void add( Task &task )
    {
      if( lastId+1 < NUM_OF_TASKS )
      {
        task.id = lastId++;
      }
    }

    //---------------------------------------------------------------
    void start( Task &task )
    {
      for( BYTE i = 1; i < NUM_OF_TASKS; i++ )
      {
        if( stackPointer[i] == 0 )
        {
           task.id = i;
           break;
        }
      }
      stackPointer[task.id] = Rtos_Mcu::start(  task.stack,
                                                STACKSIZE,
                                               &task,
                                                task.starter );
    }

    //---------------------------------------------------------------
    void stop( Task &task )
    {
       Rtos_Mcu::stop(stackPointer[task.id]);
       stackPointer[task.id] = 0;
    }

    //---------------------------------------------------------------
    void pause(  )
    {
      Rtos_Mcu::pause();
    }

    //---------------------------------------------------------------
    BYTE isRunning( Task &task )
    {
      return( stackPointer[task.id] != 0 );
    }

  private:
    //---------------------------------------------------------------
    static Rtos_Mcu::TaskHandle schedule( Rtos_Mcu::TaskHandle stackPtr, BYTE flag )
    {
      return( ptr->getNextTask(stackPtr, flag) );
    }

    //---------------------------------------------------------------
    Rtos_Mcu::TaskHandle getNextTask( Rtos_Mcu::TaskHandle stackPtr, bool isSysTic)
    {
      stackPointer[currentTask] = stackPtr;
      currentTask               = 0;

      if( isSysTic ) // called by timeout
      {
        nextTask = (nextTask+1) % NUM_OF_TASKS;
        if( stackPointer[nextTask] )
        {
          currentTask = nextTask;
        }
      }
      return( stackPointer[currentTask] );
    }

  private:
    //---------------------------------------------------------------
    Rtos_Mcu::TaskHandle stackPointer[NUM_OF_TASKS];

    BYTE   currentTask;
    BYTE   nextTask;
    BYTE   lastId;

    static myRTOS *ptr;

}; //class myRTOS

//*******************************************************************
myRTOS *myRTOS::ptr = NULL;

//*******************************************************************
class myTask : public myRTOS::Task
{
  public:
    //---------------------------------------------------------------
    myTask( myRTOS &rtos )
    : myRTOS::Task( rtos )
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
        rtos.pause();  // pause the task until next time slot
      }
    }

  public:
    //---------------------------------------------------------------
    DWORD cnt;

}; //class myTask

//*******************************************************************
myRTOS  rtos ( 1000/*time slice in us*/ );
myTask  task1( rtos );
myTask  task2( rtos );

//*******************************************************************
int main(void)
{
  DWORD cnt     = 0;
  DWORD timeout = 0; /*ms*/

  char str[100];

  uart.set( "\r\n\nHwRtos," __DATE__ "," __TIME__ "\r\n\n" );

  rtos.start( task1 );

  while( 1 )
  {
    DWORD time = Rtos_Mcu::getSysTime();

    if( time >= timeout )
    {
      sprintf( str, "time:%6lu, main:%6lu, task1:%6lu, task2:%6lu\r\n",
                    (unsigned long)time,
                    (unsigned long)cnt++,
                    (unsigned long)task1.cnt,
                    (unsigned long)task2.cnt);
      uart.set( str );

      timeout += 1000;

      if( rtos.isRunning( task2 ) )
      {
        rtos.stop( task2 );
        task2.cnt = 0;
      }
      else
      {
        rtos.start( task2 );
      }
    }
  }
}
