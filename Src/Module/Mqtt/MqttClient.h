//*******************************************************************
/*!
\file   MqttClient.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   12.06.2024

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _MOD_MQTT_MQTT_CLIENT_H
#define _MOD_MQTT_MQTT_CLIENT_H

//*******************************************************************
#include "Mcu_Types.h"
#include "Std/Flag.h"
#include "Std/DataPointer.h"
#include "Module/ReportID_Mod.h"
#include "Module/Mqtt/Mqtt.h"

//*******************************************************************
namespace EmbSysLib {
namespace Mod {

using namespace Std;

//*******************************************************************
/*!
\class MqttClient

\brief ...

\example ModMqttClient.cpp
*/
class MqttClient : public MqttInterf::Handler
{
  public:
    //***************************************************************
    class Request : public Sequence<Request>::Item
    {
      public:
        //-------------------------------------------------------------
        typedef enum
        {
          UNDEFINED    = 0,
          REQUESTED    = 1,
          WAITING      = 2,
          WAITING_2    = 3,
          ACKNOWLEDGED = 4
        } State;

      public:
        //-------------------------------------------------------------
        State reqFlag = UNDEFINED;
        WORD  reqPID  = 0;
        bool  isSub   = false;
        BYTE  timeout = 0;

      public:
        //-------------------------------------------------------------
        Request( Sequence<Request> &list, bool isSub )
        {
          this->isSub = isSub;
          list.add( this );
        }

        //-------------------------------------------------------------
        bool update(MqttClient &mqtt)
        {
          switch( reqFlag )
          {
            case MqttClient::Request::REQUESTED:
              send( mqtt );
              return( true );
              break;

            case MqttClient::Request::WAITING:
            case MqttClient::Request::WAITING_2:
              if( !(--timeout) )
              {
                reqFlag = MqttClient::Request::REQUESTED;
              }
              break;
            default:
              break;
          }
          return( false );
        }

        //-------------------------------------------------------------
        virtual void send(MqttClient &mqtt) = 0;
    };

    //***************************************************************
    class Subscribe : public Request
    {
      protected:
        //-------------------------------------------------------------
        char *topicStr;

      public:
        //-------------------------------------------------------------
        Subscribe( MqttClient &mqtt, char *topic )
        : Request( mqtt.subList, true )
        {
          topicStr = topic;
        }

        //-------------------------------------------------------------
        virtual void send( MqttClient &mqtt )
        {
          reqPID = mqtt.getUniquePID();
          mqtt.mqtt.subscribe(reqPID, topicStr,1);
          reqFlag = Request::WAITING;
          timeout = 200;
        }
    };

    //***************************************************************
    template <WORD TOPIC_SIZE>
    class SubscribeString : public Subscribe
    {
      public:
        //-------------------------------------------------------------
        char topicBuf[TOPIC_SIZE + 1];

      public:
        SubscribeString( MqttClient &mqtt, const char *topic )
        : Subscribe(mqtt,topicBuf)
        {
          strcpy(topicBuf,topic);
        }

        //-------------------------------------------------------------
        void topicPrintf( const char *format, ... )
        {
          va_list argzeiger;
          va_start ( argzeiger, format );
          vsnprintf( topicBuf, TOPIC_SIZE, format, argzeiger );
          va_end   ( argzeiger );

          reqFlag = Request::REQUESTED;
        }
    };

    //***************************************************************
    class Publish : public Request
    {
      public:
        //-------------------------------------------------------------
        const char *topic;
        DataPointer payload;

      public:
        //-------------------------------------------------------------
        Publish( MqttClient &mqtt, const char *topic, DataPointer payload )
        : Request( mqtt.subList, false )
        {
          this->topic    = topic;
          this->payload  = payload;
        }

        //-------------------------------------------------------------
        virtual void send( MqttClient &mqtt )
        {
          reqPID = mqtt.getUniquePID();
          mqtt.mqtt.publish( reqPID, topic,payload, 1,0,0);   //const char *topic, const char *payload, BYTE QoS, BYTE dup, BYTE retain
          reqFlag = Request::WAITING;
          timeout = 200;
        }
    };

