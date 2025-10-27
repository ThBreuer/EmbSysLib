//*******************************************************************
/*!
\file   NetEthTCP.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   04.02.2024

License: See file "LICENSE"
*/

//*******************************************************************
#include "NetEthTCP.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// NetEthTCP
//
//*******************************************************************
//-------------------------------------------------------------------
NetEthTCP::NetEthTCP( Ethernet &net, NetEthIP &ipIn )
: NetEthTransport( net, ipIn )
{
  ip.setTCP( this );
}

//-------------------------------------------------------------------
void NetEthTCP::create( Socket &con, WORD payloadLen )
{
  NetEthTCP::Msg &msg = *(NetEthTCP::Msg*)ip.eth.buf;

  msg.tcp.sourcePort        = con.localPort;
  msg.tcp.destPort          = con.remotePort;
  msg.tcp.sequenceNumber    = con.seqNum;
  msg.tcp.acknowledgeNumber = con.ackNum;
  msg.tcp.setFlags( con.flags );
  msg.tcp.windowSize        = 1000;
  msg.tcp.urgentPointer     = 0;

  if (con.mss > 0 )
  {
    // the only option we set is MSS to 1408:
    // 1408 in hex is 0x580
    //< \todo An echte Groesse anpassen !!!
    ip.eth.buf[ sizeof( NetEthTCP::Msg)    ] = 2;
    ip.eth.buf[ sizeof( NetEthTCP::Msg) + 1] = 4;
    ip.eth.buf[ sizeof( NetEthTCP::Msg) + 2] = 0x05;
    ip.eth.buf[ sizeof( NetEthTCP::Msg) + 3] = 0x80;
    payloadLen += 4;

    msg.tcp.setDataOffset( 24 );
  }
  else
  {
    msg.tcp.setDataOffset( 20 );
  }

  ip.create( con.remoteAddrPhy,
             con.remoteAddr,
             payloadLen+sizeof(NetEthTCP::Header),
             NetEthIP::PROTOCOL_TCP );

  // Calculate checksum:
  WORD  len = sizeof( NetEthTCP::Header) + payloadLen;

  msg.tcp.checksum = 0;
  msg.tcp.checksum = NetEthIP::checksum( (BYTE*)&msg.ip.sourceAddr,
                                         len + 8,
                                         (DWORD)len + NetEthIP::PROTOCOL_TCP );

  ip.eth.PacketSend( ip.eth.buf, sizeof( NetEthTCP::Msg) + payloadLen );
}

//*******************************************************************
//
// NetEthTCP::Socket
//
//*******************************************************************
/*
todo

- check ackNumMsg and seqNumMsg


Sequence Number (4 Byte)
    Sequenznummer des ersten Daten-Oktetts (Byte) dieses TCP-Pakets oder
    die Initialisierungs-Sequenznummer falls das SYN-Flag gesetzt ist.
    Nach der Datenuebertragung dient sie zur Sortierung der TCP-Segmente,
    da diese in unterschiedlicher Reihenfolge beim Empfaenger ankommen koennen.
Acknowledgement Number (Quittierungsnummer) (4 Byte)
    Sie gibt die Sequenznummer an, die der Absender dieses TCP-Segmentes als
    naechstes erwartet. Sie ist nur gueltig, falls das ACK-Flag gesetzt ist.

(https://de.wikipedia.org/wiki/Transmission_Control_Protocol#)

So keep in mind that any packets generated, which are simply acknowledgments
(in other words, have only the ACK flag set and contain no data) to previously
received packets, never increment the sequence number.

initial: SN_lok zufaellig, AN_lok = 0?

empfang: test, wenn ACK gesetzt: SN_lok == AN_empf -> letzte Sendung OK
         test: AN_lok == SN_empf -> segmente vollstaendig

         empfang SYN:
              SN_lok = zufall
         erster empfang (z.B. nach SYN) oder wenn AN_lok==SN_empf
              AN_lok = SN_empf + dataLen_empf

senden: (SN_lok,AN_lok), danach(!)
                          SN_lok = SN_lok + dataLen_gesendet

unklar: wie gross ist dataLen, wenn nur Flags, nur Daten, Daten und Flags oder ACK gesendet wird.

Sequence number (32 bits)
    Has a dual role:

        If the SYN flag is set (1), then this is the initial sequence number. The sequence number of the actual first data byte and the acknowledged number in the corresponding ACK are then this sequence number plus 1.
        If the SYN flag is unset (0), then this is the accumulated sequence number of the first data byte of this segment for the current session.[10]



*/
//-------------------------------------------------------------------
NetEthTCP::Socket::Socket( NetEthTCP   &tcp,
                           Handler     *app )

