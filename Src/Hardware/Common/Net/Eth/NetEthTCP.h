//*******************************************************************
/*!
\file   NetEthTCP.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   04.02.2024

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _NET_ETH_TCP_H
#define _NET_ETH_TCP_H

//*******************************************************************
#include "Hardware/Common/Net/Net.h"
#include "Hardware/Common/Net/Eth/NetEthIP.h"
#include "Hardware/Common/Net/Eth/NetEthTransport.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class NetEthTCP
\brief ...

\example HwNet_TCP.cpp
*/
class NetEthTCP : public NetEthTransport
{
  private:
    //-----------------------------------------------------------
    enum
    {
      FLAG_NONE   = 0x00,
      FLAG_FIN    = 0x01,
      FLAG_SYN    = 0x02,
      FLAG_RST    = 0x04,
      FLAG_PUSH   = 0x08,
      FLAG_ACK    = 0x10,
      FLAG_URG_V  = 0x20,
      FLAG_ECE_V  = 0x40,
      FLAG_CWR_V  = 0x80
    };

  private:
    //*******************************************************************
    /*
    \class Header
    \brief ...
    */
    #pragma pack(1)
    class Header
    {
      public:
        //-----------------------------------------------------------
                                    // Byte No.:
        NetWORD  sourcePort;        //   0
        NetWORD  destPort;          //   2
        NetDWORD sequenceNumber;    //   4
        NetDWORD acknowledgeNumber; //   8
        NetWORD  dataOffset_flags;  //  12
        NetWORD  windowSize;        //  14
        NetWORD  checksum;          //  16
        NetWORD  urgentPointer;     //  18
        // no options and padding
                                    // length = 20

        //-----------------------------------------------------------
        WORD getDataOffset()
        {
          return( (WORD)((dataOffset_flags.data & 0x00FF)>>4) * 4 );
        }

        //-----------------------------------------------------------
        void setDataOffset( BYTE offset )
        {
          dataOffset_flags.data =   (dataOffset_flags.data &0xFF00)
                                  | (offset/4)<<4;
        }

        //-----------------------------------------------------------
        BYTE getFlags()
        {
          return( (dataOffset_flags.data & 0xFF00) >> 8 );
        }

        //-----------------------------------------------------------
        void setFlags( BYTE flags )
        {
          dataOffset_flags.data =   (dataOffset_flags.data &0x00FF)
                                  | ((WORD)flags<<8);
        }
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
        Ethernet::Header   eth; //   0
        NetEthIP::Header   ip;  //  14
        NetEthTCP::Header  tcp; //  34
                                //  length = 54
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
          SYN_SENT,
          SYN_RECVD,
          ESTABLISHED,
          CLOSE_WAIT,
          LAST_ACK,
          FIN_WAIT_1,
          FIN_WAIT_2,
          CLOSING,
          TIMED_WAIT,
          ERROR_STATE
        };

      public:
        //-----------------------------------------------------------
        /*!
        */
        Socket( NetEthTCP   &tcp,
                Handler     *app );

        //-----------------------------------------------------------
        virtual void open( NetAddr<4> addr,  WORD port );

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

        //-----------------------------------------------------------
        virtual void update( void );

        //-----------------------------------------------------------
        void sendFlags( BYTE flagsIn);

        //-----------------------------------------------------------
        void nextState( BYTE newState );

        //-----------------------------------------------------------
        bool isFlag( BYTE flag )
        {
          return( flags & flag );
        }

      private:
        //-----------------------------------------------------------
        WORD       flags;
        BYTE       mss;
        DWORD      initialSeqNum;
        DWORD      seqNum;

        DWORD      ackNum;
        DWORD      ackNumExpected;
        NetEthTCP &tcp;

        BYTE  stateLocal;
        bool  openReq = false;
        bool  closeReq;
        bool  timeoutReq;
        bool  isDataFlag;
        bool  busy = false;
        WORD  dataLen;
        BYTE *dataPtr;
        BYTE  statePrev;
        bool  isRemoteValid = false;

      //-------------------------------------------------------------
      friend class NetEthTCP;

    }; //class Socket
    //***************************************************************

  public:
    //---------------------------------------------------------------
    /*! ...
    */
    NetEthTCP( Ethernet  &net, NetEthIP  &ip );

    //---------------------------------------------------------------
    virtual void handleARP( NetAddr<4> addr, NetAddr<6> addrPhy );

  private:
    //---------------------------------------------------------------
    void create( Socket &con, WORD payloadLen );

  private:
    //---------------------------------------------------------------
    WORD  inPos;
    WORD  inBufLen;
    BYTE *buf;

}; // class NetEthTCP

}  } //namespace

#endif
