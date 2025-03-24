//*******************************************************************
/*!
\file   Ethernet.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   04.02.2024

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_ETHERNET_H
#define _HW_ETHERNET_H

//*******************************************************************
#include "Hardware/Common/Net/NetStd.h"
#include "Hardware/Common/Net/Net.h"

#include "Std/Flag.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
class NetEthARP;
class NetEthIP;

//*******************************************************************
// todo define here? NOT AS MACRO!
#define BUFFER_SIZE  1500

//*******************************************************************
/*!
\class Ethernet
\brief ...

\example ...
*/
class Ethernet : public Net
{
  public:
    //***************************************************************
    /*!
    \class Header
    \brief ...
    */
    #pragma pack(1)
    class Header
    {
      public:                        // Byte No.:
        NetAddr<6> destinationAddr;  //  0
        NetAddr<6> sourceAddr;       //  6
        NetWORD    type;             // 12
                                     // length = 14
    };
    #pragma pack(8)
    //***************************************************************

  public:
    //---------------------------------------------------------------
    /*! \enum  ...
        \brief ...
    */
    enum
    {
      TYPE_IP    = 0x0800, //!< ...
      TYPE_ARP   = 0x0806,
      TYPE_UNDEF = 0
    };

    //-----------------------------------------------------------
    /*! \enum LinkState
        \brief Link state
    */
    typedef enum
    {
      UNDEFINED = 0, //!< State undefined
      DISCONNECTED,  //!< Ethernet disconnected
      CONNECTED,     //!< Ethernet connected
    } LinkState;

  public:
    //---------------------------------------------------------------
    /*! ...
    */
    Ethernet( const NetAddr<6> &addrPhy, Timer &timer );

    //---------------------------------------------------------------
    /*! ...
    */
    virtual bool Init() = 0;

    //---------------------------------------------------------------
    /*! ...
    */
    virtual void process();

    //---------------------------------------------------------------
    /*! ...
    */
    virtual void create( const NetAddr<6> destAddr,
                         const WORD        type ) = 0;

    //---------------------------------------------------------------
    /*! ...
    */
    virtual void PacketSend( BYTE *packet,
                             WORD  len ) = 0;

    //---------------------------------------------------------------
    /*! ...
    */
    virtual bool PacketReceive( void ) = 0;

    //---------------------------------------------------------------
    /*! ...
    */
    virtual WORD getType( void ) = 0;

    //---------------------------------------------------------------
    /*! ...
    */
    virtual bool isLinked( void ) = 0;

    //---------------------------------------------------------------
    /*! ...
    */
    void setARP( NetEthARP *arpIn );

    //---------------------------------------------------------------
    /*! ...
    */
    void setIP( NetEthIP *ipIn );

    //---------------------------------------------------------------
    /*! ...
    */
    NetAddr<4> &getIP( void );

    //---------------------------------------------------------------
    /*! ...
    */
    const NetAddr<6> &getAddrPhy( void );

    //---------------------------------------------------------------
    /*! ...
    */
    bool isNewIP();

  public:
    //---------------------------------------------------------------
    BYTE              buf[BUFFER_SIZE+1];
    WORD              plen;
    const NetAddr<6> &addrPhy;

    NetEthARP   *arp;
    NetEthIP    *ip;

  private:
    //---------------------------------------------------------------
    Flag<Ethernet::LinkState>  linkState; //!< Connection state

}; //class Ethernet

}  } //namespace

#endif
