//*******************************************************************
/*!
\file   UDPserver.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   31.01.2022

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _UDP_SERVER_H
#define _UDP_SERVER_H

//*******************************************************************
#include "WSA.h"
#include "Std/Sequence.h"

#include <winsock2.h>
#include <ws2tcpip.h>

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class UDPserver

\brief A UDP-server
*/
//*******************************************************************
class UDPserver
{
  public:
    //---------------------------------------------------------------
    class Handler : public Std::Sequence<Handler>::Item
    {
      public:
        //-----------------------------------------------------------
        Handler( UDPserver &server )
        : server( server)
        {
          server.add( *this );
        }

        //-----------------------------------------------------------
        virtual bool update( BYTE *data, WORD plen) = 0;

      public:
        //-----------------------------------------------------------
        UDPserver &server;

    }; //class Handler

    //---------------------------------------------------------------
    /*! Initialize
        \param port IP port number
    */
    UDPserver( WORD port );

    //---------------------------------------------------------------
    void add( Handler &handler );

    //---------------------------------------------------------------
    void run( void );

    //---------------------------------------------------------------
    /*! Send Data to Client
        \param buffer Pointer to data array
        \param len Length of data array
    */
    virtual void send( BYTE *buffer, WORD len );

    //---------------------------------------------------------------
    /*! Send Data to Client
        \param buffer Pointer to data array
        \param len Length of data array
    */
    virtual void sendAckn( void );

    //---------------------------------------------------------------
    /*! Receive data from client
        \warning This function blocks until data array is received
        \param buffer Pointer to data array
        \param len Size of data array
        \return Number of received bytes
    */
    WORD receive( BYTE *buffer, WORD len );

  public:
    //---------------------------------------------------------------
    Std::Report report;

  private:
    //---------------------------------------------------------------
    DWORD   remoteAddr;
    WORD    remotePort;
    SOCKET  s;
    SOCKET  aSocket;
    WSA     wsa;
    DWORD   timeOutCnt;

    Std::Sequence<Handler> handlerList;

}; //class UDPserver

}  } //namespace

#endif
