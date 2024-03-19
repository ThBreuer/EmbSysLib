//*******************************************************************
/*!
\file   NetEtDHCP.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   04.02.2024

License: See file "LICENSE"
*/

//*******************************************************************
#include "NetEthDHCP.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// NetEthDHCP
//
//*******************************************************************
//-------------------------------------------------------------------
NetEthDHCP::NetEthDHCP( NetEthUDP &udpIn, const char  *hostNameIn )
: addrPhy( udpIn.getAddrPhy() )
, udp    ( udpIn )
, con    ( udpIn, this, 68 )
{
  hostName    = hostNameIn;
  serverAddr  = zeroAddr;
  udp.net.addTask( *this );
  timeout = 1*Net::Task::ticsPerSecond;
}

//-------------------------------------------------------------------
void NetEthDHCP::onReceive( NetSocket &socketLocal )
{
  NetEthDHCP::Data *dhcp = (NetEthDHCP::Data*)con.getDataPointer();

  // check message
  if(    dhcp
      && dhcp->op   == 2               // DHCP reply?
      && dhcp->hlen == sizeof(addrPhy) // compare MAC-Addr
      && memcmp( (char*)&addrPhy,
                 dhcp->chAddr,
                 sizeof(addrPhy) ) == 0 )
  {
    switch( parse_options( dhcp->options, sizeof(dhcp->options) ) )
    {
      case OFFER:
        if( serverAddr == zeroAddr )
        {
          serverAddr = dhcp->siAddr;
          // todo check:         addr       = dhcp->yiAddr;
          udp.ip.addr = dhcp->yiAddr;
          con.open( broadcastAddr, 67 );
          send( REQUEST );
        }
        break;

      case ACK:
      case NAK:
        timeout = 3600*Net::Task::ticsPerSecond;

        break;
    }
  }
}

//-------------------------------------------------------------------
void NetEthDHCP::onTrigger( void )
{
  if( timeout && !(--timeout) )
  {
    if( udp.ip.addr == zeroAddr )
    {
      con.open( broadcastAddr, 67 );
      send( DISCOVER );
    }
    else
    {
      con.open( serverAddr, 67 );
      send( REQUEST );
    }
    timeout = 3*Net::Task::ticsPerSecond;
  }
}

//-------------------------------------------------------------------
BYTE NetEthDHCP::parse_options( BYTE *optptr, int len )
{
  BYTE pos  = 4; // skip magic code
  BYTE type = 0;

  while( pos < len )
  {
    switch( optptr[pos] )
    {
      case 53: // message type
        type = optptr[pos + 2];
        break;

      case 255: // option end
        return type;
    }
    pos += optptr[pos+1] + 2;
  }
  return type;
}

//-------------------------------------------------------------------
void NetEthDHCP::send( BYTE msgType )
{
  // Prepare data
  // ============
  NetEthDHCP::Data *dhcp = (NetEthDHCP::Data*)con.getDataPointer();

  memset((char*)dhcp, 0, sizeof(NetEthDHCP::Data));

  dhcp->op    = 1; // DHCP request
  dhcp->htype = 1; // hardware-type: ethernet
  dhcp->hlen  = sizeof( addrPhy );
  dhcp->hops  = 0;
  dhcp->xid   = 0x1234abcd;
  dhcp->secs  = 0;
  dhcp->flags = 0x8000; // BOOTP broadcast

  memcpy( dhcp->chAddr, &addrPhy, dhcp->hlen );

  con.clear(); //con.setDataLength(  );
  con.write( (const char *)dhcp, sizeof(NetEthDHCP::Data) - sizeof(dhcp->options) );

  // Transmit data
  // =============
  // Add DHCP options
  // ----------------
  //
  // RFC 2132: magic cookie
  con.write(  99 );
  con.write( 130 );
  con.write(  83 );
  con.write(  99 );

  // RFC 2132: DHCP Message Type
  con.write( 53 );
  con.write( 1 );
  con.write( msgType );

  // RFC 2132:  Host Name Option
  con.write( 12 );
  con.write( strlen( hostName ) );
  con.write( hostName, strlen( hostName) );

  // RFC 2132: End option
  con.write( 255 );

  con.flush();
}

}  } //namespace

//EOF
