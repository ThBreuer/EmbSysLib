//*******************************************************************
/*!
\file   NetEthIP.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   04.02.2024

License: See file "LICENSE"
*/

//*******************************************************************
#include "NetEthIP.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// NetEthIP
//
//*******************************************************************
//-------------------------------------------------------------------
NetEthIP::NetEthIP( Ethernet          &ethIn,
                    const NetAddr<4>  &addrIn )

: eth ( ethIn  ),
  addr( addrIn ),
  arp ( ethIn, addr ),
  msg ( *(NetEthIP::Msg*)eth.buf )
{
  isNewIP_flag = false;

  tcp  = NULL;
  udp  = NULL;
  icmp = NULL;
  port = 50000;

  eth.setIP( this );
  eth.addTask( *this );
}

//-------------------------------------------------------------------
bool NetEthIP::onProcess( void )
{
  if( msg.ip.version_IHL == VERSION_IHL )
  {
    if(    msg.ip.destAddr == addr
        || msg.ip.protocol == PROTOCOL_UDP )
    {
      switch( msg.ip.protocol )
      {
        case NetEthIP::PROTOCOL_TCP:  if(  tcp ) {  tcp->process(); } break;
        case NetEthIP::PROTOCOL_UDP:  if(  udp ) {  udp->process(); } break;
        case NetEthIP::PROTOCOL_ICMP: if( icmp ) { icmp->process(); } break;
      }
    }
  }
  return( false );
}

//-------------------------------------------------------------------
void NetEthIP::onTrigger( void )
{
  if(  tcp ) {  tcp->trigger(); }
  if(  udp ) {  udp->trigger(); }
}

//-------------------------------------------------------------------
void NetEthIP::create( const NetAddr<6>  destAddrPhy,
                       const NetAddr<4>  destAddr,
                       const WORD        len,
                       const WORD        protocol )
{
  eth.create( destAddrPhy, Ethernet::TYPE_IP );

  msg.ip.version_IHL    = VERSION_IHL;
  msg.ip.TypeOfService  = 0;
  msg.ip.packetLength   = sizeof( NetEthIP::Header ) + len;
  msg.ip.identification = 123;
  msg.ip.TTL            = 64;
  msg.ip.protocol       = protocol;
  msg.ip.sourceAddr     = addr;
  msg.ip.destAddr       = destAddr;

  msg.ip.setFlags         ( 0x40 ); // don't fragment
  msg.ip.setFragmentOffset( 0x00 );

  msg.ip.headerChecksum = 0;
  msg.ip.headerChecksum = NetEthIP::checksum( (BYTE*)&msg.ip,
                                              sizeof( NetEthIP::Header ),
                                              0 );
}

//-------------------------------------------------------------------
inline void NetEthIP::setTCP( NetEthTCP *tcpIn )
{
  tcp = tcpIn;
  arp.setTCP( tcpIn );
}

//-------------------------------------------------------------------
inline void NetEthIP::setUDP( NetEthUDP *udpIn )
{
  udp = udpIn;
  arp.setUDP( udpIn );
}

//-------------------------------------------------------------------
inline void NetEthIP::setICMP( NetEthICMP *icmpIn )
{
  icmp = icmpIn;
}

//-------------------------------------------------------------------
bool NetEthIP::isNewIP()
{
  if( addr.addr[0] != 0 && !isNewIP_flag )
  {
    isNewIP_flag = true;
    return( true );
  }
  return( false );
}

//-------------------------------------------------------------------
inline NetAddr<4> &NetEthIP::getAddr( void )
{
  return( addr );
}

//-------------------------------------------------------------------
WORD NetEthIP::getAvailablePort( void )
{
  WORD p = port;
  do
  {
    p = ( p == 0xFFFF ) ? 50000 : p+1;
    if(    (udp == NULL || udp->isPortAvailable( p ))
        && (tcp == NULL || tcp->isPortAvailable( p )) )
    {
      port = p;
      return( port );
    }
  }
  while( p != port );

  // \todo report fatal error
  return( port );
}

///-------------------------------------------------------------------
/// This implementation is a port of
/// https://github.com/renatoaloi/EtherEncLib, file: checksum.c
///
/// See also: RFC 1071 - Computing the Internet checksum (https://www.rfc-editor.org/info/rfc1071)
///
WORD NetEthIP::checksum( BYTE *buf, WORD len, DWORD initialSum = 0 )
{
   DWORD sum = initialSum;

   // build the sum of 16bit words
   while(len > 1)
   {
      sum += (((WORD)(*buf) << 8) | *(buf+1));
      buf += 2;
      len -= 2;
   }
   // if there is a byte left then add it (padded with zero)
   if (len > 0)
   {
      sum += ((WORD)(*buf) << 8);
   }
   // now calculate the sum over the bytes in the sum
   // until the result is only 16bit long
   while (sum >> 16)
   {
      sum = (sum & 0xFFFF) + (sum >> 16);
   }
   // build 1's complement:
   return ( ~(WORD)sum );
}

}  } //namespace

//EOF