    //***************************************************************
    template <WORD TOPIC_SIZE, WORD PAYLOAD_SIZE>
    class PublishString : public Publish
    {
      public:
        //-------------------------------------------------------------
        char topicBuf[TOPIC_SIZE + 1];
        char payloadBuf[PAYLOAD_SIZE+1];

      public:
        //-------------------------------------------------------------
        PublishString( MqttClient &mqtt, const char *topic )
        : Publish(mqtt,topicBuf, DataPointer( (BYTE*)payloadBuf, 0 ) )
        {
          strcpy(topicBuf,topic);
          memset(payloadBuf,0,PAYLOAD_SIZE+1);
        }

        //-------------------------------------------------------------
        void printf( const char *format, ... )
        {
          va_list argzeiger;
          va_start ( argzeiger, format );
          vsnprintf( payloadBuf, PAYLOAD_SIZE, format, argzeiger );
          va_end   ( argzeiger );
          payload = DataPointer( (BYTE*)payloadBuf, strlen( payloadBuf) );

          reqFlag = Request::REQUESTED;
        }

        //-------------------------------------------------------------
        void topicPrintf( const char *format, ... )
        {
          va_list argzeiger;
          va_start ( argzeiger, format );
          vsnprintf( topicBuf, TOPIC_SIZE, format, argzeiger );
          va_end   ( argzeiger );
        }
    };

  private:
    //-----------------------------------------------------------------
    typedef enum
    {
      NOT_CONNECTED  = 0,
      TCP_WAITING    = 2,
      CONACK_WAITING = 4,
      CONNECTED      = 5,
      TIMEOUT        = 6
    } ConnectState;

  private:
    //-----------------------------------------------------------------
    NetAddr<4> addr            = zeroAddr;
    WORD       port            = 0;
    const char      *conClientId     = "";
    bool       conCleanSession = false;
    WORD       conTimeout      = 0;

    const char *user = NULL;
    const char *passwd = NULL;
    const char *willTopic  = NULL;
    const char *willPayload = NULL;
    BYTE willQoS = 0;
    BYTE willRetain = 0;

    ConnectState conState   = NOT_CONNECTED;
    WORD         timeout    = 0;
    bool         connectReq = false;
    Flag<BYTE>   controlFlag;

    MqttInterf                 &mqtt;
    Sequence<MqttClient::Request>  subList;

    WORD pid=1;

    WORD pingTimeoutLimit = 0;
    WORD pingTimeoutCnt = 0;



  public:
    //-----------------------------------------------------------------
    /*!
    */
    MqttClient( MqttInterf &mqtt )
    : mqtt( mqtt )
    {
    }

  public:
    //---------------------------------------------------------------
    /*!
    */
    virtual void onReceived(DataPointer topic, DataPointer payload) = 0;

    //-----------------------------------------------------------------
    /*!
    */
    void open( NetAddr<4> &addr,
               WORD        port,
               const char       *clientId,
               bool        cleanSession,
               WORD        timeout,
               const char       *userName,
               const char       *passWord,
               const char       *willTopic,
               const char       *willMsg,
               BYTE        willQoS,
               BYTE        willRetain )
    {
      this->addr            = addr;
      this->port            = port;
      this->conClientId     = clientId;
      this->conCleanSession = cleanSession;
      this->conTimeout      = timeout;
      this->user            = userName;
      this->passwd          = passWord;
      this->willTopic       = willTopic;
      this->willPayload     = willMsg;
      this->willQoS         = willQoS;
      this->willRetain      = willRetain;

      connectReq = true;
    }

    //-----------------------------------------------------------------
    /*!
    */
    void open( void )
    {
      connectReq = true;
    }

    //-----------------------------------------------------------------
    /*!
    */
    void close( void )
    {
      connectReq = false;
      mqtt.close();
    }

    //---------------------------------------------------------------
    /*!
    */
    void subscribe( void )
    {
      MqttClient::Request *ptr = subList.getFirst();
      while( ptr )
      {
        if( ptr->isSub )
        {
          ptr->reqFlag = MqttClient::Subscribe::REQUESTED;
        }
        ptr          = ptr->getNext();
      }
    }

    //---------------------------------------------------------------
    /*! ...
    */
    WORD getUniquePID()
    {
      return( ++pid );
    }

