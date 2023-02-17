//*******************************************************************
/*!
\file   UDPclient.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _UDPclient_H
#define _UDPclient_H

//*******************************************************************
#include "WSA.h"

#include <winsock2.h>
#include <ws2tcpip.h>

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class UDPclient

\brief Interface to Server simulating virtual components
*/
//*******************************************************************
class UDPclient
{
  public:
    //---------------------------------------------------------------
    /*! Constructor, connects to \rev host
        \param host String with hostname:port or IP-Address:port.
        Example: "localhost:1000"
    */
    UDPclient( const char *host );

    //---------------------------------------------------------------
    /*! Send Data to Server
        \param buffer Pointer to data array
        \param len Length of data array
    */
    virtual void send( BYTE *buffer, WORD len );

    //---------------------------------------------------------------
    /*! Receive data from server.
        \warning This function blocks until data array is received
        \param buffer Pointer to data array
        \param len Size of data array
        \return Number of received bytes
    */
    WORD receive(BYTE *buffer, WORD len);

    //---------------------------------------------------------------
    /*! Wait for acknowledge byte from server.
        \warning This function blocks until acknowledge is received
    */
    void waitAckn( void );

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

}; //class UDPclient

}  } //namespace

#endif
