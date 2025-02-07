//*******************************************************************
/*!
\file   Rtos.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   13.03.2023

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _MOD_RTOS_H
#define _MOD_RTOS_H

//*******************************************************************
#include "Mcu_Types.h"
#include "Src/Rtos_Mcu.h"

//*******************************************************************
namespace EmbSysLib {
namespace Mod {

using namespace EmbSysLib::Hw;

//*******************************************************************
/*!
\class Rtos

\brief ...

\todo Add comments
\todo Add error reports

\example ModRtos.cpp
*/
//*******************************************************************
class Rtos
{
  private:
    //---------------------------------------------------------------
    class TCB
    {
      public:
        //-----------------------------------------------------------
        typedef enum
        {
          NO_TASK = 0, // No instantiated task
          READY   = 1, // A task is available but not running
          RUNNING = 2  // The task is running
        } State;

      public:
        //-----------------------------------------------------------
        TCB()
        {
          stackPointer = NULL;
          stack        = NULL;
          stackSize    = 0;
          state        = NO_TASK;
        }

        Rtos_Mcu::TaskHandle stackPointer;

        BYTE  *stack;
        DWORD stackSize;
        State state;
    };

  private:
    //---------------------------------------------------------------
    TCB *tcb;

    BYTE   currentTask;
    BYTE   nextTask;
    BYTE   numOfTasks;

  public:
    //---------------------------------------------------------------
    class Task
    {
      public:
        //-----------------------------------------------------------
        /*! ...
        */
        Task( Rtos &rtos, DWORD stackSize )
        : rtos( rtos )
        {
          id = rtos.add( *this, stackSize );
        }

        //-----------------------------------------------------------
        ~Task( void )
        {
          rtos.remove( *this );
        }

        //-----------------------------------------------------------
        /*! ...
        */
        void start()
        {
          rtos.start( *this );
        }

        //-----------------------------------------------------------
        /*! ...
        */
        void stop()
        {
          rtos.stop( *this );
        }

        //-----------------------------------------------------------
        /*! ...
        */
        void pause()
        {
          rtos.pause(  );
        }

        //-----------------------------------------------------------
        /*! ...
        */
        bool isRunning()
        {
          return( rtos.isRunning( *this ) );
        }

      private:
        //-----------------------------------------------------------
        static void starter( void *ptr )
        {
          ((Rtos::Task*)ptr)->update();
          //!< \todo Explicitly stop the task here?
          while(1); // The task should never end
        }

        //-----------------------------------------------------------
        virtual void update( void ) = 0;

      private:
        //-----------------------------------------------------------
        BYTE id;
        Rtos &rtos;

      //-------------------------------------------------------------
      friend class Rtos;

    }; // class Task

  public:
    //*******************************************************************
    /*!
    \class Clock

    \brief Timing features within a task

    The Clock uses the timer tics, which are provided by a task object.
    */
    class Clock : public Std::Clock
    {
      public:
        //---------------------------------------------------------------
        /*! Instantiate a timer
            The runtime will be stored in the object. Further calls to
            \a start() or \a timeout() without parameter will use this stored runtime.
            \param taskHandler Reference to a task handler, which
                               provides the timer tics
            \param timeToWait_msec Runtime in miliseconds (ms)
            \param timeToWait_usec Runtime in microseconds (us)
        */
        Clock( WORD timeToWait_msec = 0,
               WORD timeToWait_usec = 0 );

      private:
        //---------------------------------------------------------------
        virtual LWORD getTics( void );

        //---------------------------------------------------------------
        virtual DWORD getTimeOfTic( void );

      private:
        //---------------------------------------------------------------
        //Rtos &taskHandler;

    }; //Clock

  public:
    //---------------------------------------------------------------
    /*! ...
    */
    Rtos( BYTE numOfTasks, WORD us )
    {
      this->numOfTasks = numOfTasks;
      tcb              = new TCB[numOfTasks];
      ptr              = this;
      currentTask      = 0;

       Rtos_Mcu::init( us, schedule );
    }

    //---------------------------------------------------------------
    /*! ...
    */
    void start( Task &task )
    {
      if( task.id != (BYTE)(-1) )
      {
        Rtos_Mcu::TaskHandle sp = Rtos_Mcu::start( tcb[task.id].stack,
                                                   tcb[task.id].stackSize,
                                                  &task,
                                                   task.starter );
        tcb[task.id].stackPointer = sp;
        tcb[task.id].state        = TCB::RUNNING;
      }
    }

    //---------------------------------------------------------------
    /*! ...
    */
    void stop( Task &task )
    {
      if( task.id != (BYTE)(-1) )
      {
        //!< \todo funktioniert nicht, wenn currentTask == task.id ?
        if( tcb[task.id].state == TCB::RUNNING )
        {
          Rtos_Mcu::stop( tcb[task.id].stackPointer);
          tcb[task.id].state        = TCB::READY;
          tcb[task.id].stackPointer = NULL;
        }
      }
    }

    //---------------------------------------------------------------
    /*! ...
    */
    void pause()
    {
      Rtos_Mcu::pause();
    }

    //---------------------------------------------------------------
    /*! ...
    */
    BYTE isRunning( Task &task )
    {
      if( task.id != (BYTE)(-1) )
      {
        return( tcb[task.id].state == TCB::RUNNING );
      }
      return( false );
    }

  private:
    //---------------------------------------------------------------
    BYTE add( Task &task, DWORD stackSize )
    {
      for( BYTE i = 1; i < numOfTasks; i++ )
      {
        if( tcb[i].state == TCB::NO_TASK )
        {
          tcb[i].stackSize    = stackSize;
          tcb[i].stack        = new BYTE[stackSize];
          tcb[i].stackPointer = NULL;
          tcb[i].state        = TCB::READY;
          return( i );
        }
      }
      return( -1 );
    }

    //---------------------------------------------------------------
    void remove( Task &task )
    {
      if( task.id != (BYTE)(-1) )
      {
        tcb[task.id].state        = TCB::NO_TASK;
        tcb[task.id].stackPointer = NULL;
        delete tcb[task.id].stack;
      }
    }

    //---------------------------------------------------------------
    static Rtos_Mcu::TaskHandle schedule( Rtos_Mcu::TaskHandle stackPtr, BYTE flag )
    {
      return( ptr->getNextTask(stackPtr, flag) );
    }

    //---------------------------------------------------------------
    Rtos_Mcu::TaskHandle getNextTask( Rtos_Mcu::TaskHandle stackPtr, bool isSysTic)
    {
      tcb[currentTask].stackPointer = stackPtr;

      currentTask               = 0;

      if( isSysTic ) // called by timeout
      {
        nextTask = (nextTask+1) % numOfTasks;
        if( tcb[nextTask].stackPointer )
        {
          currentTask = nextTask;
        }
      }
      return( tcb[currentTask].stackPointer );
    }

  private:
    //---------------------------------------------------------------
    static Rtos *ptr;

};

} } //namespace

#endif
