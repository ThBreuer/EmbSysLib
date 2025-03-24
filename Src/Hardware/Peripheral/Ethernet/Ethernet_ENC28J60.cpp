//*******************************************************************
/*!
\file   Ethernet_ENC28J60.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   01.07.2024

/// Origin: enc28j60.c, enc28j60.h from https://www.olimex.com/Products/Modules/Ethernet/ENC28J60-H/resources/Webserver_ATMega32_ENC28J60-H.zip

License: See file "LICENSE"
*/

//*******************************************************************
#include "Hardware/Common/Ethernet/Ethernet.h"
#include "Ethernet_ENC28J60.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// Ethernet_ENC28J60
//
//*******************************************************************
//-------------------------------------------------------------------
Ethernet_ENC28J60::Ethernet_ENC28J60( const NetAddr<6>  &addrPhyIn,
                                      SPImaster::Device &spiIn,
									  Timer             &timer )

: Ethernet( addrPhyIn, timer )
, spi     ( spiIn )
, addrPhy ( addrPhyIn )

{
  InitEth( addrPhyIn );
  System::delayMilliSec( 10 );
  Init();
}

//-------------------------------------------------------------------
WORD Ethernet_ENC28J60::getType( void )
{
  Ethernet::Header *eth =(Ethernet::Header *)buf;

  if( plen >= sizeof( NetEthIP::Msg ) && eth->type == TYPE_IP )
  {
    return( TYPE_IP );
  }
  else if( plen >= sizeof( NetEthARP::Msg ) && eth->type == TYPE_ARP )
  {
    return( TYPE_ARP );
  }
  else
  {
    return( TYPE_UNDEF );
  }
}

//-------------------------------------------------------------------
uint8_t Ethernet_ENC28J60::ReadOp( BYTE op, BYTE address )
{
  BYTE data[3] = { (BYTE)(op | (address & ADDR_MASK)),
                   0x00,
                   0x00 };

  if( address & 0x80 ) // do dummy read if needed (for mac and mii, see datasheet page 29)

  {
    spi.transceive( data, 3 );
    return( data[2] );
  }
  else
  {
    spi.transceive( data, 2 );
    return( data[1] );
  }
}

//-------------------------------------------------------------------
void Ethernet_ENC28J60::WriteOp( BYTE op, uint8_t address, BYTE dataIn )
{
  WORD x = ((WORD)( op | (address & ADDR_MASK) )<<8) | dataIn;

  spi.write( x );
}

//-------------------------------------------------------------------
void Ethernet_ENC28J60::ReadBuffer( WORD len, BYTE* data )
{
  spi.start();
  spi.transceiveByte( (BYTE)ENC28J60_READ_BUF_MEM );

  for( WORD i=0; i<len; i++ )
  {
     data[i] = spi.transceiveByte();
  }
  // data[len] = '\0';
  spi.stop();

}

//-------------------------------------------------------------------
void Ethernet_ENC28J60::WriteBuffer( WORD len, BYTE* data )
{
  spi.start();
  spi.transceiveByte( (BYTE)ENC28J60_WRITE_BUF_MEM );
  for( WORD i=0; i<len;i ++ )
  {
    spi.transceiveByte( data[i] );
  }
  spi.stop();
}

//-------------------------------------------------------------------
void Ethernet_ENC28J60::SetBank( BYTE address )
{
   // set the bank (if needed)
   if( (address & BANK_MASK) != bank )
   {
      // set the bank
      WriteOp( ENC28J60_BIT_FIELD_CLR, ECON1, (ECON1_BSEL1|ECON1_BSEL0) );
      WriteOp( ENC28J60_BIT_FIELD_SET, ECON1, (address & BANK_MASK)>>5  );
      bank = ( address & BANK_MASK );
   }
}

//-------------------------------------------------------------------
BYTE Ethernet_ENC28J60::Read( BYTE address )
{
   SetBank( address );
   return ReadOp( ENC28J60_READ_CTRL_REG, address );
}

//-------------------------------------------------------------------
void Ethernet_ENC28J60::WriteByte( BYTE address, BYTE data )
{
   SetBank( address );
   WriteOp( ENC28J60_WRITE_CTRL_REG, address, data );
}

//-------------------------------------------------------------------
void Ethernet_ENC28J60::WriteWord( BYTE address, WORD data )
{
   SetBank( address );
   WriteOp( ENC28J60_WRITE_CTRL_REG, address, data&0xFF );
   SetBank( address+1 );
   WriteOp( ENC28J60_WRITE_CTRL_REG, address+1, data>>8 );
}

//-------------------------------------------------------------------
void Ethernet_ENC28J60::PhyWrite( BYTE address, WORD data )
{
   WriteByte( MIREGADR, address );         // set the PHY register address
   WriteWord( MIWR, data );                // write the PHY data
   while( Read( MISTAT ) & MISTAT_BUSY )   // wait until the PHY write completes
   {
      // todo delay or no delay?
      asm("NOP"); //System::delayMicroSec( 50 );
   }
}