: NetEthTransport::Socket( tcp.net, app )
, tcp( tcp )
{
  initialSeqNum = 10000;
  nextState( CLOSED );
  tcp.socketList.add( this );
}

//-------------------------------------------------------------------
void NetEthTCP::Socket::open( NetAddr<4> addr, WORD port )
{
  if( stateLocal == CLOSED )
  {
    initialSeqNum += 10000; // step the inititial seq num by something
                            // we will not use during this tcp session:

    if( addr == zeroAddr )
    {
      isServerFlag  = true;
      remotePort    = 0;
      localPort     = port;
      remoteAddrPhy = zeroAddrPhy;
      isRemoteValid = false;
    }
    else
    {
      isServerFlag = false;
      remotePort   = port;
      localPort    = tcp.ip.getAvailablePort();

      if( !(remoteAddr == addr ) )  // open with new(!) addr? => unknown PHY
      {
        remoteAddrPhy = zeroAddrPhy;
      }
      isRemoteValid = true;
    }
    remoteAddr = addr;

    seqNum = initialSeqNum;
    ackNum = 0;
    mss    = true;

    openReq = true;

    update();
  }
}

//-------------------------------------------------------------------
void NetEthTCP::Socket::close( void )
{
  closeReq = true;

  update();
}

//-------------------------------------------------------------------
void NetEthTCP::Socket::clear( void )
{
  tcp.ip.eth.plen = 0;
}

//-------------------------------------------------------------------
void NetEthTCP::Socket::fillBuffer( const char c )
{
  WORD pos = sizeof( NetEthTCP::Msg ) + tcp.ip.eth.plen;

  if( pos < BUFFER_SIZE )
  {
    tcp.ip.eth.buf[pos] = c;
    tcp.ip.eth.plen++;
  }

  if( pos + 1 >= BUFFER_SIZE )
  {
    flush();
  }
}

//-------------------------------------------------------------------
void NetEthTCP::Socket::flush( void )
{
  // todo use Push (PSH) flag, (parameter in send() ?) to indicate, that data should sent to app or to idicate last package/no more data. (for example in HTTP?)

  if( stateLocal == ESTABLISHED )
  {
    BYTE temp = flags;
    flags = FLAG_ACK;
    tcp.create( *this, tcp.ip.eth.plen );
    seqNum += tcp.ip.eth.plen;
    setTimeout( 3 );
    ackNumExpected = seqNum;
    flags = temp;
  }
  else
  {
    // ERROR_STATE
  }
  clear( );
}

//-------------------------------------------------------------------
void NetEthTCP::Socket::sendFlags( BYTE flagsIn)
{
  BYTE temp = flags;
  flags = flagsIn;
  clear();

  tcp.create( *this, tcp.ip.eth.plen );
  if( isFlag( FLAG_SYN | FLAG_FIN ) )
  {
    seqNum += 1;
  }
  flags = temp;
}

//-------------------------------------------------------------------
bool NetEthTCP::Socket::get( BYTE &x )
{
  if( tcp.inPos < tcp.inBufLen )
  {
    x = tcp.buf[tcp.inPos++];
    return( true );
  }
  return( false );
}

