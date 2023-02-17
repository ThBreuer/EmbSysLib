//*******************************************************************
/*!
\file   Report.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   10.09.2020

License: See file "LICENSE"
*/

//*******************************************************************
#include "Report.h"
#include "Hardware/MCU/System.h"

//*******************************************************************
namespace EmbSysLib {
namespace Std {

//*******************************************************************
//
// Report
//
//*******************************************************************
//-------------------------------------------------------------------
Report::Handler *Report::msgHandler   = 0;
WORD             Report::globalCode   = 0;
WORD             Report::globalModule = 0;

//-------------------------------------------------------------------
Report::Report( WORD moduleId_In )
{
  moduleId  = moduleId_In;
  localCode = 0;
}

//-------------------------------------------------------------------
void Report::store( const WORD code )
{
  if( localCode == 0 )
  {
    localCode = code;
  }
  if( globalCode == 0 )
  {
    globalCode = code;
    globalModule = moduleId;
  }
}

//-------------------------------------------------------------------
void Report::error( const WORD code )
{
  store( code );
  if( msgHandler )
  {
    msgHandler->onReport( code, moduleId, true );
  }
  else
  {
    Hw::System::reset();
  }
}

//-------------------------------------------------------------------
void Report::alert( const WORD code )
{
  store( code );
  if( msgHandler )
  {
    msgHandler->onReport( code, moduleId, false );
  }
}

//-------------------------------------------------------------------
WORD Report::getCode( void )
{
  return( localCode );
}

//-------------------------------------------------------------------
WORD Report::getCodeEvent( void )
{
  WORD c;

  Hw::System::disableInterrupt();
  c = localCode;
  localCode = 0;
  Hw::System::enableInterrupt();

  return( c );
}

//-------------------------------------------------------------------
WORD Report::getModuleId( void )
{
  return( moduleId );
}

//*******************************************************************
//
// Report::Handler
//
//*******************************************************************
//-------------------------------------------------------------------
Report::Handler::Handler()
{
  Report::msgHandler = this;
}

//-------------------------------------------------------------------
Report::Handler::~Handler()
{
  if( this == Report::msgHandler )
  {
    Report::msgHandler = 0;
  }
}

}  } //namespace

// EOF