//-------------------------------------------------------------------
// Flash the 2 RJ45 LEDs twice to show that the interface works
bool Ethernet_ENC28J60::Init( void )
{
	// Magjack leds configuration, see enc28j60 datasheet, page 11
	// LEDA=green LEDB=yellow

	// 0x880 is PHLCON LEDB=on, LEDA=on
	// enc28j60PhyWrite(PHLCON,0b0000 1000 1000 00 00);
	PhyWrite( PHLCON, 0x880 );

	// 0x990 is PHLCON LEDB=off, LEDA=off
	// enc28j60PhyWrite(PHLCON,0b0000 1001 1001 00 00);
	PhyWrite( PHLCON, 0x990 );

	// 0x880 is PHLCON LEDB=on, LEDA=on
	// enc28j60PhyWrite(PHLCON,0b0000 1000 1000 00 00);
	PhyWrite( PHLCON, 0x880 );

	// 0x990 is PHLCON LEDB=off, LEDA=off
	// enc28j60PhyWrite(PHLCON,0b0000 1001 1001 00 00);
	PhyWrite( PHLCON, 0x990 );

	// 0x476 is PHLCON LEDA=links status, LEDB=receive/transmit
  // enc28j60PhyWrite(PHLCON,0b0000 0100 0111 01 10);
  PhyWrite( PHLCON, 0x476 );

  return( true );
}

//-------------------------------------------------------------------
void Ethernet_ENC28J60::InitEth( const NetAddr<6> &addrPhyIn )
{
  // perform system reset
  WriteOp( ENC28J60_SOFT_RESET, 0, ENC28J60_SOFT_RESET );
  System::delayMilliSec( 50 );

  // check CLKRDY bit to see if reset is complete
  // The CLKRDY does not work. See Rev. B4 Silicon Errata point. Just wait.
  //while(!(enc28j60Read(ESTAT) & ESTAT_CLKRDY));
  // do bank 0 stuff
  // initialize receive buffer
  // 16-bit transfers, must write low byte first
  // set receive buffer start address
  nextPacketPtr = RXSTART_INIT;

  // Rx start
  WriteWord( ERXST, RXSTART_INIT );

  // set receive pointer address
  WriteWord( ERXRDPT, RXSTART_INIT );

  // RX end
  WriteWord( ERXND, RXSTOP_INIT );

  // TX start
  WriteWord( ETXST, TXSTART_INIT );

  // TX end
  WriteWord( ETXND, TXSTOP_INIT );

  // do bank 1 stuff, packet filter:
  // For broadcast packets we allow only ARP packtets
  // All other packets should be unicast only for our mac (MAADR)
  //
  // The pattern to match on is therefore
  // Type     ETH.DST
  // ARP      BROADCAST
  // 06 08 -- ff ff ff ff ff ff -> ip checksum for theses bytes=f7f9
  // in binary these poitions are:11 0000 0011 1111
  // This is hex 303F->EPMM0=0x3f,EPMM1=0x30
  WriteByte( ERXFCON, 0   ); // ERXFCON_UCEN  | ERXFCON_CRCEN | ERXFCON_PMEN
  WriteByte( EPMM0,   0x3f);
  WriteByte( EPMM1,   0x30);
  WriteByte( EPMCSL,  0xf9);
  WriteByte( EPMCSH,  0xf7);

  // do bank 2 stuff
  // enable MAC receive
  WriteByte( MACON1, MACON1_MARXEN | MACON1_TXPAUS | MACON1_RXPAUS );

  // bring MAC out of reset
  WriteByte( MACON2, 0x00 );

  // enable automatic padding to 60bytes and CRC operations
  WriteOp( ENC28J60_BIT_FIELD_SET, MACON3, MACON3_PADCFG0 | MACON3_TXCRCEN | MACON3_FRMLNEN );

  // set inter-frame gap (non-back-to-back)
  WriteByte( MAIPGL, 0x12 );
  WriteByte( MAIPGH, 0x0C );

  // set inter-frame gap (back-to-back)
  WriteByte( MABBIPG, 0x12 );

  // Set the maximum packet size which the controller will accept
  // Do not send packets longer than MAX_FRAMELEN:
  WriteWord( MAMXFL, MAX_FRAMELEN );

  // do bank 3 stuff
  // write MAC address
  // NOTE: MAC address in ENC28J60 is byte-backward
  WriteByte( MAADR5, addrPhyIn.addr[0] );
  WriteByte( MAADR4, addrPhyIn.addr[1] );
  WriteByte( MAADR3, addrPhyIn.addr[2] );
  WriteByte( MAADR2, addrPhyIn.addr[3] );
  WriteByte( MAADR1, addrPhyIn.addr[4] );
  WriteByte( MAADR0, addrPhyIn.addr[5] );

  // no loopback of transmitted frames
  PhyWrite( PHCON2, PHCON2_HDLDIS );

  // switch to bank 0
  SetBank( ECON1 );

  // enable interrutps
  WriteOp( ENC28J60_BIT_FIELD_SET, EIE, EIE_INTIE | EIE_PKTIE );

  // enable packet reception
  WriteOp( ENC28J60_BIT_FIELD_SET, ECON1, ECON1_RXEN );
}

