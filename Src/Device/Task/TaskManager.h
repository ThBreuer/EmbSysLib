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
class TaskManager
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
    //---------------------------------------------------------------
    /*! Initialize a TaskManager
        \param timer Timer object
    */
    TaskManager( Timer &timer )
    : timer( timer )
    {
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
        \return Cyycle in micro seconds (us)
    */
    virtual DWORD getCycleTime( void )
    {
      return( timer.getCycleTime() );
    }

  private:
    //---------------------------------------------------------------
    Timer &timer;

}; //class TaskManager

} } //namespace

#endif
