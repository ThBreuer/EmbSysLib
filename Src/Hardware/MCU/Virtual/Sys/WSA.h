//*******************************************************************
/*!
\file   WSA.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   18.11.2021

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _WSA_H
#define _WSA_H

//*******************************************************************
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <ctype.h>

#include "Std/Report.h"
#include "Hardware/ReportID_Hw.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class WSA

\brief Wrapper for 'Winsock' functions (Windows)
*/
//*******************************************************************
class WSA
{
  public:
    //---------------------------------------------------------------
    /*! Constructor
    */
    WSA( void )
    : report( ReportID_Hw::Module::SYSTEM_WSA )
    {
    }

    //---------------------------------------------------------------
    virtual bool startup( WORD version )
    {
      WSADATA w;

      if( WSAStartup(version, &w) )
      {
        report.error( WSAGetLastError() );
        return 1;
      }
      return( w.wVersion != version );
    }

    //---------------------------------------------------------------
    virtual void cleanup( void )
    {
      WSACleanup ();
    }

    //---------------------------------------------------------------
    SOCKET socketUDP( void )
    {
      SOCKET s;

      if( (s = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP )) == INVALID_SOCKET )
      {
        report.error( WSAGetLastError() );
        return INVALID_SOCKET;
      }
      return( s );
    }

    //---------------------------------------------------------------
    bool bindUDP( SOCKET s, WORD port )
    {
      SOCKADDR_IN saddr;

      saddr.sin_family      = AF_INET;
      saddr.sin_addr.s_addr = htonl(INADDR_ANY);
      saddr.sin_port        = htons(port);

      if( bind( s, (LPSOCKADDR)&saddr, sizeof(saddr) ) == SOCKET_ERROR )
      {
         report.error( WSAGetLastError() );
         return false;
       }
       return( true );
    }

    //---------------------------------------------------------------
    DWORD getHostByName( const char *hostName )
    {
      DWORD           remoteAddr = INADDR_NONE;
      struct hostent *h;

      if( (h = gethostbyname(hostName)) != 0 )
      {
        if( h->h_addr_list[0]!=0 )
        {
          remoteAddr = *(u_long *) h->h_addr_list[0];
        }
      }
      else
      {
        report.error( WSAGetLastError() );
        remoteAddr = INADDR_NONE;
      }
      return( remoteAddr );
    }

    //---------------------------------------------------------------
    bool sendTo( SOCKET &s, SOCKADDR *addr, const char* buffer, DWORD len )
    {
        if( sendto( s, buffer, len, 0, addr, sizeof(SOCKADDR_IN) ) != len )
        {
          report.alert( WSAGetLastError() );
          return 1;
        }
        return 0;
    }

    //---------------------------------------------------------------
    int receiveFrom( SOCKET &s, SOCKADDR_IN *addr, char* buffer, DWORD len )
    {
      int plen       = 0;
      int	tmpAddrLen = sizeof(SOCKADDR_IN);

      if( (plen = recvfrom(s, (char*)buffer, len, 0, (SOCKADDR*)addr,&tmpAddrLen) ) >  0 )
      {
        return( plen );
      }
      else if( plen < 0 )
      {
        int err = WSAGetLastError();

        if( err != 10035 && err != 10022 )
        {
          report.error( err );
        }
      }
      return( (plen<0)?0:plen );
    }

  public:
    //---------------------------------------------------------------
    Std::Report report;

}; // WSA

}  } //namespace

#endif
