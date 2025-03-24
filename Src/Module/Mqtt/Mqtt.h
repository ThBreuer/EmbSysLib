//*******************************************************************
/*!
\file   Mqtt.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   12.06.2024

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _MOD_MQTT_MQTT_H
#define _MOD_MQTT_MQTT_H

//*******************************************************************
#include "Mcu_Types.h"
#include "Std/Flag.h"
#include "Std/DataPointer.h"
#include "Module/ReportID_Mod.h"

//*******************************************************************
namespace EmbSysLib {
namespace Mod {

using namespace Std;

//*******************************************************************
/*!
\class MqttInterf

\brief MQTT protocol interface

This class supports transmitting and receiving of MQTT packages via a TCP socket.
The application must be derived from MqttIntgerface::Handler.
MQTT version 3.1.1

\example ModMqtt.cpp, ModMqttClient.cpp
*/
class MqttInterf : protected Net::Task, public NetSocket::Handler
{
  public:
    //---------------------------------------------------------------
    /*! \enum ControlPacketType
        \brief
        \see "MQTT Version 3.1.1, OASIS Standard
    */
    typedef enum
    {
      // Name       Value         Direction  Description
      // ----       -----         ---------  -----------
      Reserved0   =  0,  //!< \n  none       Reserved
      CONNECT     =  1,  //!< \n  C --> S    Client request to connect to Server
      CONNACK     =  2,  //!< \n  C <-- S    Connect acknowledgment
      PUBLISH     =  3,  //!< \n  C <-> S    Publish message
      PUBACK      =  4,  //!< \n  C <-> S    Publish acknowledgment
      PUBREC      =  5,  //!< \n  C <-> S    Publish received (assured delivery part 1)
      PUBREL      =  6,  //!< \n  C <-> S    Publish release (assured delivery part 2)
      PUBCOMP     =  7,  //!< \n  C <-> S    Publish complete (assured delivery part 3)
      SUBSCRIBE   =  8,  //!< \n  C --> S    Client subscribe request
      SUBACK      =  9,  //!< \n  C <-- S    Subscribe acknowledgment
      UNSUBSCRIBE = 10,  //!< \n  C --> S    Unsubscribe request
      UNSUBACK    = 11,  //!< \n  C <-- S    Unsubscribe acknowledgment
      PINGREQ     = 12,  //!< \n  C --> S    PING request
      PINGRESP    = 13,  //!< \n  C <-- S    PING response
      DISCONNECT  = 14,  //!< \n  C --> S    Client is disconnecting
      Reserved1   = 15   //!< \n  none       Reserved
    } ControlPacketType;

  private:
    //---------------------------------------------------------------
    /*
       MQTT connect fags
       \see "MQTT Version 3.1.1, OASIS Standard
    */
    typedef enum
    {
      Reserved     = 0x01,
      CLEANSESSION = 0x02,
      WILLFLAG     = 0x04,
      WILLMASK     = 0x1C,
      WILLRETAIN   = 0x20,
      PASSWORD     = 0x40,
      USERNAME     = 0x80
    } ConnectFlag;

  public:
    //---------------------------------------------------------------
    /*!
    \class Handler

    \brief Call back interface used by NqttInterface.
    */
    class Handler
    {
      public:
        //-----------------------------------------------------------
        /*! This method is called by MqttInterface after receiving a
            PUBLISH message.
             \param pid Package ID
             \param control Control flags (bitmap): DUP,QoS, Retain
             \param topic Pointer to topic memory location
             \param payload Pointer to topic memory location
             \warning The application must not change memory located by topic and payload
        */
        virtual void onPublishReceived( WORD        pid,
                                        BYTE        control,
                                        DataPointer topic,
                                        DataPointer payload ) = 0;

