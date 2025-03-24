//*******************************************************************
/*!
\file   NetWinTCP.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   04.02.2024

License: See file "LICENSE"
*/

//*******************************************************************
#include "NetWinTCP.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// NetWinTCP
//
//*******************************************************************
//-------------------------------------------------------------------
NetWinTCP::NetWinTCP( NetWin &net )
: net( net )
{
}

//*******************************************************************
//
// NetWinTCP::Socket
//
//*******************************************************************
//-------------------------------------------------------------------
NetWinTCP::Socket::Socket( NetWinTCP &tcp, NetSocket::Handler *app )
: NetWin::Socket( tcp.net, app )
{
  sock  = 0;
  s     = 0;
  state = CLOSED;
  net.addTask( *this );
}

//-------------------------------------------------------------------
void NetWinTCP::Socket::open( NetAddr<4> addr, WORD port )
{
  if( addr == zeroAddr )
  {
    isServerFlag = true;
    remotePort   = 0;
  }
  else
  {
    isServerFlag = false;
    remotePort   = port;
  }
  remoteAddr = addr;

  if( sock )
  {
    closesocket( sock );
  }

  if( (sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP )) == INVALID_SOCKET )
  {
    // todo: report ERROR_INVALID_HANDLE
    state = ERROR_STATE;
    return ;
  }

  SOCKADDR_IN saddr;
  saddr.sin_family = AF_INET;

  if( isServerFlag )
  {
    // Open as a server
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port        = htons(port);
    if( bind( sock, (LPSOCKADDR)&saddr, sizeof(saddr) ) == SOCKET_ERROR )
    {
      // todo: reoprt ERROR_BIND_FAILED
      state = ERROR_STATE;
      return ;
    }

    if( listen(sock, 2) == SOCKET_ERROR )
    {
      // todo: report ERROR_LISTEN_FAILED
      state = ERROR_STATE;
      return ;
    }
    state = LISTENING;

    s = 0;

    unsigned long arg = 1;
    ioctlsocket(sock, FIONBIO, &arg);
  }
  else
  {
    // Open as a client
    s                     = sock;
    saddr.sin_addr.s_addr = (DWORD)remoteAddr;
    saddr.sin_port        = htons(port);

    if( connect(s, (SOCKADDR *)&saddr, sizeof(saddr)) == SOCKET_ERROR )
    {
      close();
      // todo: report ERROR_CONNECT_FAILED
      state = ERROR_STATE;
      return;
    }

    unsigned long arg = 1;
    ioctlsocket(s, FIONBIO, &arg);

    state = CONNECTED;
  }
}

//-------------------------------------------------------------------
void NetWinTCP::Socket::close( void )
{
  flush();
  if( s )
  {
    closesocket( s );
  }
  s               = 0;
  state = (isServerFlag)?LISTENING:CLOSED;
}

//-------------------------------------------------------------------
void NetWinTCP::Socket::flush( void )
{
  if( net.bufLen > 0 )
  {
    if( ::send( s,(const char*)net.buf, net.bufLen,0) != net.bufLen )
    {
      int err = WSAGetLastError();
      if( err != 10054 )
      {
        // todo: report ERROR_CONNECT_FAILED
        state = ERROR_STATE;
      }
    }
    net.bufIdx = 0;
    net.bufLen = 0;
  }
}

//-------------------------------------------------------------------
bool NetWinTCP::Socket::onProcess( void )
{
  if( state.getUnique() )
  {
    app->onEvent( *this, SOCKET_STATE, state.get() );
  }

  if( isServerFlag && s == 0 && sock )
  {
    SOCKADDR_IN saddr;
    int      addrLen = sizeof(SOCKADDR_IN);

    if( (int)(s = accept(sock, (SOCKADDR*)&saddr, &addrLen)) > 0 )
    {
      unsigned long arg = 1;
      ioctlsocket(s, FIONBIO, &arg);
      remoteAddr = saddr.sin_addr.S_un.S_addr;
      remotePort = ntohs(saddr.sin_port);

      state = CONNECTED;
    }
    else
    {
      s = 0;
      state = LISTENING;
    }
  }

  if( s == 0 )
  {
    return( false );
  }

  int plen = 0;

  if( (plen = recv(s, (char*)net.buf,net.bufSize,0 )) >  0 )
  {
    net.bufIdx = 0;
    net.bufLen = plen;
    if( app )
    {
      app->onReceive( *this );
    }
  }
  else if( plen < 0 || (plen == 0 && isServerFlag) )
  {
    net.bufIdx = 0;
    net.bufLen = 0;

    int err = WSAGetLastError();
    if( err != 10035 )
    {
      close();
      // todo: report ERROR_RECEIVE_FAILED
      state = ERROR_STATE;
    }
  }
  net.bufIdx = 0;
  net.bufLen = 0;
  return( false );
}

//-------------------------------------------------------------------
void NetWinTCP::Socket::onTrigger( void )
{
  if( state == ERROR_STATE )
  {
    state = CLOSED;
  }
}

}  } //namespace

//EOF
