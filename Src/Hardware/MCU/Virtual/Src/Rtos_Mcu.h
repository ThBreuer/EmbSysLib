//*******************************************************************
/*!
\file   Rtos_Mcu.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   07.03.2023

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_RTOS_MCU_H
#define _HW_RTOS_MCU_H

//*******************************************************************
#include <windows.h>

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
long int GetSysTimeMilliSec( void );
long int GetSysTimeMicroSec( void );

//*******************************************************************
/*!
\class Rtos_Mcu

\brief ...

This module provides a set of static methods to start and terminate threads

\todo Add comments

\example ...
*/
class Rtos_Mcu //: public Rtos
{
  public:
    //---------------------------------------------------------------
    typedef  HANDLE TaskHandle;

  public:
    //---------------------------------------------------------------
    /*! ...
    */
    static void init( WORD us, TaskHandle (*scheduleIn)(TaskHandle sp, BYTE flag) )
    {
      sysTicTime = us;
    }

    //---------------------------------------------------------------
    /*! ...
    */
    static void (*startFuncPtr)(void *);

    //---------------------------------------------------------------
    /*! ...
    */
    static DWORD startFuncLoc( void *arg )
    {
      startFuncPtr( arg );
      return(0);
    }

    //---------------------------------------------------------------
    /*! ...
    */
    static TaskHandle start( BYTE      *stackAddrIn,
                             unsigned   stackSizeIn,
                             PVOID      arg,
                             void     (*startFunc)(void *) )
    {
      DWORD  threadID = 0;
      HANDLE threadHandle;

      startFuncPtr = startFunc;

      threadHandle = CreateThread(  NULL,    // attributes
                                    0,       // stack size
                                    startFuncLoc, // thread function
                                    arg,     // argument to thread function
                                    0,       // creation flags
                                   &threadID );

      return( (TaskHandle)threadHandle );
    }

    //---------------------------------------------------------------
    /*! ...
    */
    static void stop( TaskHandle id )
    {
      TerminateThread((HANDLE)id, 0);
      CloseHandle((HANDLE)id);
    }

    //---------------------------------------------------------------
    /*! ...
    */
    static void pause( void )
    {
      unsigned long nextStart =  GetSysTimeMicroSec() + sysTicTime;
      Sleep( sysTicTime/1000 + 1 );
    }

		//---------------------------------------------------------------
    /*! ...
    */
    static unsigned long getSysTime( void )
		{
			return( (unsigned long)GetSysTimeMilliSec() - timeOffset);
		}

    // todo redesign, use another TimeOfTic
    //---------------------------------------------------------------
    static DWORD getTics()
    {
      return( getSysTime() );
    }

    //---------------------------------------------------------------
    static WORD getTimeOfTic( void )
    {
      return( 1000 );
    }

  private:
    //---------------------------------------------------------------
    //This is called when the task returns
    static void del_process( void )
    {
      while(1);
    }

  private:
    //---------------------------------------------------------------
    static unsigned long timeOffset;
    static unsigned long sysTicTime;

}; //class Rtos_Mcu

}  } //namespace

#endif
