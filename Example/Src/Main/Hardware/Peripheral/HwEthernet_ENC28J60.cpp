//*******************************************************************
/*!
\file   HwEthernet_ENC28J60.cpp
\author Thomas Breuer
\date   24.06.2024
\brief  Sample of using hardware related classes
*/

//*******************************************************************
/*
Usage:    ...


*/

//*******************************************************************
#include <stdio.h>
#include "EmbSysLib.h"

//-------------------------------------------------------------------
#include "ReportHandler.h"
#include "configHw.h"

using namespace EmbSysLib::Dev;

Terminal   terminal( uart, 255,255, "erw" );

//==================================================
#include "Hardware/Peripheral/Ethernet/Ethernet_ENC28J60.cpp"

NetAddr<6> addrPhy = {0x10,0x20,0x68,0x01,0x02,0x30};

//===================================================
Ethernet_ENC28J60 net(addrPhy, spiENC28J60, timerSlow);

NetEthIP   ip   ( net /*, myIP*/ );
NetEthICMP icmp ( ip );
NetEthUDP  udp  ( net, ip );
NetEthDHCP dhcp ( udp, "EmbSysLib" );

//*******************************************************************
int main(void)
{
  terminal.printf( "\r\n\nHwEthernet_ENC28J60," __DATE__ "," __TIME__ "\r\n\n" );

  while( 1 )
  {
    net.update();
    if( net.isNewIP() )
    {
      terminal.printf("IP: %s\r\n", (const char*)(net.getIP()) );
    }
  }
}
