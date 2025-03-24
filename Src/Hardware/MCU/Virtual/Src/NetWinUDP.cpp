//*******************************************************************
/*!
\file   NetWinUDP.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   04.02.2024

License: See file "LICENSE"
*/

//*******************************************************************
#include "NetWinUDP.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// NetWinUDP
//
//*******************************************************************
//-------------------------------------------------------------------
NetWinUDP::NetWinUDP( NetWin &net )
: net( net )
{
}

//*******************************************************************
//
// NetWinUDP::Socket
//
//*******************************************************************
//-------------------------------------------------------------------
NetWinUDP::Socket::Socket( NetWinUDP &udp, NetSocket::Handler *app )
: NetWin::Socket( udp.net, app )
{
  sock  = 0;
  state = CLOSED;
  net.addTask( *this );
}

//-------------------------------------------------------------------
void NetWinUDP::Socket::open( NetAddr<4> addr, WORD port )
{
  if( addr == zeroAddr )
  {
    isServerFlag = true;
  }
  else
  {
    isServerFlag = false;
  }

  remoteAddr = addr;
  remotePort = port;

  if( sock )
  {
    closesocket( sock );
    sock = 0;
  }

  if( (sock = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP )) == INVALID_SOCKET )
  {
    sock  = 0;
    // todo: report ERROR_INVALID_HANDLE
    state = ERROR_STATE;
    return;
  }

  if( isServerFlag )
  {
    // Open as a server
    SOCKADDR_IN saddr;
    saddr.sin_family      = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port        = htons(port);

    if( bind( sock, (LPSOCKADDR)&saddr, sizeof(saddr) ) == SOCKET_ERROR )
    {
      closesocket( sock );
      sock  = 0;
      // todo: report ERROR_BIND_FAILED
      state = ERROR_STATE;
      return ;
    }
    state = LISTENING;
  }
  else
  {
    state = CONNECTED;
  }
  unsigned long arg = 1;
  ioctlsocket( sock, FIONBIO, &arg );
}

//-------------------------------------------------------------------
// todo Does it make sense to close a server(!) socket here?
//      It is NOT consistent to TCP behaviour
void NetWinUDP::Socket::close( void )
{
  if( sock )
  {
    closesocket(sock);
    sock = 0;
  }
  state = CLOSED;
}

//-------------------------------------------------------------------
void NetWinUDP::Socket::flush( void )
{
  if( net.bufLen > 0 )
  {
    SOCKADDR_IN addr;

    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = (DWORD)remoteAddr;
    addr.sin_port        = htons(remotePort);
    if( ::sendto( sock,(const char*)net.buf, net.bufLen,0,(SOCKADDR*)&addr,sizeof(SOCKADDR_IN)) != net.bufLen )
    {
      // todo: report ERROR_SEND_FAILED
      state = ERROR_STATE;
    }
    net.bufIdx = 0;
    net.bufLen = 0;
  }
}

//-------------------------------------------------------------------
bool NetWinUDP::Socket::onProcess( void )
{
  if( sock == 0 )
  {
    return( false );
  }

  int plen = 0;

  SOCKADDR_IN tmpAddr;
  int	        tmpAddrLen = sizeof(SOCKADDR_IN);

  if( (plen = recvfrom( sock, (char*)net.buf,net.bufSize, 0, (SOCKADDR*)&tmpAddr,&tmpAddrLen)) >  0)
  {
    if( isServerFlag )
    {
      remoteAddr = tmpAddr.sin_addr.S_un.S_addr;
      remotePort = ntohs(tmpAddr.sin_port);
    }

    net.bufIdx = 0;
    net.bufLen = plen;
    if( app )
    {
      app->onReceive( *this );
    }
  }
  else if( plen < 0 )
  {
    net.bufIdx = 0;
    int err = WSAGetLastError();

    if( err != 10035 && err != 10022)
    {
      // todo: report ERROR_RECEIVE_FAILED
      state = ERROR_STATE;
    }
  }
  net.bufIdx = 0;
  
  if( state.getUnique() )
  {
    app->onEvent( *this, SOCKET_STATE, state.get() );
  }

  return( false );
}

//-------------------------------------------------------------------
void NetWinUDP::Socket::onTrigger( void )
{
}

}  } //namespace

//EOF
