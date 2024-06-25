//*******************************************************************
/*!
\file   NetEthUDP.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   04.02.2024

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef NET_ETH_UDP_H
#define NET_ETH_UDP_H

//*******************************************************************
#include "Hardware/Common/Net/Net.h"
#include "Hardware/Common/Net/Eth/NetEthIP.h"
#include "Hardware/Common/Net/Eth/NetEthTransport.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class NetEthUDP
\brief ...

\example HwNet_UDP.cpp
*/
class NetEthUDP : public NetEthTransport
{
  private:
    //****************************************************************
    /*
    \class Header
    \brief ...
    */
    #pragma pack(1)
    class Header
    {
      public:                // Byte No.:
        NetWORD sourcePort;  //   0
        NetWORD destPort;    //   2
        NetWORD length;      //   4
        NetWORD checksum;    //   6
                             // length = 8
    };
    #pragma pack(8)
    //***************************************************************

  private:
    //***************************************************************
    /*
    \class Msg
    \brief ...
    */
    #pragma pack(1)
    class Msg
    {
      public:                   // Byte No.:
        Ethernet::Header  eth;  //   0
        NetEthIP::Header  ip;   //  14
        NetEthUDP::Header udp;  //  34
                                // length = 42
    };
    #pragma pack(8)
    //***************************************************************

  public:
    //***************************************************************
    /*!
    \class Socket
    \brief ...

    The application can use \b NetSocket::state to obtain socket connection state.
    */
    class Socket : public NetEthTransport::Socket
    {
      private:
        //-----------------------------------------------------------
        enum
        {
          CLOSED = 0,
          WAIT_ARP,
          LISTEN,
          OPEN,
        };

      public:
        //-----------------------------------------------------------
        /*!
        */
        Socket( NetEthUDP   &udp,
                Handler     *app,
                WORD         defPort = 0 );

        //-----------------------------------------------------------
        virtual void open( NetAddr<4> addr, WORD port );

        //-----------------------------------------------------------
        virtual void close( void );

        //-----------------------------------------------------------
        virtual void clear( void );

        //-----------------------------------------------------------
        virtual void flush( void );

        //-----------------------------------------------------------
        virtual bool get( BYTE &x );

        //-----------------------------------------------------------
        virtual DataPointer get( WORD maxLen );

        //-----------------------------------------------------------
        virtual DataPointer getDataPointer( void );

      private:
        //-------------------------------------------------------------------
        virtual bool onProcess( void );

        //-------------------------------------------------------------------
        virtual void onTrigger( void );

        //-----------------------------------------------------------
        virtual void fillBuffer( const char c );

      private:
        //-------------------------------------------------------------------
        WORD        dataLen;
        BYTE       *dataPtr;
        NetEthUDP  &udp;
        BYTE        stateLocal;

        bool        isBroadcast;

      friend class NetEthUDP;

    }; //class Socket
    //***************************************************************

  public:
    //---------------------------------------------------------------
    /*! ...
    */
    NetEthUDP( Ethernet &net, NetEthIP &ip );

    //---------------------------------------------------------------
    virtual void handleARP( NetAddr<4> addr, NetAddr<6> addrPhy );

  private:
    //---------------------------------------------------------------
    void create( Socket &con, WORD payloadLen );

  private:
    //---------------------------------------------------------------
    WORD       inPos;
    WORD       inBufLen;
    BYTE      *buf;

}; //class NetEthUDP

}  } //namespace

#endif
