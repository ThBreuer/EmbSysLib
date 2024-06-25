//*******************************************************************
/*!
\file   Net.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   04.02.2024

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_NET_H
#define _HW_NET_H

//*******************************************************************
#include "NetStd.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class Net
\brief Main class for network applications

This class supports access to a network and must be derived by
an suitable class that supports the hardware (see class Ethernet, NetWin, ...).

The application can specify objects to be used for processing received data.
The application has to call the update()-method.
The update method checks if data has been received. In this case,
receive data are processed by calling the onProcess() method of the associated object.
All associated object are triggered by calling the onTrigger()-method periodically after every 10 ms.

\example HwNet_TCP.cpp, HwNet_UDP.cpp
*/
class Net
{
  private:
    //***************************************************************
    class Handler : public Timer::Task
    {
      public:
        //---------------------------------------------------------
        void update()
        {
          if( tics )
          {
            tics--;
          }
        }
      public:
        //---------------------------------------------------------
        WORD tics = 0;

    } handler;
    //***************************************************************

  public:
    //***************************************************************
    /*!
    \class Task
    \brief Interface for classes that process received data and
           trigger events.
    */
    class Task : public Std::Sequence<Task>::Item
    {
      public:
        //---------------------------------------------------------
        /* Method called by Net if data are received.
        */
        virtual bool onProcess( void ) = 0;

        //---------------------------------------------------------
        /* Method called by Net when a trigger event (every 10ms)
            occurs.
        */
        virtual void onTrigger( void ) = 0;

      public:
        //---------------------------------------------------------
        static const DWORD ticsPerSecond = 100;

    }; //class Net::Task
    //***************************************************************

  public:
    //---------------------------------------------------------------
    /*! Instatiate a network object.
    */
    Net( Timer &timer )
    {
      isIPvalidFlag = false;
      timeoutTics =   1E6 / Net::Task::ticsPerSecond / timer.getCycleTime();
      timer.add( &handler );
    }

    //---------------------------------------------------------------
    /*! Ceck and process received data
    */
    virtual void update( void )
    {
      process();

      if( handler.tics == 0 )
      {
        handler.tics = timeoutTics;
        Task *ptr = taskList.getFirst();
        while( ptr )
        {
          ptr->onTrigger();
          ptr = ptr->getNext();
        }
      }
    }

    //---------------------------------------------------------------
    /*! Add an object that process data and trigger events.
        \param task Reference to a Net::Task object.
    */
    virtual void addTask( Task &task )
    {
      taskList.add( &task );
    }

  public:
    //---------------------------------------------------------------
    /*! Returns own IPv4 address
        \return address of zeroAddr if address is unkown/not defined
    */
    virtual NetAddr<4> &getIP( void ) = 0;

    //---------------------------------------------------------------
    /*! Checks, if own IP becomes valid
        \return Flag
    */
    bool isNewIP( void )
    {
      if( isIPvalidFlag )
      {
        return( false ); // valid, but not new
      }
      else
      {
        isIPvalidFlag = !(getIP() == zeroAddr);
      }
      return( isIPvalidFlag );
    }


  protected:
    //---------------------------------------------------------------
    virtual void process( void ) = 0;

  protected:
    //---------------------------------------------------------------
    Std::Sequence<Task> taskList; // list of callable objects

  private:
    //---------------------------------------------------------------
    WORD timeoutTics;
    bool isIPvalidFlag = false;

}; //class Net

}  } //namespace

#endif
