//*******************************************************************
/*!
\file   NetEthARP.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   04.02.2024

License: See file "LICENSE"
*/

//*******************************************************************
#include "NetEthARP.h"
#include "NetEthTCP.h"
#include "NetEthUDP.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// NetEthARP
//
//*******************************************************************
//-------------------------------------------------------------------
NetEthARP::NetEthARP( Ethernet &ethIn, NetAddr<4> &addrIn )
: eth ( ethIn  )
, addr( addrIn )
, msg ( *(NetEthARP::Msg*)eth.buf )
{
  eth.setARP( this );
  tcp = NULL;
  udp = NULL;
}

//-------------------------------------------------------------------
void NetEthARP::process( void )
{
  switch( msg.arp.operationCode )
  {
    case OPCODE_REQUEST:
      if( msg.arp.destAddr == addr )
      {
        eth.create( msg.eth.sourceAddr, Ethernet::TYPE_ARP );

        msg.arp.operationCode = OPCODE_REPLY;
        msg.arp.sourceAddrPhy = eth.addrPhy;
        msg.arp.sourceAddr    = addr;
        msg.arp.destAddrPhy   = msg.arp.sourceAddrPhy;
        msg.arp.destAddr      = msg.arp.sourceAddr;

        eth.PacketSend( (BYTE*)&msg, sizeof(NetEthARP::Msg) );
      }
      break;

    case OPCODE_REPLY:
      if( tcp )
      {
        tcp->handleARP( msg.arp.sourceAddr,
                        msg.arp.sourceAddrPhy );
      }
      if( udp )
      {
        udp->handleARP( msg.arp.sourceAddr,
                        msg.arp.sourceAddrPhy );
      }
      break;
  }
}

//-------------------------------------------------------------------
void NetEthARP::request( NetAddr<4> &addrIn )
{
  eth.create( broadcastAddrPhy, Ethernet::TYPE_ARP );

  msg.arp.hardwareType    = (WORD)1;
  msg.arp.protocolType    = 0x0800;
  msg.arp.physicalAddrLen = 6;
  msg.arp.protocolAddrLen = 4;
  msg.arp.operationCode   = OPCODE_REQUEST;
  msg.arp.sourceAddrPhy   = eth.addrPhy;
  msg.arp.sourceAddr      = addr;
  msg.arp.destAddrPhy     = broadcastAddrPhy;
  msg.arp.destAddr        = addrIn;

  eth.PacketSend( (BYTE*)&msg,  sizeof(NetEthARP::Msg) );
}

//-------------------------------------------------------------------
void NetEthARP::setTCP( NetEthTCP *tcpIn )
{
  tcp = tcpIn;
}

//-------------------------------------------------------------------
void NetEthARP::setUDP( NetEthUDP *udpIn )
{
  udp = udpIn;
}

}  } //namespace

//EOF