//-------------------------------------------------------------------
DataPointer NetEthTCP::Socket::get( WORD maxLen )
{
  DataPointer dp;
  if( tcp.inPos < tcp.inBufLen )
  {
    WORD minLen = MIN( (WORD)(tcp.inBufLen-tcp.inPos), maxLen );
	dp = DataPointer( (BYTE*)&tcp.buf[tcp.inPos], minLen);
	tcp.inPos += minLen;
  }
  return( dp );
}

//-------------------------------------------------------------------
DataPointer NetEthTCP::Socket::getDataPointer( void )
{
  const WORD offset = sizeof(NetEthTCP::Msg);
  DataPointer dp( (BYTE*)&tcp.ip.eth.buf[ offset ],  BUFFER_SIZE-offset );
  return( dp );
}

//-------------------------------------------------------------------
bool NetEthTCP::Socket::onProcess( void )
{
  NetEthTCP::Msg &msg = *(NetEthTCP::Msg*)tcp.ip.eth.buf;

  WORD destPort = msg.tcp.destPort;

  if( !(localPort == destPort) )
  {
    return( false );
  }
  if( isRemoteValid )
  {
    if( !(remotePort == msg.tcp.sourcePort) || !(remoteAddr == msg.ip.sourceAddr) )
    {
      return( false );
    }
  }

  flags   = msg.tcp.getFlags();

  isDataFlag = true;

  DWORD ackNumMsg  = msg.tcp.acknowledgeNumber;
  DWORD seqNumMsg  = msg.tcp.sequenceNumber;

   dataLen =   msg.ip.packetLength
                 - sizeof(NetEthIP::Header)
                 - msg.tcp.getDataOffset();

  dataPtr = NULL;

  if( dataLen > 0 )
  {
    WORD idx = sizeof( NetEthIP::Msg ) + msg.tcp.getDataOffset();
    if( idx > 0 )
    {
      dataPtr = &tcp.ip.eth.buf[ idx ];
    }
  }

  // todo  clear(); /// ??? plen is used later !
  tcp.ip.eth.plen = 0;

  if( isFlag( FLAG_RST ) )
  {
    //stateLocal = STATE_WAITING;
    mss   = false;
    //return( true );
  }

  if( isFlag( FLAG_SYN | FLAG_FIN ) )
    ackNum = seqNumMsg+1;
  else
    ackNum = seqNumMsg;

  update();

  return( true ); // todo if remoteAddr == broadcastAddr {return( false )} ???
                   // todo return true, if data are processed successfully
}

