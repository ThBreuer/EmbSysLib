//*******************************************************************
/*!
\file   Mqtt.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   12.06.2024

License: See file "LICENSE"
*/

//*******************************************************************
#include "EmbSysLib.h"
#include "Module/Mqtt/Mqtt.h"

//*******************************************************************
namespace EmbSysLib {
namespace Mod {

using namespace Std;

//*******************************************************************
//
// MqttInterf
//
//*******************************************************************
//-------------------------------------------------------------------
MqttInterf::MqttInterf( NetSocket & socket )
: sock(socket)
{
// C'tor must not use socket because socket will been initialized later
}

//-------------------------------------------------------------------
void MqttInterf::add( Handler *handler )
{
  this->handler = handler;
}

//-------------------------------------------------------------------
void MqttInterf::open( NetAddr<4> addr, WORD port )
{
  sock.open( addr, port );
}

//-------------------------------------------------------------------
void MqttInterf::close( )
{
  sock.close();
}

//-------------------------------------------------------------------
// Methods to create and send messages
//-------------------------------------------------------------------
void MqttInterf::connect( const char *clientName,
                          BYTE        cleanSession,
                          WORD        timeout,
                          const char *userName,
                          const char *passWord,
                          const char *willTopic,
                          const char *willPayload,
                          BYTE        willQoS,
                          BYTE        willRetain )
{
  Message m( CONNECT, 0 );

  Message::String  a( &m, "MQTT");
  Message::Byte    b( &m, 4);
  Message::Byte    c( &m, (cleanSession?CLEANSESSION:0)
                         |(userName    ?USERNAME    :0)
                         |(passWord    ?PASSWORD    :0)
                         |(willTopic?(WILLFLAG|(willQoS<<3)|(willRetain?WILLRETAIN:0)):0));

  Message::Word    d( &m, timeout);
  Message::String  e( &m, clientName);  // payload: client identifier
  Message::String  f( &m, willTopic);   // if Will Flag == 1 -> Will topic payload
  Message::String  g( &m, willPayload); // if Will Flag == 1 -> Will message
  Message::String  h( userName?&m:NULL, userName );
  Message::String  i( passWord?&m:NULL, passWord );

  m.send( sock );
}

//-------------------------------------------------------------------
void MqttInterf::connack( bool sp, BYTE rc )
{
  Message m( CONNACK, 0 );

  Message::Byte a(&m, sp & 0x01);
  Message::Byte b(&m, rc );

  m.send( sock );
}

//-------------------------------------------------------------------
void MqttInterf::publish( WORD        pid,
                          const char *topic,
                          DataPointer payload,
                          BYTE        QoS,
                          bool        dup,
                          bool        retain )
{
  Message m( PUBLISH, (dup?0x08:0) | ((QoS&0x3)<<1) | (retain?0x01:0) );

  Message::String a( &m, topic );
  Message::Word   b( (QoS)?&m:NULL, pid );
  Message::Raw    c( &m, payload );

  m.send( sock );
}

//-------------------------------------------------------------------
void MqttInterf::puback( WORD pid )
{
  Message m( PUBACK, 0 );

  Message::Word a( &m, pid );

  m.send( sock );
}
//-------------------------------------------------------------------
void MqttInterf::pubrec( WORD pid )
{
  Message m( PUBREC, 0 );

  Message::Word a( &m, pid );

  m.send( sock );
}

//-------------------------------------------------------------------
void MqttInterf::pubrel( WORD pid )
{
  Message m( PUBREL, 0x02 );

  Message::Word a( &m, pid );

  m.send( sock );
}

//-------------------------------------------------------------------
void MqttInterf::pubcomp( WORD pid )
{
  Message m( PUBCOMP, 0 );

  Message::Word a( &m, pid );

  m.send( sock );
}

//-------------------------------------------------------------------
void MqttInterf::subscribe( WORD pid, const char *topic, BYTE QoS )
{
  Message m( SUBSCRIBE, 1<<1 );

  Message::Word    a( &m, pid );
  Message::String  b( &m, topic );
  Message::Byte    c( &m, QoS );

  m.send( sock );
}

//-------------------------------------------------------------------
void MqttInterf::suback( WORD pid, BYTE rc )
{
  Message m( SUBACK, 0 );

  Message::Word a( &m, pid );
  Message::Byte b( &m, rc );

  m.send( sock );
}

//-------------------------------------------------------------------
void MqttInterf::unsubscribe(WORD pid, const char *topic )
{
  Message m( UNSUBSCRIBE, 1<<1 );

  Message::Word    a( &m, pid );
  Message::String  b( &m, topic );

  m.send( sock );
}

//-------------------------------------------------------------------
void MqttInterf::unsuback( WORD pid )
{
  Message m( UNSUBACK, 0 );

  Message::Word  a( &m, pid );

  m.send( sock );
}

//-------------------------------------------------------------------
void MqttInterf::pingreq( void )
{
  Message m( PINGREQ, 0 );

  m.send( sock );
}

//-------------------------------------------------------------------
void MqttInterf::pingresp( void )
{
  Message m( PINGRESP, 0 );

  m.send( sock );
}

//-------------------------------------------------------------------
void MqttInterf::disconnect( void )
{
  Message m( DISCONNECT, 0 );

  m.send( sock );
}

//-------------------------------------------------------------------
// Private methods
//-------------------------------------------------------------------
bool MqttInterf::readByte(  BYTE &data )
{
  BYTE c;
  if( sock.get( c ) )
  {
    data = c;
    return( true );
  }
  return( false );
}

//-------------------------------------------------------------------
bool MqttInterf::readWord(  WORD &data )
{
  BYTE c[2];
  if( sock.get( c[0] ) && sock.get( c[1] ))
  {
    data = ((WORD)c[0]<<8) | (c[1]);
    return( true );
  }
  return( false );
}

//-------------------------------------------------------------------
DataPointer MqttInterf::readString( void )
{
  WORD len;

  if( readWord( len ) )
  {
    return( readRaw( len ) );
  }

  return( DataPointer() );
}

//-------------------------------------------------------------------
DataPointer MqttInterf::readRaw(  WORD maxLen )
{
  return( sock.get( maxLen ) );
}

//-------------------------------------------------------------------
void MqttInterf::onReceive( NetSocket &socketLocal )
{
  BYTE   control;
  DWORD  remainingLength = 0;

  // read fixed header
  if( readByte(control) )
  {
    //----------------------------------
    // decode remainingLength
    // code from mqtt-v3.1.1-os.pdf, modified
    // todo implement as method 'readFlexWord()'
    BYTE shift = 0;
    BYTE encodedByte;
    do
    {
      if( !readByte(encodedByte) ) //next byte from stream
      {
        return;
      }
      remainingLength += (DWORD)(encodedByte & 0x7F) << shift; // add lower part * multiplier
      shift +=7;     // multiplier *= 128
      if( shift>21 ) // multiplier > 128*128*128 ?
      {
        return; // throw Error(Malformed Remaining Length)
      }
    }
    while( encodedByte & 0x80 );
    //----------------------------------
  }
  else
  {
    return;
  }

  WORD pid     = 0;
  BYTE flags   = 0;
  BYTE ret = 0;
  BYTE qos     = (control>>1)&0x03;

  ControlPacketType controlType = (ControlPacketType)(control >> 4);
  switch( controlType )
  {
    case PUBLISH:
      {
        DataPointer topic = readString();

        if( qos )
          readWord(  pid );

        DWORD len = remainingLength - (qos?sizeof(WORD):0) - topic.getSize() - sizeof(WORD);

        DataPointer payload = readRaw( len );

      if( handler )
        handler->onPublishReceived( pid, control&0x0F, topic, payload );
      }
      break;

    case CONNACK:
      readByte(  flags   );
      readByte(  ret );
      break;

    case PUBACK:
    case PUBREC:
    case PUBREL:
    case PUBCOMP:
    case UNSUBACK:
      readWord(  pid );
      break;

    case SUBACK:
      readWord(  pid );
      readByte(  ret );
      break;

    case PINGRESP:
      break;
    
    default:
      break;
  }

  sock.clear();
  if( controlType != PUBLISH && handler )
  {
    handler->onControlReceived( controlType, pid, ret, flags);
  }
}


//-------------------------------------------------------------------
void MqttInterf::onEvent( NetSocket &socketLocal, NetSocket::Event event, WORD param )
{
// todo
//  if( event == NetSocket::SOCKET_STATE )
//  {
//	  socketState = param;
//  }
}

//-------------------------------------------------------------------
bool MqttInterf::onProcess( void )
{
  return( false );
}

//-------------------------------------------------------------------
void MqttInterf::onTrigger( void )
{
  if( handler )
    handler->onTrigger();
}

//*******************************************************************
//
// MqttInterf::Message
//
//*******************************************************************
//-------------------------------------------------------------------
MqttInterf::Message::Message( ControlPacketType type, BYTE flags )
{
  controlPacketType = (type << 4) | (flags & 0x0F);
  remainingLength   = 0;
}

//-------------------------------------------------------------------
void MqttInterf::Message::send( NetSocket &socket )
{
  socket.clear();

  // send fixed header
  socket.write( (BYTE)controlPacketType );

  //----------------------------------
  // encode remainingLength
  // code from mqtt-v3.1.1-os.pdf, modified
  // todo implement as 'writeFlexWord()'
  DWORD x = remainingLength;
  do
  {
    BYTE  encodedByte = x & 0x7F;
    x = x>>7;
    // if there are more data to encode, set the top bit of this byte
    if ( x > 0 )
    {
      encodedByte |= 0x80;
    }
    socket.write( (BYTE)encodedByte );
  }
  while ( x > 0 );
  //----------------------------------

  // send variable header and payload
  Item *ptr = seq.getFirst();
  while( ptr )
  {
    ptr->send( socket );
    ptr = ptr->getNext();
  }
  socket.flush();
}

//-------------------------------------------------------------------
void MqttInterf::Message::add( Item *item, DWORD len )
{
  seq.add( item );
  remainingLength += len;
}

//*******************************************************************
//
// MqttInterf::Message::Byte
//
//*******************************************************************
//-------------------------------------------------------------------
MqttInterf::Message::Byte::Byte( Message *msg, BYTE v )
{
  value = v;
  if( msg )
    msg->add( this, sizeof( BYTE ) );
}

//-----------------------------------------------------------
void MqttInterf::Message::Byte::send( NetSocket &socket )
{
  socket.write( (BYTE)value );
}

//*******************************************************************
//
// MqttInterf::Message::Word
//
//*******************************************************************
//-------------------------------------------------------------------
MqttInterf::Message::Word::Word( Message *msg, WORD v )
{
  value = v;
  if( msg )
    msg->add( this, sizeof( WORD ) );
}

//-----------------------------------------------------------
void MqttInterf::Message::Word::send( NetSocket &socket )
{
  socket.write( (BYTE)(value >> 8   ) );
  socket.write( (BYTE)(value &  0xFF) );
}
//*******************************************************************
//
// MqttInterf::Message::String
//
//*******************************************************************
//-------------------------------------------------------------------
MqttInterf::Message::String::String( Message *msg, const char *s )
{
  length = s?strlen( s ):0;
  str    = s;
  if( msg && str )
    msg->add( this, strlen( str ) + 2 );
}

//-----------------------------------------------------------
void MqttInterf::Message::String::send( NetSocket &socket )
{
  socket.write( (BYTE)(length >> 8   ) );
  socket.write( (BYTE)(length &  0xFF) );
  if( str )
    socket.write( str, length );
}

//*******************************************************************
//
// MqttInterf::Message::Raw
//
//*******************************************************************
//-------------------------------------------------------------------
//-----------------------------------------------------------
MqttInterf::Message::Raw::Raw( Message *msg, DataPointer dp )
{
  ptr    = dp.getPtr();
  length = dp.getSize();
  if( msg )
    msg->add( this, length );
}

//-----------------------------------------------------------
void MqttInterf::Message::Raw::send( NetSocket &socket )
{
  socket.write( (char*)ptr, length );
}


}  } //namespace

//EOF
