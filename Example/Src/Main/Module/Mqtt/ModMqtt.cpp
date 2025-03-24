//*******************************************************************
/*!
\file   ModMqtt.cpp
\author Thomas Breuer
\date   06.02.2024

\brief

\todo Add comment
*/

//*******************************************************************
/*
Usage:    ...
*/

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//
// SETUP:
// ======

/// Select Qos and timeout:
///------------------------
const unsigned qos     = 0;
const unsigned timeout = 10;
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//*******************************************************************
#include <stdio.h>
#include <ctype.h>
#include "EmbSysLib.h"

using namespace EmbSysLib::Std;
using namespace EmbSysLib::Dev;
using namespace EmbSysLib::Ctrl;
using namespace EmbSysLib::Mod;

//-------------------------------------------------------------------
#include "ReportHandler.h"
#include "configHwNet.h"

#include "Module/MQTT/Mqtt.h"

//-------------------------------------------------------------------
Terminal     terminal (uart, 255,255);

//*******************************************************************
class Application : public MqttInterf::Handler
{
  public:
    //---------------------------------------------------------------
    Application(MqttInterf &mqtt)
    {
      mqtt.add( this );
    }

  private:
    //---------------------------------------------------------------
    virtual void onPublishReceived( WORD pid, BYTE control, DataPointer topic, DataPointer payload )
    {
      terminal.printf("rec: PUBLISH '%.*s' [%u] '%.*s' [%u]\r\n",
                        topic.getSize(),topic.getPtr(),
                        topic.getSize(),
                        payload.getSize(),payload.getPtr(),
                        payload.getSize()
                      );
    }

    //---------------------------------------------------------------
    virtual void onControlReceived( MqttInterf::ControlPacketType type, WORD pid, BYTE rc, BYTE flags )
    {
      const char *str = "unknown";
      switch( type )
      {
        case MqttInterf::CONNACK: str = "CONNACK";  break;
        case MqttInterf::PUBACK:  str = "PUBACK";   break;
        case MqttInterf::PUBREC:  str = "PUBREC";   break;
        case MqttInterf::PUBREL:  str = "PUBREL";   break;
        case MqttInterf::PUBCOMP: str = "PUBCOMP";  break;
        case MqttInterf::SUBACK:  str = "SUBACK";   break;
        case MqttInterf::UNSUBACK:str = "UNSUBACK"; break;
        case MqttInterf::PINGRESP:str = "PINGRESP"; break;
        default:                                    break;
      }
      terminal.printf("rec: %s (PID=%u RC=%u FLAGS=%u)\r\n", str, pid, rc, flags );
    }

    //---------------------------------------------------------------
    virtual void onTrigger()
    {
    }
};

//*******************************************************************
Mqtt<TCP_type>   mqtt( tcp );
Application      app( mqtt );

//*******************************************************************
int main(void)
{
  terminal.printf( "\r\n\nModMqtt," __DATE__ "," __TIME__ "\r\n\n" );

  int pid = 1;
  int cnt = 0;
  char content[128];
  Flag<NetSocket::State> state;

  while(1)
  {
    net.update();

    if( net.isNewIP() )
    {
      terminal.printf( "MQTT-Client:%s\n\r\n", (const char*)(net.getIP()) );
    }

    switch( uart.get() )
    {
      case 0:   // no input
                break;

      case 'o': terminal.printf( "cmd: open TCP %s:%u\r\n", (char *)addr, PORT);
                mqtt.open( addr, PORT );
                break;

      case 'f': terminal.printf( "cmd: finish TCP\r\n");
                mqtt.close();
                break;

      case 'c': terminal.printf( "cmd: connect to Broker\r\n");
                mqtt.connect( "my name", true, timeout, NULL, NULL, NULL, NULL, 0 , false );
                break;

      case 'd': terminal.printf( "cmd: disconnect from Broker\r\n");
                mqtt.disconnect( );
                break;

      case 'p': terminal.printf( "cmd: publish\r\n");
                sprintf(content,"test %d", cnt++);
                mqtt.publish( pid++, "aaa/bbb", content, qos );
                break;

      case 'a': terminal.printf( "cmd: puback\r\n");
                mqtt.puback( pid++ );
                break;

      case 's': terminal.printf( "cmd: subscribe\r\n");
                mqtt.subscribe( pid++, "aaa/bbb", qos);
                break;

      case 'u': terminal.printf( "cmd: unsubscribe\r\n");
                mqtt.unsubscribe( pid++, "aaa/bbb" );
                break;

      case 'g': terminal.printf( "cmd: pingreq\r\n");
                mqtt.pingreq();
                break;

      default: // undefined command, print help
        uart.set( "\r\nCommand:\r\n");
        uart.set( "  o - open TCP\r\n");
        uart.set( "  f - finish TCP\r\n");
        uart.set( "  c - connect to broker\r\n");
        uart.set( "  d - disconnect\r\n");
        uart.set( "  p - publish\r\n");
        uart.set( "  a - puback\r\n");
        uart.set( "  s - subscribe\r\n");
        uart.set( "  u - unsubscribe\r\n");
        uart.set( "  g - pingreq\r\n");
        uart.set( "\r\n");
        break;
    }

    // print TCP connection state
    state = mqtt.getSocketState();
    switch( state.getUnique() )
    {
      case NetSocket::UNDEFINED:                                      break;
      case NetSocket::CLOSED:      uart.set("tcp: CLOSED\r\n"      ); break;
      case NetSocket::LISTENING:   uart.set("tcp: LISTENING\r\n"   ); break;
      case NetSocket::CONNECTED:   uart.set("tcp: CONNECTED\r\n"   ); break;
      case NetSocket::ERROR_STATE: uart.set("tcp: ERROR_STATE\r\n" ); break;
      default:                     uart.set("tcp: state unkown\r\n"); break;
    }
  }
}
