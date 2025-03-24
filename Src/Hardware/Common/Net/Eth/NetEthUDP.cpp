//*******************************************************************
/*!
\file   NetEthUDP.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   04.02.2024

License: See file "LICENSE"
*/

//*******************************************************************
#include "NetEthUDP.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// NetEthUDP
//
//*******************************************************************
//-------------------------------------------------------------------
NetEthUDP::NetEthUDP( Ethernet &net, NetEthIP &ipIn )
: NetEthTransport( net, ipIn )
{
  ip.setUDP( this );
}

//-------------------------------------------------------------------
void NetEthUDP::create( Socket &con, WORD payloadLen )
{
  NetEthUDP::Msg &msg = *(NetEthUDP::Msg*)ip.eth.buf;

  msg.udp.sourcePort = con.localPort;
  msg.udp.destPort   = con.remotePort;
  msg.udp.length     = sizeof( NetEthUDP::Header ) + payloadLen;

  ip.create( con.remoteAddrPhy,
             con.remoteAddr,
             payloadLen+sizeof(NetEthUDP::Header),
             NetEthIP::PROTOCOL_UDP );

  // Calculate checksum:
  WORD  len = sizeof( NetEthUDP::Header) + payloadLen;

  msg.udp.checksum = 0;
  msg.udp.checksum = NetEthIP::checksum( (BYTE*)&msg.ip.sourceAddr,
                                         len + 8,
                                         (DWORD)len + NetEthIP::PROTOCOL_UDP );

  ip.eth.PacketSend( ip.eth.buf,
                     sizeof( NetEthUDP::Msg) + payloadLen );
}

//*******************************************************************
//
// NetEthUDP::Socket
//
//*******************************************************************
//-------------------------------------------------------------------
NetEthUDP::Socket::Socket( NetEthUDP   &udpIn,
                           Handler     *appIn,
                           WORD        defPort )

: NetEthTransport::Socket( udpIn.net, appIn )
, udp( udpIn )

{
  defaultLocalPort = defPort; //(defPort)?defPort:udpIn.net.ip->getAvailablePort();
  isBroadcast      = false;

  remoteAddr = zeroAddr;

  stateLocal       = CLOSED;
  state            = NetSocket::CLOSED;

  udp.socketList.add( this );
}

//-------------------------------------------------------------------
void NetEthUDP::Socket::open( NetAddr<4> addr, WORD port )
{
  if( addr == zeroAddr )
  {
    isServerFlag = true;
    isBroadcast  = false;
    remoteAddrPhy = zeroAddrPhy;
    remotePort = 0;
    localPort  = port;
    stateLocal = LISTEN;
    state      = NetSocket::LISTENING;
  }
  else if( addr == broadcastAddr )
  {
    isServerFlag = false;
    isBroadcast  = true;
    remoteAddrPhy = broadcastAddrPhy;
    remotePort = port;
    localPort  = (defaultLocalPort) ? defaultLocalPort : udp.ip.getAvailablePort();
    stateLocal = OPEN;
    state      = NetSocket::CONNECTED;
  }
  else
  {
    isServerFlag = false;
    isBroadcast  = false;
    if( !(remoteAddr == addr ) )  // open with new(!) addr? => unknown PHY
    {
      remoteAddrPhy = zeroAddrPhy;
      udp.ip.arp.request( addr );
      stateLocal = WAIT_ARP; // use nextState
      state      = NetSocket::CLOSED; // use nextState !
    }
    else
    {
      stateLocal = OPEN; // use nextState
      state      = NetSocket::CONNECTED; // use nextState !
    }
    remotePort = port;
    localPort  = (defaultLocalPort) ? defaultLocalPort : udp.ip.getAvailablePort();
  }

  remoteAddr = addr;

  setTimeout( 5 );
//  isOpen = true;
}

//-------------------------------------------------------------------
void NetEthUDP::Socket::close( void )
{
  stateLocal = CLOSED;
  state = NetSocket::CLOSED;
}

//-------------------------------------------------------------------
void NetEthUDP::Socket::clear( void )
{
  if( stateLocal == OPEN || stateLocal == LISTEN )
  {
    udp.ip.eth.plen = 0;
  }
}

//-------------------------------------------------------------------
void NetEthUDP::Socket::fillBuffer( const char c )
{
  if( stateLocal == OPEN || stateLocal == LISTEN )
  {
    WORD pos = sizeof( NetEthUDP::Msg ) + udp.ip.eth.plen;

    if( pos < BUFFER_SIZE )
    {
      udp.ip.eth.buf[pos] = c;
      udp.ip.eth.plen++;
    }

    if( pos + 1 >= BUFFER_SIZE )
    {
      flush( );
    }
  }
}