        //-----------------------------------------------------------
        /*! This method is called by MqttInterface after receiving a
            control message (all messages except PUBLISH)
            \param type Message type
            \param pid Package ID
            \param rc Return code (CONNACK, SUBACK)
            \param flags Connect Acknowledge Flags (CONNACK)
        */
        virtual void onControlReceived( ControlPacketType type,
                                        WORD              pid,
                                        BYTE              rc,
                                        BYTE              flags ) = 0;

        //-----------------------------------------------------------
        /*! This method is called periodically(10ms) by MqttInterface
        */
        virtual void onTrigger( void ) = 0;
    };
    //---------------------------------------------------------------

  private:
    //---------------------------------------------------------------
    class Message
    {
      private:
        //---------------------------------------------------------------
        class Item : public Sequence<Item>::Item
        {
          public:
            //-----------------------------------------------------------
            virtual void send( NetSocket &socket ) = 0;

        }; //class MsgItem

      public:
        //-----------------------------------------------------------
        class Byte : public Item
        {
          public:
            //-------------------------------------------------------
            BYTE value;
            Byte( Message *msg, BYTE v );
            virtual void send( NetSocket &socket );
        }; //class Byte

        //---------------------------------------------------------------
        class Word : public Item
        {
          public:
            //-----------------------------------------------------------
            WORD value;
            Word( Message *msg, WORD v );
            virtual void send( NetSocket &socket );
        }; //class Word

        //---------------------------------------------------------------
        class String : public Item
        {
          public:
            //-----------------------------------------------------------
            WORD length;
            const char *str;
            String( Message *msg, const char *s );
            virtual void send( NetSocket &socket );
        }; //class String

        //---------------------------------------------------------------
        class Raw : public Item
        {
          public:
            //-----------------------------------------------------------
            WORD length;
            const BYTE *ptr;
            Raw( Message *msg, DataPointer dp );
            virtual void send( NetSocket &socket );
        }; //class Raw

      private:
        //-----------------------------------------------------------
        Message( ControlPacketType type, BYTE flags );

        //-----------------------------------------------------------
        void add( Item *item, DWORD len );

        //-----------------------------------------------------------
        void send( NetSocket &socket );

      private:
        //-----------------------------------------------------------
        BYTE           controlPacketType;
        DWORD          remainingLength;
        Sequence<Item> seq;

      //-------------------------------------------------------------
      friend class MqttInterf;

   }; //class Message

  private:
    //---------------------------------------------------------------
    MqttInterf::Handler   *handler;
    NetSocket             &sock;

  protected:
    //---------------------------------------------------------------
    MqttInterf( NetSocket &socket );

  public:
    //---------------------------------------------------------------
    /*! Add a callback handler to the MqttInterf object.
        \param handler Pointer to an Handler object or NULL
    */
    void add( Handler *handler);

    //---------------------------------------------------------------
    /*! Open TCP connection
        \param addr Broker's TCP address
        \param port Broker's port (normaly 1883)
    */
    virtual void open( NetAddr<4> addr, WORD port );

    //---------------------------------------------------------------
    /*! Close TCP connection
    */
    void close();

    //---------------------------------------------------------------
    /*! Get actual socket state
        \return Socket state
    */
    NetSocket::State getSocketState( void )
    {
      return( sock.getState() );
    }

    //---------------------------------------------------------------
    /*! Send a CONNECT message
        \param clientName   Pointer to client name
        \param cleanSession Clean session flag
        \param timeout      Keep alive timeout
        \param userName     Pointer to user name,
        \param passWord     Pointer to password,
        \param willTopic    Pointer to last will topic
        \param willPayload  Pointer to last will payload,
        \param willQoS      Last will quality of service,
        \param willRetain   Last will retain
    */
    virtual void connect( const char *clientName,
                          BYTE        cleanSession,
                          WORD        timeout,
                          const char *userName,
                          const char *passWord,
                          const char *willTopic,
                          const char *willPayload,
                          BYTE        willQoS,
                          BYTE        willRetain );