//-------------------------------------------------------------------
void NetEthTCP::Socket::update( void )
{
  if( isFlag( FLAG_RST ) )
  {
    nextState( ERROR_STATE );
  }

  switch( stateLocal )
  {
    //---------------
    case CLOSED:
    //---------------
      isRemoteValid = false;

      if( openReq )
      {
        if( isServerFlag )
        {
          nextState( LISTEN );
        }
        else
        {
          if( !(remoteAddrPhy == zeroAddrPhy) ) // PHY known
          {
            sendFlags( FLAG_SYN );
            nextState( SYN_SENT );
          }
          else                        // PHY unknown
          {
            tcp.ip.arp.request( remoteAddr );
            nextState( WAIT_ARP );
          }
        }
      }
      //state = State::CLOSED;
      break;

    //---------------
    case WAIT_ARP:
    //---------------
      if( timeoutReq )
      {
        nextState( ERROR_STATE );
      }
      else if( remoteAddrPhy != 0 ) // Requested PHY is received
      {
        sendFlags( FLAG_SYN );
        nextState( SYN_SENT );
      }
      break;

    //---------------
    case LISTEN:
    //---------------
      // maybe leaved by open()/SYN -> SYN_SENT (not supported)
      isRemoteValid = false;

      if( closeReq )
      {
        nextState( CLOSED );
      }
      else if( isFlag( FLAG_SYN ) )
      {
        if( busy )
        {
          sendFlags( FLAG_RST );
        }
        else
        {
          NetEthTCP::Msg &msg = *(NetEthTCP::Msg*)tcp.ip.eth.buf;
          remoteAddr    = msg.ip.sourceAddr;
          remotePort    = msg.tcp.sourcePort;
          remoteAddrPhy = msg.eth.sourceAddr;
          isRemoteValid = true;

          mss     = true;

          initialSeqNum += 10000;
          seqNum = initialSeqNum;

          sendFlags( FLAG_SYN|FLAG_ACK ); // (or SYN only?)
          nextState( SYN_RECVD );
        }
      }
      //state = State::LISTENING;
      break;

    //---------------
    case SYN_SENT:
    //---------------
      if( timeoutReq )
      {
        sendFlags( FLAG_RST );
        nextState( ERROR_STATE );
      }
      else if( closeReq )
      {
        sendFlags( FLAG_RST );
        nextState( (isServerFlag)?LISTEN:CLOSED );
      }
      else if( isFlag( FLAG_SYN ) )
      {
        if( isFlag( FLAG_ACK ) ) // SYN+ACK
        {
          sendFlags( FLAG_ACK );
          nextState( ESTABLISHED );
          mss   = false;
        }
        else                     // SYN
        {
          sendFlags( FLAG_SYN|FLAG_ACK );
          nextState(SYN_RECVD);
        }
      }
      break;

    //---------------
    case SYN_RECVD:
    //---------------
      if( timeoutReq )
      {
        sendFlags( FLAG_RST );
        nextState( ERROR_STATE );
      }
      else if( closeReq )
      {
        sendFlags( FLAG_FIN );
        nextState( FIN_WAIT_1 );
      }
      else if( busy )
      {
        sendFlags( FLAG_RST );
        nextState( (isServerFlag)?LISTEN:CLOSED );
      }
      else if( isFlag( FLAG_ACK ) )
      {
        // nothing to send
        nextState( ESTABLISHED );

        mss   = false;
      }
      break;

    //---------------
    case ESTABLISHED:
    //---------------
      /// ... communication ....

      if( /*isDataFlag || */isFlag( FLAG_ACK|FLAG_SYN ) )
      {
        setTimeout(0);
      }

      if( closeReq )
      {
        sendFlags( FLAG_FIN|FLAG_ACK );
        nextState( FIN_WAIT_1 );
      }
      else if( timeoutReq )
      {
        sendFlags( FLAG_FIN );
        nextState( FIN_WAIT_1 );
      }
      else if( isFlag( FLAG_FIN ) )
      {
        // todo
        // moeglicher weise wurde letzte paket nicht mehr empfangen
        // NetEthTCP::Msg &msg = *(NetEthTCP::Msg*)tcp.ip.eth.buf;
        // seqNum = msg.tcp.acknowledgeNumber;

        sendFlags( FLAG_ACK );
        nextState( CLOSE_WAIT );
      }
      else if( isDataFlag && dataLen > 0 )
      {
        ackNum += dataLen;
        mss = false;

        tcp.buf = dataPtr;
        tcp.inPos = 0;
        tcp.inBufLen = dataLen;

        if( app )
        {
          app->onReceive( *this );
        }

        /// \todo wenn app kein ACK sendet, muss dies hier nachgeholt werden!
       sendFlags( FLAG_ACK );


        isDataFlag=false;
      }
      //state = State::CONNECTED;
      break;

    //---------------
    case CLOSE_WAIT:
    //---------------
      // todo use ? if( timeoutReq )
      {
        sendFlags( FLAG_FIN|FLAG_ACK );
        nextState( LAST_ACK );
      }
      break;

    //---------------
    case LAST_ACK:
    //---------------
      if( timeoutReq )
      {
        // nothing to send
        nextState( ERROR_STATE );
      }
      else if( isFlag( FLAG_ACK ) )
      {
        // nothing to send
        nextState( (isServerFlag)?LISTEN:CLOSED );
      }
      break;

    //---------------
    case FIN_WAIT_1:
    //---------------
      if( timeoutReq )
      {
        sendFlags( FLAG_RST );
        nextState( ERROR_STATE );
      }
      else if( isFlag( FLAG_ACK ) )
      {
        if( isFlag( FLAG_FIN ) )       // FIN+ACK
        {
          sendFlags( FLAG_ACK );
          nextState( TIMED_WAIT );
        }
        else            // ACK
        {
          // nothing to send
          nextState( FIN_WAIT_2 );
        }
      }
      else if( isFlag( FLAG_FIN ) )    // FIN
      {
        sendFlags( FLAG_ACK );
        nextState( CLOSING );
      }
      break;

    //---------------
    case FIN_WAIT_2:
    //---------------
      if( timeoutReq )
      {
        sendFlags( FLAG_RST );
        nextState( ERROR_STATE );
      }
      else if( isFlag( FLAG_FIN ) )
      {
        sendFlags( FLAG_ACK );
        nextState( TIMED_WAIT );
      }
      break;

    //---------------
    case CLOSING:
    //---------------
      if( timeoutReq )
      {
        sendFlags( FLAG_RST );
        nextState( ERROR_STATE );
      }
      else if( isFlag( FLAG_ACK ) )
      {
        // nothing to send
        nextState( TIMED_WAIT );
      }
      break;

    //---------------
    case TIMED_WAIT:
    //---------------
      // todo no timeout? Or leave with ACK received?    if( timeoutReq )
      {
        // nothing to send
        nextState( (isServerFlag)?LISTEN:CLOSED ); //ERROR_STATE );
      }
      break;

    //---------------
    default:
    case ERROR_STATE:
    //---------------
      // todo timeout or close before leaving this state?
      nextState( (isServerFlag)?LISTEN:CLOSED );
      break;
  }

  timeoutReq = false;
  closeReq = false;
  openReq = false;


  flags = 0;

///  return( true );

}

