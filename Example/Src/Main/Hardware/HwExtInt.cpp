//*******************************************************************
/*!
\file   HwExtInt.cpp
\author Thomas Breuer
\date   06.07.2022
\brief  Sample of using hardware related class "ExtInt"
*/

//*******************************************************************
/*
Usage:    The update()-Method is executed within an external interrupt,
          triggered by falling or rising edge at btnA's port pin.
          The number of events is displayed.
*/

//*******************************************************************
#include <stdio.h>
#include "EmbSysLib.h"

//-------------------------------------------------------------------
#include "ReportHandler.h"
#include "configHw.h"

//*******************************************************************
class cTest : public ExtInt::Task
{
  public:
    //---------------------------------------------------------------
    cTest( ExtInt &extInt )
    {
      cnt = 0;
      extInt.add( this );
    }

    //---------------------------------------------------------------
    virtual void update( void )
    {
      cnt++;
    }

    //---------------------------------------------------------------
    DWORD cnt;
};

//*******************************************************************
int main(void)
{
  uart.set( "\r\n\nHwExtInt," __DATE__ "," __TIME__ "\r\n\n" );

  cTest test( extInt );

  extInt.enable();
  extInt.setEdge( extInt.BOTH );
  
  while( 1 )
  {
    char str[60];
    sprintf( str, "cnt: %5lu\r\n", (unsigned long)test.cnt );
    uart.set( str );
  }
}
