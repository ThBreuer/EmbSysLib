//*******************************************************************
/*!
\file   UDPclient.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   10.09.2020

License: See file "LICENSE"
*/

//*******************************************************************
#include "UDPclient.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// UDPclient
//
//*******************************************************************
//-------------------------------------------------------------------
UDPclient::UDPclient( const char *host )

: report( ReportID_Hw::Module::SYSTEM_UDP_CLIENT )

{
  if( wsa.startup(0x0202) )
  {
    wsa.cleanup();
    report.error( ReportID_Hw::Event::WRONG_VERSION );
    return;
  }

  if( (aSocket = wsa.socketUDP()) == INVALID_SOCKET )
  {
    return;
  }

  char hostName[256+1] = "";

  remoteAddr = INADDR_NONE;
  remotePort = 0;

  sscanf( host, "%256[^:]:%u", hostName, &remotePort );

  if( isalpha(hostName[0]) ) // host address is a name
  {
    remoteAddr = wsa.getHostByName( hostName );
  }
  else // host address is an IP-Number
  {
      remoteAddr = inet_addr( hostName );
  }

  if( remoteAddr == INADDR_NONE )
  {
    report.error( ReportID_Hw::Event::VALUE_OUT_OF_RANGE );
  }

  s = aSocket;
}

//-------------------------------------------------------------------
void UDPclient::send( BYTE *buffer, WORD len )
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
WORD UDPclient::receive(BYTE *buffer, WORD len)
{
  SOCKADDR_IN addr;

  return wsa.receiveFrom( s, &addr, (char *)buffer, len );
}

//-------------------------------------------------------------------
void UDPclient::waitAckn( void )
{
  SOCKADDR_IN addr;
  char        buffer;

  wsa.receiveFrom( s, &addr, &buffer, 1 );
}

}  } //namespace

//EOF
