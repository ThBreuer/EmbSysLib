//*******************************************************************
/*!
\file   NetEthTransport.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   04.02.2024

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_NET_ETH_TRANSPORT_H
#define _HW_NET_ETH_TRANSPORT_H

//*******************************************************************
#include "Hardware/Common/Net/Net.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class NetEthTransport
\brief Base class for transport layer (TCP, UDP)

\example HwNet_TCP.cpp, HwNet_UDP.cpp
*/
class NetEthTransport
{
  protected:
    //***************************************************************
    /*
    \class   Socket
    \brief   Socket base class
    \example HwNet_TCP.cpp, HwNet_UDP.cpp
    */
    class Socket : public NetSocket
    {
      public:
        //---------------------------------------------------------------
        Socket( Net &net, NetSocket::Handler *app )
        : NetSocket( app )
        {
          localPort        = 0;
          defaultLocalPort = 0;
          remoteAddrPhy    = zeroAddrPhy;
        }

     protected:
        //-------------------------------------------------------------------
        virtual void setTimeout( WORD timeSec, WORD timeMilli = 0 )
        {
          timeOutTic = ((DWORD)1000*timeSec + timeMilli)*Net::Task::ticsPerSecond/1000;
        }

      protected:
        //---------------------------------------------------------------
        WORD        localPort;
        WORD        defaultLocalPort;
        NetAddr<6>  remoteAddrPhy;
        DWORD       timeOutTic;

        Std::Flag<Ethernet::LinkState> linkState;

      //-----------------------------------------------------------------
      friend class NetEthTransport;

    }; // class NetEthTransport::Socket
    //***************************************************************

  protected:
    //---------------------------------------------------------------
    NetEthTransport(Ethernet &net, NetEthIP &ip )
    : net( net )
    , ip ( ip  )
    {
    }

  public:
    //---------------------------------------------------------------
    virtual NetAddr<4> getAddr( void );

    //---------------------------------------------------------------
    virtual const NetAddr<6> getAddrPhy( void );

  private:
    //---------------------------------------------------------------
    // Called by NetEthIP
    //
    void process( void );

    //---------------------------------------------------------------
    // Called by NetEthIP
    //
    void trigger( void );

    //---------------------------------------------------------------
    // Called by NetEthIP
    //
    bool isPortAvailable( WORD port );



 public:
    //---------------------------------------------------------------
    Ethernet             &net;
    NetEthIP             &ip;
    Sequence<Net::Task>   socketList;


  //-----------------------------------------------------------------
  friend class NetEthIP;

}; //class NetEthTransport

}  } //namespace

#endif
