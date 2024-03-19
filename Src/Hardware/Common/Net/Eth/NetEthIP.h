//*******************************************************************
/*!
\file   NetEthIP.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   04.02.2024

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _NET_IP_H
#define _NET_IP_H

//*******************************************************************
#include "Hardware/Common/Ethernet/Ethernet.h"
#include "Hardware/Common/Timer/Timer.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
class NetEthICMP;
class NetEthTCP;
class NetEthUDP;

//*******************************************************************
/*!
\class NetEthIP
\brief IP class
*/
class NetEthIP : protected Net::Task
{
  public:
    //***************************************************************
    /*
    \class Header
    \brief IP header
    */
    #pragma pack(1)
    class Header
    {
      public:                            // Byte No.:
        NetBYTE    version_IHL;          //  0
        NetBYTE    TypeOfService;        //  1
        NetWORD    packetLength;         //  2
        NetWORD    identification;       //  4
        NetWORD    flags_fragmentOffset; //  6
        NetBYTE    TTL;                  //  8
        NetBYTE    protocol;             //  9
        NetWORD    headerChecksum;       // 10
        NetAddr<4> sourceAddr;           // 12
        NetAddr<4> destAddr;             // 16
        // no options                    //
                                         // length = 20

        //-----------------------------------------------------------
        // Get flags
        BYTE getFlags()
        {
          return( flags_fragmentOffset.data & 0x00FF );
        }

        //-----------------------------------------------------------
        // Set flags
        void setFlags( BYTE flags )
        {
          flags_fragmentOffset.data =   (flags_fragmentOffset.data & 0xFF00)
                                      | flags;
        }

        //-----------------------------------------------------------
        // Get fragment offset
        BYTE getFragmentOffset()
        {
          return( (flags_fragmentOffset.data & 0xFF00) >> 8 );
        }

        //-----------------------------------------------------------
        // Set fragment offset
        void setFragmentOffset( BYTE offset )
        {
          flags_fragmentOffset.data =  (flags_fragmentOffset.data & 0x00FF)
                                     | ((WORD)offset<<8);
        }

    }; //class NetEthIP::Header
    #pragma pack(8)

    //***************************************************************
    /*
    \class Msg
    \brief IP message data structur
    */
    #pragma pack(1)
    class Msg
    {
      public:                  // Byte No.:
        Ethernet::Header  eth; //   0
        NetEthIP::Header  ip;  //  14
                               // length = 34
    };
    #pragma pack(8)
    //***************************************************************

  public:
    //---------------------------------------------------------------
    enum
    {
      PROTOCOL_NONE = 0x00,
      PROTOCOL_ICMP = 0x01,
      PROTOCOL_TCP  = 0x06,
      PROTOCOL_UDP  = 0x11
    };

  public:
    //---------------------------------------------------------------
    /*! Instantiate an IP object
        \param eth Reference to an Ethernet object
        \param addr Local (own) IP address, if available. A DHCP
                    request will be called, if addr == zeroAddr (default)
    */
    NetEthIP( Ethernet &eth, const NetAddr<4> &addr = zeroAddr );

    //---------------------------------------------------------------
    void create( const NetAddr<6> destAddrPhy,
                 const NetAddr<4> destAddr,
                 const WORD       len,
                 const WORD       protocol );

    //---------------------------------------------------------------
    bool onProcess( void );

    //---------------------------------------------------------------
    void onTrigger();

    //---------------------------------------------------------------
    void setTCP( NetEthTCP *tcp );

    //---------------------------------------------------------------
    void setUDP( NetEthUDP *udp );

    //---------------------------------------------------------------
    void setICMP( NetEthICMP *icmp );

    //---------------------------------------------------------------
    bool isNewIP();

    //---------------------------------------------------------------
    NetAddr<4> &getAddr( void );

    //---------------------------------------------------------------
    WORD getAvailablePort( void );

    //---------------------------------------------------------------
    static WORD checksum( BYTE *buf, WORD len, DWORD initialSum );

  public:
    //---------------------------------------------------------------
    Ethernet       &eth;
    NetAddr<4>      addr;
    NetEthARP       arp;
    NetEthIP::Msg  &msg;
    NetEthUDP      *udp;
    NetEthTCP      *tcp;
    NetEthICMP     *icmp;
    WORD            port;
    bool            isNewIP_flag;

  private:
    //---------------------------------------------------------------
    static const BYTE  VERSION_IHL = (0x40 | 0x05); // IPv4, 5x4=20 Byte
                                                    // header length

}; //class  NetEthIP

}  } //namespace

#endif
