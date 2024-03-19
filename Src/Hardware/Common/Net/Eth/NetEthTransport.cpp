//*******************************************************************
/*!
\file   NetEthTransport.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   04.02.2024

License: See file "LICENSE"
*/

//*******************************************************************
#include "NetEthTransport.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// NetEthTransport
//
//*******************************************************************
//-------------------------------------------------------------------
inline NetAddr<4> NetEthTransport::getAddr( void )
{
  return( ip.addr );
}

//---------------------------------------------------------------
inline const NetAddr<6> NetEthTransport::getAddrPhy( void )
{
  return( ip.eth.addrPhy );
}

//---------------------------------------------------------------
void NetEthTransport::process( void )
{
  Net::Task *ptr = socketList.getFirst();

  while( ptr && !ptr->onProcess() )
  {
    ptr = ptr->getNext();
  }
}

//---------------------------------------------------------------
void NetEthTransport::trigger( void )
{
  Net::Task *ptr = socketList.getFirst();

  while( ptr   )
  {
    ptr->onTrigger();
    ptr = ptr->getNext();
  }
}

//---------------------------------------------------------------
bool NetEthTransport::isPortAvailable( WORD port )
{
  Socket *ptr = (Socket *)socketList.getFirst();

  while( ptr )
  {
    ptr->onTrigger();
    if( ptr->localPort == port )
    {
      return( false );
    }
    ptr = (Socket *)ptr->getNext();
  }
  return( true );
}

}  } //namespace

//EOF
