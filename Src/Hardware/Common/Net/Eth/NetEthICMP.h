//*******************************************************************
/*!
\file   NetEthICMP.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   04.02.2024

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _NET_ETH_ICMP_H
#define _NET_ETH_ICMP_H

//*******************************************************************
#include "NetEthIP.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class NetEthICMP
\brief Class to provide ICMP
*/
class NetEthICMP
{
  public:
    //***************************************************************
    /*
    \class Header
    \brief ICMP header
    */
    #pragma pack(1)
    class Header
    {
      public:             // Byte No.:
        NetBYTE type;     //  0
        NetBYTE code;     //  1
        NetWORD checksum; //  2
                          // length = 4
    };
    #pragma pack(8)

    //***************************************************************
    /*
    \class Msg
    \brief ICMP Message
    */
    #pragma pack(1)
    class Msg
    {
      public:                     // Byte No.:
        Ethernet::Header    eth;  //  0
        NetEthIP::Header     ip;   // 14
        NetEthICMP::Header  icmp; // 34
                                  // length = 38
    };
    #pragma pack(8)

  public:
    //---------------------------------------------------------------
    /*! Instantiate an ICMP object
        \param ip Reference to an IP object
    */
    NetEthICMP( NetEthIP &ip );

    //---------------------------------------------------------------
    void process( void );

  private:
    //---------------------------------------------------------------
    enum
    {
      TYPE_ECHOREPLY   = 0x00,
      TYPE_ECHOREQUEST = 0x08
    };

  private:
    //---------------------------------------------------------------
    NetEthIP &ip;

}; //class NetEthICMP

}  } //namespace

#endif