    //---------------------------------------------------------------
    /*! Send a CONNACK message
        \param sp Session Present Flag
        \param rc Return Code
    */
    virtual void connack( bool sp, BYTE rc );

    //---------------------------------------------------------------
    /*! Send a PUBLISH message
        \param pid Package ID
        \param topic Popinter to null terminated topic string
        \param payload Pointer to payload
        \param QoS Quality of Service
        \param dup Duplicate Flag
        \param retain Retain Flag
    */
    virtual void publish( WORD        pid,
                          const char *topic,
                          DataPointer payload,
                          BYTE        QoS    = 0,
                          bool        dup    = 0,
                          bool        retain = 0 );

    //---------------------------------------------------------------
    /*! Send a PUBACK message
        \param pid Package ID
    */
    virtual void puback( WORD pid );

    //---------------------------------------------------------------
    /*! Send a PUBREC message
        \param pid Package ID
    */
    virtual void pubrec( WORD pid );

    //---------------------------------------------------------------
    /*! Send a PUBREL message
        \param pid Package ID
    */
    virtual void pubrel( WORD pid );

    //---------------------------------------------------------------
    /*! Send a PUBCOMP message
        \param pid Package ID
    */
    virtual void pubcomp( WORD pid );

    //---------------------------------------------------------------
    /*! Send a SUBSCRIBE message
        \param pid Package ID
        \param topic Popinter to null terminated topic string
        \param QoS Quality of Service
    */
    virtual void subscribe( WORD        pid,
                            const char *topic,
                            BYTE        QoS );

    //---------------------------------------------------------------
    /*! Send a SUBACK message
        \param pid Package ID
        \param rc Return Code
    */
    virtual void suback( WORD pid,
                         BYTE rc );

    //---------------------------------------------------------------
    /*! Send a UNSUBSCRIBE message
        \param pid Package ID
        \param topic Popinter to null terminated topic string
    */
    virtual void unsubscribe( WORD        pid,
                              const char *topic );

    //---------------------------------------------------------------
     /*! Send a UNSUBACK message
        \param pid Package ID
   */
    virtual void unsuback( WORD pid );

    //---------------------------------------------------------------
    /*! Send a PINGREQ message
    */
    virtual void pingreq( void );

    //---------------------------------------------------------------
    /*! Send a PINGRESP message
    */
    virtual void pingresp( void );

    //---------------------------------------------------------------
    /*! Send a DISCONNECT message
    */
    virtual void disconnect( void );

  private:
    //---------------------------------------------------------------
    bool readByte( BYTE &data );

    //---------------------------------------------------------------
    bool readWord( WORD &data );

    //---------------------------------------------------------------
    DataPointer readString( void );

    //---------------------------------------------------------------
    DataPointer readRaw( WORD maxLen );

    //---------------------------------------------------------------
    /*!
    // Override NetSocket::Handler
    */
    virtual void onReceive( NetSocket &socketLocal );

    //---------------------------------------------------------------
    /*!
    // Override NetSocket::Handler
    */
    virtual void onEvent( NetSocket &socketLocal, NetSocket::Event event, WORD param );

    //---------------------------------------------------------------
    // Override Net::Task
    virtual bool onProcess( void );

    //---------------------------------------------------------------
    /*!
    // Override Net::Task
    */
    virtual void onTrigger( void );

};

//*******************************************************************
/*!
\class Mqtt

\brief Same as MqttInterf but with own NetSocket
*/
template <class TCP>
class Mqtt : public MqttInterf
{
  public:
    //---------------------------------------------------------------
    /*! Instantiate a MQTT Interface
        \param tcp Reference to a TCP object
    */
    Mqtt( TCP &tcp )
    : MqttInterf( socket )
    , socket    ( tcp, NULL )
    {
      socket.add( this );
      tcp.net.addTask( *this );
    }

  private:
    //---------------------------------------------------------------
    typename TCP::Socket  socket;

};  //class Mqtt

} } //namespace

#endif