//-------------------------------------------------------------------
void NetEthUDP::Socket::flush( void )
{
  if( stateLocal == OPEN || stateLocal == LISTEN )
  {
    udp.create( *this, udp.ip.eth.plen );

    udp.ip.eth.plen = 0;
  }
}

//-------------------------------------------------------------------
bool NetEthUDP::Socket::get( BYTE &x )
{
  if( stateLocal == OPEN || stateLocal == LISTEN )
  {
    if( udp.inPos < udp.inBufLen )
    {
      x = udp.buf[udp.inPos++];
      return( true );
    }
  }
  return( false );
}

//-------------------------------------------------------------------
DataPointer NetEthUDP::Socket::get( WORD maxLen )
{
  DataPointer dp;
  if( stateLocal == OPEN || stateLocal == LISTEN )
  {
    if( udp.inPos < udp.inBufLen )
    {
      WORD minLen = MIN( (WORD)(udp.inBufLen-udp.inPos), dp.getSize() );
	  dp = DataPointer( (BYTE*)&udp.buf[udp.inPos], minLen);
	  udp.inPos += minLen;
    }
  }
  return( dp );
}

//-------------------------------------------------------------------
DataPointer NetEthUDP::Socket::getDataPointer( void )
{
  const WORD offset = sizeof(NetEthUDP::Msg);
  DataPointer dp( (BYTE*)&udp.ip.eth.buf[ offset ],  BUFFER_SIZE-offset );
  return( dp );
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
bool NetEthUDP::Socket::onProcess( void )
{
  if( stateLocal == OPEN || stateLocal == LISTEN )
  {
    NetEthUDP::Msg &msg = *(NetEthUDP::Msg*)udp.ip.eth.buf;

    if( !(localPort == msg.udp.destPort) )   // todo  || !(remotePort == msg.tcp.sourcePort) || !(remoteAddr == msg.ip.sourceAddr))
    {
      return( false );
    }

    WORD  dataLen = msg.udp.length - sizeof(NetEthUDP::Header);
    BYTE *dataPtr = NULL;;

    if( dataLen > 0 )
    {
      WORD idx = sizeof( NetEthUDP::Msg );
      dataPtr  = &udp.ip.eth.buf[ idx ];
    }

    if( stateLocal == LISTEN
        && (   isServerFlag
            || remoteAddr == broadcastAddr
            || remoteAddr == msg.ip.sourceAddr ) )
    {
      remoteAddr    = msg.ip.sourceAddr;
      remotePort    = msg.udp.sourcePort;
      remoteAddrPhy = msg.eth.sourceAddr;
//      stateLocal = STATE_CONNECTED; // use nextState!
    }
    if( dataLen > 0 )
    {
      udp.buf          = dataPtr;
      udp.inPos        = 0;
      udp.inBufLen     = dataLen;

      if( !isBroadcast )
      {
        remoteAddr    = msg.ip.sourceAddr;
        remoteAddrPhy = msg.eth.sourceAddr;
        remotePort    = msg.udp.sourcePort;
      }

      if( app )
      {
        app->onReceive( *this );
      }
    }
    return( false ); // todo return true, if data are processed successfully
  }
  else
  {
    return( false );
  }
}

//-------------------------------------------------------------------
void NetEthUDP::Socket::onTrigger( void )
{
  if(      stateLocal == WAIT_ARP
      && !(remoteAddrPhy == zeroAddrPhy) )
  {
    stateLocal = OPEN;
    state = NetSocket::CONNECTED;
  }

  if( state.getUnique() )
  {
    app->onEvent( *this, SOCKET_STATE, state.get() );
  }

  linkState = udp.ip.eth.isLinked()?Ethernet::CONNECTED:Ethernet::DISCONNECTED;
  if( linkState.getUnique() )
  {
    app->onEvent( *this, LINK_STATE, linkState.get() );
  }

  if( timeOutTic && !(--timeOutTic) )
  {
    switch( stateLocal )
    {
      case WAIT_ARP:
        stateLocal = CLOSED;
        state      = NetSocket::CLOSED;
        break;
    }
  }
}

//-------------------------------------------------------------------
void NetEthUDP::handleARP( NetAddr<4> addr, NetAddr<6> addrPhy )
{
  Socket *ptr = (Socket *)socketList.getFirst();
  while( ptr )
  {
    if(    ptr->remoteAddr == addr
        && (   ptr->stateLocal == Socket::CLOSED
            || ptr->stateLocal == Socket::WAIT_ARP ) )
    {
      ptr->remoteAddrPhy = addrPhy;
      ptr->stateLocal = Socket::OPEN;
      ptr->state = NetSocket::CONNECTED;
    }
    ptr = (Socket *)ptr->getNext();
  }
}

}  } //namespace

//EOF
