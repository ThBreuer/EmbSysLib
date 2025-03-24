//*******************************************************************
/*!
\file   Ethernet.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   04.02.2024

License: See file "LICENSE"
*/

//*******************************************************************
#include "Hardware/Common/Net/Eth/NetEthARP.h"
#include "Hardware/Common/Net/Eth/NetEthIP.h"
#include "Ethernet.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// Ethernet
//
//*******************************************************************
//-------------------------------------------------------------------
Ethernet::Ethernet( const NetAddr<6> &addrPhyIn, Timer &timer )

: Net( timer )
, addrPhy( addrPhyIn )
{
  linkState = DISCONNECTED;
  plen      = 0;
  arp       = NULL;
  ip        = NULL;
}

//-------------------------------------------------------------------
void Ethernet::process( void )
{
  linkState = isLinked()?CONNECTED:DISCONNECTED;

  if( linkState.getUnique() == CONNECTED )
  {
    Init();
  }

  if( PacketReceive( ) )
  {
    switch( getType() )
    {
      case Ethernet::TYPE_ARP: if( arp )  arp->process(); break;
      case Ethernet::TYPE_IP:  if(  ip )   ip->onProcess(); break;
    }
  }

//  if( ip )
//  {
//    ip->update();
//  }
}

//-------------------------------------------------------------------
void Ethernet::setARP( NetEthARP *arpIn )
{
  arp = arpIn;
}

//-------------------------------------------------------------------
void Ethernet::setIP( NetEthIP *ipIn )
{
  ip = ipIn;
}

//-------------------------------------------------------------------
/*const*/ NetAddr<4> &Ethernet::getIP( void )
{
	static NetAddr <4> ret = zeroAddr;

  if( ip )
    ret = ip->getAddr();
  return( ret );
}

//-------------------------------------------------------------------
const NetAddr<6> &Ethernet::getAddrPhy( void )
{
    return( addrPhy );
}

//-------------------------------------------------------------------
bool Ethernet::isNewIP()
{
  if( ip )
    return( ip->isNewIP() );
  return( false );
}

}  } //namespace

//EOF
