//*******************************************************************
/*!
\file   ModMqttClient.cpp
\author Thomas Breuer
\date   06.02.2024

\brief
*/

//*******************************************************************
/*
Usage:    ...
*/

//*******************************************************************
#include <stdio.h>
#include <ctype.h>
#include "EmbSysLib.h"

using namespace EmbSysLib::Std;
using namespace EmbSysLib::Ctrl;
using namespace EmbSysLib::Mod;

//-------------------------------------------------------------------
#include "ReportHandler.h"
#include "configHwNet.h"

#include "Module/MQTT/Mqtt.h"
#include "Module/MQTT/MqttClient.h"

//-------------------------------------------------------------------
Terminal     terminal (uart, 255,255);

//*******************************************************************
//*******************************************************************
class Application : public MqttClient
{
  public:
    //---------------------------------------------------------------
    MqttClient::SubscribeString<32>  a;
    MqttClient::SubscribeString<32>  b;

    MqttClient::PublishString<32,32> s;

  public:
    //---------------------------------------------------------------
    Application(MqttInterf &mqtt)
    :MqttClient(mqtt)
    , a( *this,"aa/#" )
    , b( *this ,"+/+")
    , s( *this,"xaa/bb")
    {
     // new MqttClient::SubscribeString<32> (*this,"kcjelqc");
     // new SubscribeString<32> (*this,"$SYS/#");

      mqtt.add( this );
    }

  private:
    //---------------------------------------------------------------
    virtual void onReceived( DataPointer topic, DataPointer payload )
    {

      terminal.printf("onReceived '%.*s' [%u] '%.*s' [%u]\r\n",
                        topic.getSize(),topic.getPtr(),
                        topic.getSize(),
                        payload.getSize(),payload.getPtr(),
                        payload.getSize()
                      );
    }

  public:


};

//*******************************************************************
Mqtt<TCP_type>   mqtt( tcp );
Application      app( mqtt );

//*******************************************************************
int main(void)
{
  uart.set( "\r\n\nModMqttClient," __DATE__ "," __TIME__ "\r\n\n" );

  int cnt=0;

  app.open(addr, 1883,
           "TEST", true,
           10,
           NULL,NULL,
           "aa/will","bye",2,0);

  int t = 0;

  while(1)
  {
    net.update();

    if( net.isNewIP() )
    {
      terminal.printf( "MQTT-Client:%s\r\n", (const char*)(net.getIP()) );
    }

    switch( uart.get() )
    {
      case 'o': terminal.printf( "\r\nconnecting to %s:%u\r\n", (char *)addr, PORT);
                app.open();
                break;

      case 'c': terminal.printf( "\r\nclosing\r\n");
                app.close();
                break;

      case 'p': terminal.printf( "\r\npublishing\r\n");
                app.s.printf("result:%d",cnt++);
                break;

      case 't': terminal.printf( "\r\nset topic\r\n");
                app.s.topicPrintf("aa/x%d",t);
                app.a.topicPrintf("aa/x%d",t);
                t++;
                break;

    }
  }
}