//-------------------------------------------------------------------
BYTE Ethernet_ENC28J60::getrev( void )     // read the revision of the chip
{
  return( Read( EREVID ) );
}

//-------------------------------------------------------------------
void Ethernet_ENC28J60::PacketSend( BYTE* packet, WORD len )
{
	// Set the write pointer to start of transmit buffer area
	WriteWord( EWRPT, TXSTART_INIT );

	// Set the TXND pointer to correspond to the packet size given
	WriteWord( ETXND, (TXSTART_INIT + len) );

	// write per-packet control byte (0x00 means use macon3 settings)
	WriteOp( ENC28J60_WRITE_BUF_MEM, 0, 0x00 );

	// copy the packet into the transmit buffer
	WriteBuffer( len, packet );

	// send the contents of the transmit buffer onto the network
	WriteOp( ENC28J60_BIT_FIELD_SET, ECON1, ECON1_TXRTS );

	// Reset the transmit logic problem. See Rev. B4 Silicon Errata point 12.
	if( (Read( EIR ) & EIR_TXERIF) )
	{
	  //! /todo Workaround on transmit error
	  InitEth( addrPhy );
	  System::delayMilliSec( 10 );
	  Init();
	  //    WriteOp(ENC28J60_BIT_FIELD_CLR, ECON1, ECON1_TXRTS);
	}
}

//-------------------------------------------------------------------
// Gets a packet from the network receive buffer, if one is available.
// The packet will be headed by an ethernet header.
//      maxlen  The maximum acceptable length of a retrieved packet.
//      packet  Pointer where packet data should be stored.
// Returns: Packet length in bytes if a packet was retrieved, zero otherwise.
bool Ethernet_ENC28J60::PacketReceive( void )
{
  uint16_t maxlen = BUFFER_SIZE;
	uint16_t rxstat;
	uint16_t len;

	// check if a packet has been received and buffered
	//if( !(enc28j60Read(EIR) & EIR_PKTIF) ){
  // The above does not work. See Rev. B4 Silicon Errata point 6.
  // todo check!
	if( /*(Read( EIR ) & EIR_PKTIF) &&*/ Read( EPKTCNT ) == 0 )
  {
	  return(0);
  }

	// Set the read pointer to the start of the received packet
	WriteWord( ERDPT, nextPacketPtr );

  // read the next packet pointer
	nextPacketPtr  = ReadOp( ENC28J60_READ_BUF_MEM, 0 );
	nextPacketPtr |= ReadOp( ENC28J60_READ_BUF_MEM, 0 ) << 8;

	// read the packet length (see datasheet page 43)
	len  = ReadOp( ENC28J60_READ_BUF_MEM, 0 );
	len |= ReadOp( ENC28J60_READ_BUF_MEM, 0 ) << 8;
  len -= 4; //remove the CRC count

   // read the receive status (see datasheet page 43)
	rxstat  = ReadOp( ENC28J60_READ_BUF_MEM, 0 );
	rxstat |= ReadOp( ENC28J60_READ_BUF_MEM, 0 ) << 8;

	// limit retrieve length
  if(len>maxlen-1)
  {
    len = maxlen-1;
  }

  // check CRC and symbol errors (see datasheet page 44, table 7-3):
  // The ERXFCON.CRCEN is set by default. Normally we should not
  // need to check this.
  if( (rxstat & 0x80) == 0 )
  {
    // invalid
    len = 0;
  }
  else
  {
    // copy the packet from the receive buffer
    ReadBuffer( len, buf );
  }

  // Move the RX read pointer to the start of the next received packet
	// This frees the memory we just read out
	WriteWord( ERXRDPT, nextPacketPtr );

  // decrement the packet counter indicate we are done with this packet
	WriteOp( ENC28J60_BIT_FIELD_SET, ECON2, ECON2_PKTDEC );

  plen = len;
	return( (plen==0) ? false : true );
}

//-------------------------------------------------------------------
void Ethernet_ENC28J60::create( const NetAddr<6> destAddr,
                                const WORD       type )
{
  Ethernet::Header *eth = (Ethernet::Header *)buf;

  eth->destinationAddr = destAddr;
  eth->sourceAddr      = addrPhy;
  eth->type            = type;
}

}  } //namespace

//EOF
