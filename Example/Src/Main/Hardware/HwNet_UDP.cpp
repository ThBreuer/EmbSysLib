//*******************************************************************
/*!
\file   HwNet_UDP.cpp
\author Thomas Breuer
\date   06.02.2024

\brief  Sample of using hardware related class 'HwNet'
*/

//*******************************************************************
/*
Usage:    ...
*/

//*******************************************************************
#include <stdio.h>
#include <ctype.h>
#include "EmbSysLib.h"

//-------------------------------------------------------------------
#include "ReportHandler.h"
#include "configHwNet.h"

//*******************************************************************
class Application : public NetSocket::Handler
{
  public:
    //---------------------------------------------------------------
    Application( NetSocket &socket )
    {
      this->socket = &socket;
      socket.add( this );
    }

  private:
    //---------------------------------------------------------------
    virtual void onReceive( NetSocket &socketLocal )
    {
      char str[128+1];
      BYTE c;

      sprintf( str,"%s:%u <", (char *)socketLocal.getRemoteAddr(),
                                      socketLocal.getRemotePort() );
      uart.set( str );

      BYTE pos = 0;
      while( pos < 128 && socketLocal.get( c ) )
      {
        str[pos++] = toupper(c);
        uart.set( c );
      }
      str[pos] = 0;
      uart.set( "> \r");

      if( str[0] == '!' )
      {
        send();
      }
      else
      {
        socketLocal.clear();
        socketLocal.write( '!' );
        socketLocal.write( str );
        socketLocal.flush();
      }
    }

    //---------------------------------------------------------------
    virtual void onEvent( NetSocket &socketLocal, NetSocket::Event event, WORD param )
    {
      if( event == NetSocket::SOCKET_STATE )
      {
        switch( param )
        {
          case NetSocket::UNDEFINED:                                    break;
          case NetSocket::CLOSED:      uart.set("\r\nCLOSED\r\n");      break;
          case NetSocket::LISTENING:   uart.set("\r\nLISTENING\r\n");   break;
          case NetSocket::CONNECTED:   uart.set("\r\nCONNECTED\r\n");   break;
          case NetSocket::ERROR_STATE: uart.set("\r\nERROR_STATE\r\n"); break;
          default:                     uart.set("\r\nstate unkown\r\n");break;
        }
      }

      if( event == NetSocket::Event::LINK_STATE)
      {
        if( param == Ethernet::CONNECTED )
           uart.set( "\r\nLink:connected\r\n" );
        else
           uart.set( "\r\nLink:disconnected\r\n" );
      }
    }

  public:
    //---------------------------------------------------------------
    virtual void send()
    {
      if( socket )
      {
        socket->clear();
        socket->printf("Teststring %u", x++);
        socket->flush();
      }
    }


WORD cnt = 0;
  public:
    //---------------------------------------------------------------
    DWORD x = 0;
    NetSocket *socket;
};

//*******************************************************************
int main(void)
{
  uart.set( "\r\n\ncHwNet_UDP," __DATE__ "," __TIME__ "\r\n\n" );
//System::delayMilliSec(1000);
  while( net.getIP().addr[0] == 0)
  {
    net.update();
  }

  UDP_type::Socket socket( udp, NULL );
  Application app( socket );

  char txt[128];
  sprintf(txt, "UDP, %s:%u\n", (const char*)(net.getIP()), PORT );
  uart.set( txt);

  while(1)
  {
    net.update();

    switch( uart.get() )
    {
      case 's': uart.set("\r\n");
                app.send();
                break;

      case 'c': app.socket->close();
                break;

      case 'l': sprintf( txt,"\r\nlistening on port %u\r\n", PORT);
                uart.set( txt );
                app.socket->open(zeroAddr, PORT);
                break;

      case 'o': sprintf( txt,"\r\nconnecting to %s:%u\r\n", (char *)addr, PORT);
                uart.set( txt );
                app.socket->open(addr, PORT); break;
    }


//     bool value = (btnPort.get() & btnPinMask)?true:false;

//    // Set output to port
//    if( value )
//      ledPort.set( ledPinMask );
//    else
//      ledPort.clr( ledPinMask );
  }
}
