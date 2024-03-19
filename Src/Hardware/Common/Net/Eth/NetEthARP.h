//*******************************************************************
/*!
\file   NetEthARP.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   04.02.2024

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _NET_ETH_ARP_H
#define _NET_ETH_ARP_H

//*******************************************************************
#include "Hardware/Common/Ethernet/Ethernet.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
class NetEthTCP;
class NetEthUDP;

//*******************************************************************
/*
\class NetEthARP
\brief Address Resolution Protocol (ARP)
*/
class NetEthARP
{
  public:
    //***************************************************************
    /*
    \class Header
    \brief Data structure of ARP header
    */
    #pragma pack(1)
    class Header
    {
      public:                         // Byte No.:
        NetWORD    hardwareType;     //  0
        NetWORD    protocolType;     //  2
        NetBYTE    physicalAddrLen;  //  4
        NetBYTE    protocolAddrLen;  //  5
        NetWORD    operationCode;    //  6
        NetAddr<6> sourceAddrPhy;    //  8
        NetAddr<4> sourceAddr;       // 14
        NetAddr<6> destAddrPhy;      // 18
        NetAddr<4> destAddr;         // 24
                                     // length = 28
    };
    #pragma pack(8)
    //***************************************************************

    //***************************************************************
    /*
    \class Msg
    \brief Data structure of ARP Message
    */
    #pragma pack(1)
    class Msg
    {
      public:                   // Byte No.:
        Ethernet::Header eth;   //  0
        NetEthARP::Header arp;  // 14
                                // length = 42
    };
    #pragma pack(8)
    //***************************************************************

  public:
    //---------------------------------------------------------------
    /*  Instatiate an ARP object
        \param eth  Reference to a Net object
        \param addr Local (own) IP address
    */
    NetEthARP( Ethernet &eth, NetAddr<4> &addr );

    //---------------------------------------------------------------
    /*  Send an ARP request
        \param addr  Requested IPv4 address
    */
    void request( NetAddr<4> &addr );

    //---------------------------------------------------------------
    /*  Process received data
    */
    void process( void );

    //---------------------------------------------------------------
    void setTCP( NetEthTCP *tcp );

    //---------------------------------------------------------------
    void setUDP( NetEthUDP *udp );

  private:
    //---------------------------------------------------------------
    enum
    {
      OPCODE_REQUEST = 0x0001,
      OPCODE_REPLY   = 0x0002
    };

  private:
    //---------------------------------------------------------------
    Ethernet        &eth;
    NetAddr<4>      &addr;
    NetEthARP::Msg  &msg;
    NetEthTCP       *tcp;
    NetEthUDP       *udp;

}; //class NetEthARP

}  } //namespace

#endif
