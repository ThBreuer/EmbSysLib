//*******************************************************************
/*!
\file   UDPserver.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   31.01.2022

License: See file "LICENSE"
*/

//*******************************************************************
#include "UDPserver.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// UDPserver
//
//*******************************************************************
//-------------------------------------------------------------------
UDPserver::UDPserver( WORD port )

: report( ReportID_Hw::Module::SYSTEM_UDP_SERVER )

{
  if( wsa.startup(0x0202) )
  {
    wsa.cleanup();
    report.error( ReportID_Hw::Event::WRONG_VERSION );
    return;
  }

  if( (aSocket = wsa.socketUDP()) == INVALID_SOCKET )
  {
    return ;
  }

  char hostName[256+1] = "";

  remoteAddr = INADDR_NONE;
  remotePort = 0;

  wsa.bindUDP( aSocket, port );

  s = aSocket;

  unsigned long arg = 1;
  ioctlsocket(s, FIONBIO, &arg);
}

//-------------------------------------------------------------------
void UDPserver::add( Handler &handler )
{
  handlerList.add( &handler );
}

//-------------------------------------------------------------------
void UDPserver::run()
{
  BYTE data[10000];
  WORD plen;

  if( (plen = receive(data,10000)) > 0 )
  {
    timeOutCnt = 10000000;
    Handler *ptr = (Handler*)handlerList.getFirst();
    while( ptr )
    {
      if( ptr->update( data, plen ) )
      {
        return;
      }
      ptr = (Handler*)ptr->getNext();
    }
    data[0] = 0;
    send( data, 1 );
  }
  else
  {
    if( timeOutCnt == 0 )
    {
      Sleep(1);
    }
    else
    {
      timeOutCnt--;
    }
  }
}

//-------------------------------------------------------------------
void UDPserver::send( BYTE *buffer, WORD len )
{
  if( len > 0  )
  {
    SOCKADDR_IN addr;

    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = (DWORD)remoteAddr;
    addr.sin_port        = htons(remotePort);

    wsa.sendTo( s, (SOCKADDR *)&addr, (const char *)buffer, len );
  }
}

//-------------------------------------------------------------------
void UDPserver::sendAckn( void )
{
  const char buffer = 0;

  SOCKADDR_IN addr;

  addr.sin_family      = AF_INET;
  addr.sin_addr.s_addr = (DWORD)remoteAddr;
  addr.sin_port        = htons(remotePort);

  wsa.sendTo( s, (SOCKADDR *)&addr, &buffer, 1 );
}

//-------------------------------------------------------------------
WORD UDPserver::receive(BYTE *buffer, WORD len)
{
  SOCKADDR_IN addr;
  int         plen = 0;

  plen = wsa.receiveFrom( s, &addr, (char*)buffer, len );

  if( plen > 0 )
  {
     remoteAddr  = addr.sin_addr.S_un.S_addr;
     remotePort  = ntohs(addr.sin_port);
  }
  return( (plen<0)?0:plen );
}

}  } //namespace

//EOF