//-------------------------------------------------------------------
// todo timout ?
void NetEthTCP::Socket::onTrigger( void )
{
  linkState = tcp.ip.eth.isLinked()?Ethernet::CONNECTED:Ethernet::DISCONNECTED;
  if( app && linkState.getUnique() )
  {
    app->onEvent( *this, LINK_STATE, linkState.get() );
  }

  if( timeOutTic && !(--timeOutTic) )
  {
    timeoutReq = true;
  }
  update();
}

//-------------------------------------------------------------------
void NetEthTCP::handleARP( NetAddr<4> addr, NetAddr<6> addrPhy )
{
  Socket *ptr = (Socket *)socketList.getFirst();
  while( ptr )
  {
    if(    ptr->remoteAddr == addr
        && (   ptr->stateLocal == Socket::WAIT_ARP
           // || ptr->stateLocal == Socket::STATE_OPEN_WAIT_ARP
            ) )
    {
      ptr->remoteAddrPhy = addrPhy;
      //      ptr->stateLocal = Socket::STATE_OPEN_WAIT_ARP; // ???
    }
    ptr = (Socket *)ptr->getNext();
  }
}

//-------------------------------------------------------------------
void NetEthTCP::Socket::nextState( BYTE newState )
{
  statePrev = stateLocal;
  stateLocal = newState;

  switch( stateLocal )
  {
    case CLOSE_WAIT: setTimeout( 1 ); break;

    case SYN_SENT:
    case SYN_RECVD:
    case LAST_ACK:
    case FIN_WAIT_1:
    case FIN_WAIT_2:
    case CLOSING:    setTimeout( 3 ); break;

    case WAIT_ARP:   setTimeout( 6 ); break;

    case TIMED_WAIT:
    default:         setTimeout( 0 ); break;
  }

  switch( stateLocal )
  {
    case CLOSED:      state = NetSocket::CLOSED;      break;
    case LISTEN:      state = NetSocket::LISTENING;   break;
    case ESTABLISHED: state = NetSocket::CONNECTED;   break;
    case ERROR_STATE: state = NetSocket::ERROR_STATE; break;
  }

  if( app && state.getUnique() )
  {
    app->onEvent(*this, SOCKET_STATE, state.get() );
  }
}

}  } //namespace

//EOF
