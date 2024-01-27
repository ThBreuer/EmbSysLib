//*******************************************************************
/*!
\file   TaskManager.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   31.01.2023

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _DEV_TASK_MANAGER_H
#define _DEV_TASK_MANAGER_H

//*******************************************************************
#include "Hardware/Common/Timer/Timer.h"

//*******************************************************************
namespace EmbSysLib {
namespace Dev {

//*******************************************************************
/*!
\class   TaskManager

\brief   Manages a list of tasks and calls their update() method

This base class contains an update-list, to which objects can be
added if derived from \a TaskManager::Task. The update()-methods of all
added objects are successively called by each timer interrupt.

\example DevTaskManager.cpp
*/
class TaskManager : public Timer::Task
{
  public:
    //*******************************************************************
    /*!
    \class Task
    \brief Base class for a Task
    */
    class Task : public Timer::Task
    {
      public:
        //---------------------------------------------------------
        /*! Method called by TaskManager.
            The application has to override this method.
        */
        virtual void update( void ) = 0;

    }; //class TaskManager::Task
    //*******************************************************************

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
        /*! Instantiate a Clock
            The runtime will be stored in the object. Further calls to
            \a start() or \a timeout() without parameter will use this stored runtime.
            \param taskHandler Reference to a task handler, which
                               provides the timer tics
            \param timeToWait_msec Runtime in miliseconds (ms)
            \param timeToWait_usec Runtime in microseconds (us)
        */
        Clock( TaskManager &taskHandler,
               DWORD        timeToWait_msec = 0,
               WORD         timeToWait_usec = 0 );

      private:
        //---------------------------------------------------------------
        virtual LWORD getTics( void );

        //---------------------------------------------------------------
        virtual DWORD getTimeOfTic( void );

      private:
        //---------------------------------------------------------------
        TaskManager &taskManager;

    }; //class TaskManager::Clock
    //*******************************************************************

  public:
    //---------------------------------------------------------------
    /*! Initialize a TaskManager
        \param timer Timer object
    */
    TaskManager( Timer &timer )
    : timer( timer )
    {
      cycleTime = timer.getCycleTime();
      tics      = 0;
      timer.add( this );
    }

  public:
    //---------------------------------------------------------------
    /*! Add a new object to the task list.
        The objects update()-method will be called periodically by
        a timer interrupt.
        \remark The update()-method of the TaskManager::Task may be
        called immediately, so call this add()-method only when the constructor of the Task-object is completed.
        \param ptr Pointer to the TaskManager::Task object
    */
    virtual void add( Task *ptr )
    {
      timer.add( ptr );
    }

    //---------------------------------------------------------------
    /*! Get cycle time of update() calls
        \return Cycle time in mikroseconds (us)
    */
    virtual DWORD getCycleTime( void )
    {
      return( cycleTime );
    }

    //---------------------------------------------------------------
    /*! Returns the number of tics resp. number of cycles, the task
        handler is called
        \return Number of tics
    */
    virtual LWORD getTics( void )
    {
      return( tics );
    }

  private:
    //---------------------------------------------------------------
    virtual void update( void )
    {
      tics++;
    }

  private:
    //---------------------------------------------------------------
    Timer &timer;
    LWORD  tics;
    DWORD  cycleTime;

}; //class TaskManager

} } //namespace

#endif
