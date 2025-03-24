//*******************************************************************
/*!
\file   NetEthDHCP.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   04.02.2024

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef NET_ETH_DHCP_H
#define NET_ETH_DHCP_H

//*******************************************************************
#include "NetEthUDP.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class NetEthDHCP
\brief Class to request own IP via DHCP
*/
class NetEthDHCP : protected NetSocket::Handler, protected Net::Task
{
  public:
    //***************************************************************
    /*
    \class Data
    \brief DHCP header
    */
    #pragma pack(1)
    class Data
    {
      public:                   // Byte No.:
        NetBYTE    op;          //   0
        NetBYTE    htype;       //   1
        NetBYTE    hlen;        //   2
        NetBYTE    hops;        //   3
        NetDWORD   xid;         //   4
        NetWORD    secs;        //   8
        NetWORD    flags;       //  10
        NetAddr<4> ciAddr;      //  12
        NetAddr<4> yiAddr;      //  16
        NetAddr<4> siAddr;      //  20
        NetAddr<4> giAddr;      //  24
        NetBYTE    chAddr[16];  //  28
        NetBYTE    sname[64];   //  44
        NetBYTE    bfile[128];  // 108
        NetBYTE    options[64]; // 236
                                // length = 300
    };
    #pragma pack(8)
    //***************************************************************

  public:
    //---------------------------------------------------------------
    /*! Instatiate a DHCP object
        \param udp Reference to a NetEthUDP object
        \param hostName Local host name (optional)
    */
    NetEthDHCP( NetEthUDP &udp, const char *hostName = "" );

  private:
    //---------------------------------------------------------------
    // Override NetSocket::Handler
    virtual void onReceive( NetSocket &socketLocal );

    //---------------------------------------------------------------
    // Override NetSocket::Handler
    virtual void onEvent( NetSocket &socketLocal, NetSocket::Event event, WORD param )
    {
    }

    //---------------------------------------------------------------
    // Override Net::Task
    virtual bool onProcess( void )
    {
      return( false );
    }

    //---------------------------------------------------------------
    // Override Net::Task
    virtual void onTrigger( void );

  private:
    //---------------------------------------------------------------
    enum
    {
      DISCOVER = 1,
      OFFER    = 2,
      REQUEST  = 3,
      DECLINE  = 4,
      ACK      = 5,
      NAK      = 6,
      RELEASE  = 7,
      INFORM   = 8
    };

  private:
    //-----------------------------------------------------------------
    void send( BYTE msgType );

    //-----------------------------------------------------------------
    BYTE parse_options( BYTE *optptr, int len );

  private:
    //-----------------------------------------------------------------
    const char         *hostName;
    const NetAddr<6>    addrPhy;
    NetAddr<4>          serverAddr;
    NetEthUDP          &udp;
    NetEthUDP::Socket   con;

    DWORD timeout = 0;

}; //NetEthDHCP

}  } //namespace

#endif