  private:
    //-----------------------------------------------------------------
    virtual void onTrigger()
    {
      // run state machine connecting to the broker
      switch( conState )
      {
        case NOT_CONNECTED:
          if( connectReq )
          {
            mqtt.open( addr, port );
            conState = TCP_WAITING;
            timeout  = 0;
          }
          break;

        case TCP_WAITING:
          if( mqtt.getSocketState() == NetSocket::CONNECTED )
          {
            mqtt.connect(conClientId, conCleanSession, conTimeout, user, passwd,willTopic,willPayload,willQoS,willRetain);
            conState = CONACK_WAITING;
            timeout  = 0;
            pingTimeoutLimit = (DWORD)80*conTimeout; // 80% of timeout time (timeout in sec, 100 trigger/sec)
            pingTimeoutCnt = 0;
          }
          else if( timeout++ > 100 )
          {
            conState = NOT_CONNECTED;
          }
          break;

        case CONACK_WAITING:
          if( controlFlag.getEvent() == MqttInterf::CONNACK )
          {
            conState = CONNECTED;
            timeout  = 0;
            subscribe();
          }
          else if( timeout++ > 100 )
          {
            conState = NOT_CONNECTED;
            timeout  = 0;
          }
          break;

        case CONNECTED:
          if( mqtt.getSocketState() != NetSocket::CONNECTED )
          {
            conState = TIMEOUT;
            timeout  = 0;
          }
          break;

        case TIMEOUT:
          if( timeout++ > 100 )
          {
            conState = NOT_CONNECTED;
            timeout  = 0;
          }
          break;
      }

      // send requested subscription or publication to the broker
      if( conState == CONNECTED )
      {
        MqttClient::Request *ptr = subList.getFirst();
        while( ptr )
        {
          if( ptr->update( *this ) )
          {
            pingTimeoutCnt = 0;
          }
          ptr = ptr->getNext();
        }
      }

      if( mqtt.getSocketState() != NetSocket::CONNECTED )
      {
        pingTimeoutLimit = 0;
      }
      if( pingTimeoutLimit && pingTimeoutCnt++ > pingTimeoutLimit )
      // todo check if connected
      // todo check if we should send pinreq here (alternatively: MqttClient will send)
      {
        mqtt.pingreq();
        pingTimeoutCnt = 0;
      }

    }

  private:
    //---------------------------------------------------------------
    virtual void onPublishReceived( WORD pid, BYTE control, DataPointer topic, DataPointer payload )
    {
      //BYTE dup    = (control>>3)&0x01;
      //BYTE qos    = (control>>1)&0x03;
      //BYTE retain = (control   )&0x01;

      if( control&(0x03<<1) ) // QoS > 0 ?
      {
        mqtt.puback(pid);
      }
      onReceived( topic, payload);
    }

  private:
    //---------------------------------------------------------------
    virtual void onControlReceived( MqttInterf::ControlPacketType type,
                                    WORD                          pid,
                                    BYTE                          rc,
                                    BYTE                          flags )
    {
      switch( type )
      {
        case MqttInterf::CONNACK:
          controlFlag = MqttInterf::CONNACK;
          break;

        case MqttInterf::SUBACK:
        case MqttInterf::PUBACK:
        case MqttInterf::PUBREC:
        case MqttInterf::PUBCOMP:
          {
            MqttClient::Request *ptr = subList.getFirst();
            while( ptr )
            {
              if( ptr->reqPID  == pid )
              {
                if( ptr->reqFlag == MqttClient::Request::WAITING )
                {
                  if( type == MqttInterf::PUBREC) // check QoS also
                  {
                    ptr->reqFlag = MqttClient::Request::WAITING_2;
                    mqtt.pubrel( ptr->reqPID );
                  }
                  else
                    ptr->reqFlag = MqttClient::Request::ACKNOWLEDGED;
                }
                else if(ptr->reqFlag == MqttClient::Request::WAITING_2 )
                {
                  if( type == MqttInterf::PUBCOMP) // check QoS also
                  {
                    ptr->reqFlag = MqttClient::Request::ACKNOWLEDGED;
                    break;
                  }
                }
                break;
              }
              ptr = ptr->getNext();
             }
          }
          break;

        default:
          break;
      }
    }

};  //class MqttApp


} } //namespace

#endif
