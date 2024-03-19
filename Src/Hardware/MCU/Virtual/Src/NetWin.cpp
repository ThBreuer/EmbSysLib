//*******************************************************************
/*!
\file   NetWint.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   04.02.2024

License: See file "LICENSE"
*/

//*******************************************************************
#include "NetWin.h"

#include <windows.h>
#include <winsock.h>

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// NetWin
//
//*******************************************************************
//-------------------------------------------------------------------
NetWin::NetWin( Timer &timer )
: Net( timer )
, report( ReportID_Hw::Module::NET_WIN_TCP )
{
  if( WSAStartup( 0x0202, &w ) )
  {
    report.error( ReportID_Hw::Event::STARTUP_FAILED );
    return;
  }
  if( w.wVersion != 0x0202 )
  {
    WSACleanup();
    report.error( ReportID_Hw::Event::WRONG_VERSION );
    return ;
  }
}

//-------------------------------------------------------------------
void NetWin::process( void )
{
  Net::Task *ptr = taskList.getFirst();

  while( ptr && !ptr->onProcess() )
  {
    ptr = ptr->getNext();
  }
}

//-------------------------------------------------------------------
NetAddr<4> &NetWin::getIP( void )
{
  static NetAddr <4> ret = zeroAddr;

  PHOSTENT hostinfo = gethostbyname("");
  if( hostinfo )
  {
    struct in_addr addr = *(struct in_addr *)*hostinfo->h_addr_list;

    ret = addr.S_un.S_addr;
  }
  return( ret );
}

}  } //namespace

// EOF
