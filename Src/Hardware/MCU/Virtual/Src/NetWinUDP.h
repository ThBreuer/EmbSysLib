//*******************************************************************
/*!
\file   NetWinUDP.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   04.02.2024

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_NET_WIN_UDP_H
#define _HW_NET_WIN_UDP_H

//*******************************************************************
#include "Hardware/Common/Net/Net.h"

#include <windows.h>
#include <winsock.h>
#include <stdio.h>

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class NetWinUDP

\brief ...

\example HwNet.cpp
*/
class NetWinUDP
{
  public:
    //***************************************************************
    /*!
    \class   Socket
    \brief   ...
    \example HwNet.cpp
    */
    class Socket : public NetWin::Socket
    {
      public:
        //-----------------------------------------------------------
        /*! ...
        */
        Socket( NetWinUDP &udp, NetSocket::Handler *app );

        //-----------------------------------------------------------
        /*! ...
        */
        virtual void open( NetAddr<4> addr, WORD port );

        //-----------------------------------------------------------
        /*! ...
        */
        virtual void close( void );

        //-----------------------------------------------------------
        /*! ...
        */
        virtual void flush( void );

      private:
        //-----------------------------------------------------------
        virtual bool onProcess( void );

        //-----------------------------------------------------------
        virtual void onTrigger( void );

      private:
        //-----------------------------------------------------------
        SOCKET  sock;

    };  //class NetWinUDP::Socket
    //***************************************************************

    //---------------------------------------------------------------
    /*!
    */
    NetWinUDP( NetWin &net );

  public:
    //---------------------------------------------------------------
    NetWin &net;

}; //class NetWinUDP

}  } //namespace

#endif
