//*******************************************************************
/*!
\file   DevTaskManager.cpp
\author Thomas Breuer
\date   01.02.2023
\brief  Sample of using class Digital
*/

//*******************************************************************
/*
Usage:    Connect board with a terminal.

          The timer driven task is running, see terminal output.
*/

//*******************************************************************
#include <stdio.h>
#include "EmbSysLib.h"

//-------------------------------------------------------------------
#include "ReportHandler.h"
#include "config.h"

//*******************************************************************
class cTest : public TaskManager::Task
{
  public:
    //---------------------------------------------------------------
    cTest( TaskManager &taskManager )
    {
      cnt = 0;
      taskManager.add( this );
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
  terminal.printf( "\r\n\nDevTaskManager," __DATE__ "," __TIME__ "\r\n\n" );

  cTest test( taskManager );

  while( 1 )
  {
    terminal.printf( "cnt: %5u\r", test.cnt );
  }
}
