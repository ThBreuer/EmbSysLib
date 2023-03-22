//*******************************************************************
/*!
\file   StdReport.cpp
\author Thomas Breuer
\date   13.09.2022
\brief  Sample
*/

//*******************************************************************
/*
Configuration:
          Uart

Usage:    Two instances of a Test class send some messages to their Report objects.
          Output:
            - During 'running' the Report::Handler will print the received messages
            - Finally, registered messages (at global and module scope) are displayed
*/

//*******************************************************************
#include <stdio.h>
#include "EmbSysLib.h"

//-------------------------------------------------------------------
#include "config.h"

//*******************************************************************
/*
   Define an optional Report::Handler.
   If no Report::Handler is defined or instantiated, an error message will cause a reset.
*/
class myReportHandler : public EmbSysLib::Std::Report::Handler
{
  virtual void onReport(const WORD code,const WORD module,const bool isFatal)
  {
    char str[128];

    sprintf(str, "%s: Module: 0x%04X, Code: 0x%04X\r\n", isFatal?"Error":"Alert", module, code );

    uart.set( str );
  }
} reportHandler; // Instantiate a Report::Handler object. This the object is automatically registered at Report class

//*******************************************************************
class Test
{
  public:
    Test( WORD moduleId )
    : report( moduleId )
    {
    }

    // do something and alert
    void functionWithAlert( WORD code )
    {
      report.alert( 0xA000 | code );
    }

    // do something and report an error
    void functionWithError( void )
    {
      report.error( 0xE000 );
    }

    Report report;
};

//*******************************************************************
int main(void)
{
  uart.set( "\r\n\nStdReport," __DATE__ "," __TIME__ "\r\n\n" );
  uart.set( "running ...\r\n" );

  // Instantiate Test objects with individual modueId's
  Test modA( 0xF00A );
  Test modB( 0xF00B );

  // Send an alert or an error message to Report
  modA.functionWithAlert( 1 );
  modA.functionWithAlert( 2 );
  modA.functionWithError(   );
  modB.functionWithAlert( 3 );

  // Print report
  char str[80];
  uart.set( "\nsummary:\r\n" );

  sprintf(str, "global -> Module: 0x%04X  Code: 0x%04X \r\n", Report::globalModule, Report::globalCode);
  uart.set( str );

  sprintf(str, "local  -> Module: 0x%04X  Code: 0x%04X \r\n", modA.report.getModuleId(), modA.report.getCode());
  uart.set( str );

  sprintf(str, "local  -> Module: 0x%04X  Code: 0x%04X \r\n", modB.report.getModuleId(), modB.report.getCode());
  uart.set( str );

  while( 1 )
  {
  }
}
