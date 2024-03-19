//*******************************************************************
/*!
\file   NetEthICMP.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   04.02.2024

License: See file "LICENSE"
*/

//*******************************************************************
#include "NetEthICMP.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// NetEthICMP
//
//*******************************************************************
//-------------------------------------------------------------------
NetEthICMP::NetEthICMP( NetEthIP &ipIn )

: ip( ipIn )

{
  ip.setICMP( this );
}

//-------------------------------------------------------------------
void NetEthICMP::process( void )
{
  NetEthICMP::Msg &msg = *(NetEthICMP::Msg*)ip.eth.buf;

  switch( msg.icmp.type )
  {
    case TYPE_ECHOREQUEST:

      WORD dataLen    = msg.ip.packetLength;
      WORD payloadLen = dataLen - (   sizeof( NetEthIP::Header )
                                    + sizeof( NetEthICMP::Header ) );
      ip.create( msg.eth.sourceAddr,
                 msg.ip.sourceAddr,
                 payloadLen+sizeof(NetEthICMP::Header),
                 NetEthIP::PROTOCOL_ICMP );

      msg.icmp.type = TYPE_ECHOREPLY;

      msg.icmp.checksum = 0;
      msg.icmp.checksum = NetEthIP::checksum( (BYTE*)&msg.ip, dataLen, 0 );

      ip.eth.PacketSend( ip.eth.buf, sizeof( Ethernet::Header ) + dataLen );
      break;
  }
}

}  } //namespace

//EOF
