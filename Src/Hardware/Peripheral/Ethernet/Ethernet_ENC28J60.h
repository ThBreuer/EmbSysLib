//*******************************************************************
/*!
\file   Ethernet_ENC28J60.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   01.07.2024

/// Origin: enc28j60.c, enc28j60.h from https://www.olimex.com/Products/Modules/Ethernet/ENC28J60-H/resources/Webserver_ATMega32_ENC28J60-H.zip

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_ETHERNET_ENC28J60_H
#define _HW_ETHERNET_ENC28J60_H

//*******************************************************************
#include "Hardware/Common/Ethernet/Ethernet.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class Ethernet_ENC28J60

\brief ...

\example  Ethernet_ENC28J60.cpp
*/
class Ethernet_ENC28J60 : public Ethernet
{
  private:
    //---------------------------------------------------------------
    typedef enum
    {
      // ENC28J60 Control Registers
      // Control register definitions are a combination of address,
      // bank number, and Ethernet/MAC/PHY indicator bits.
      // - Register address   (bits 0-4)
      // - Bank number        (bits 5-6)
      // - MAC/PHY indicator  (bit  7)
      ADDR_MASK   =     0x1F,
      BANK_MASK   =     0x60,
      SPRD_MASK   =     0x80,

      // All-bank registers
      EIE         =     0x1B,
      EIR         =     0x1C,
      ESTAT       =     0x1D,
      ECON2       =     0x1E,
      ECON1       =     0x1F,

      // Bank 0 registers
      ERDPT          =  (0x00|0x00),
      ERDPTL         =  (0x00|0x00),
      ERDPTH         =  (0x01|0x00),
      EWRPT          =  (0x02|0x00),
      EWRPTL         =  (0x02|0x00),
      EWRPTH         =  (0x03|0x00),
      ETXST          =  (0x04|0x00),
      ETXSTL         =  (0x04|0x00),
      ETXSTH         =  (0x05|0x00),
      ETXND          =  (0x06|0x00),
      ETXNDL         =  (0x06|0x00),
      ETXNDH         =  (0x07|0x00),
      ERXST          =  (0x08|0x00),
      ERXSTL         =  (0x08|0x00),
      ERXSTH         =  (0x09|0x00),
      ERXND          =  (0x0A|0x00),
      ERXNDL         =  (0x0A|0x00),
      ERXNDH         =  (0x0B|0x00),
      ERXRDPT        =  (0x0C|0x00),
      ERXRDPTL       =  (0x0C|0x00),
      ERXRDPTH       =  (0x0D|0x00),
      ERXWRPTL       =  (0x0E|0x00),
      ERXWRPTH       =  (0x0F|0x00),
      EDMASTL        =  (0x10|0x00),
      EDMASTH        =  (0x11|0x00),
      EDMANDL        =  (0x12|0x00),
      EDMANDH        =  (0x13|0x00),
      EDMADSTL       =  (0x14|0x00),
      EDMADSTH       =  (0x15|0x00),
      EDMACSL        =  (0x16|0x00),
      EDMACSH        =  (0x17|0x00),

      // Bank 1 registers
      EHT0           =  (0x00|0x20),
      EHT1           =  (0x01|0x20),
      EHT2           =  (0x02|0x20),
      EHT3           =  (0x03|0x20),
      EHT4           =  (0x04|0x20),
      EHT5           =  (0x05|0x20),
      EHT6           =  (0x06|0x20),
      EHT7           =  (0x07|0x20),
      EPMM0          =  (0x08|0x20),
      EPMM1          =  (0x09|0x20),
      EPMM2          =  (0x0A|0x20),
      EPMM3          =  (0x0B|0x20),
      EPMM4          =  (0x0C|0x20),
      EPMM5          =  (0x0D|0x20),
      EPMM6          =  (0x0E|0x20),
      EPMM7          =  (0x0F|0x20),
      EPMCSL         =  (0x10|0x20),
      EPMCSH         =  (0x11|0x20),
      EPMOL          =  (0x14|0x20),
      EPMOH          =  (0x15|0x20),
      EWOLIE         =  (0x16|0x20),
      EWOLIR         =  (0x17|0x20),
      ERXFCON        =  (0x18|0x20),
      EPKTCNT        =  (0x19|0x20),

      // Bank 2 registers
      MACON1         =  (0x00|0x40|0x80),
      MACON2         =  (0x01|0x40|0x80),
      MACON3         =  (0x02|0x40|0x80),
      MACON4         =  (0x03|0x40|0x80),
      MABBIPG        =  (0x04|0x40|0x80),
      MAIPGL         =  (0x06|0x40|0x80),
      MAIPGH         =  (0x07|0x40|0x80),
      MACLCON1       =  (0x08|0x40|0x80),
      MACLCON2       =  (0x09|0x40|0x80),
      MAMXFL         =  (0x0A|0x40|0x80),
      MAMXFLL        =  (0x0A|0x40|0x80),
      MAMXFLH        =  (0x0B|0x40|0x80),
      MAPHSUP        =  (0x0D|0x40|0x80),
      MICON          =  (0x11|0x40|0x80),
      MICMD          =  (0x12|0x40|0x80),
      MIREGADR       =  (0x14|0x40|0x80),
      MIWR           =  (0x16|0x40|0x80),
      MIWRL          =  (0x16|0x40|0x80),
      MIWRH          =  (0x17|0x40|0x80),
      MIRDL          =  (0x18|0x40|0x80),
      MIRDH          =  (0x19|0x40|0x80),

      // Bank 3 registers
      MAADR1         =  (0x00|0x60|0x80),
      MAADR0         =  (0x01|0x60|0x80),
      MAADR3         =  (0x02|0x60|0x80),
      MAADR2         =  (0x03|0x60|0x80),
      MAADR5         =  (0x04|0x60|0x80),
      MAADR4         =  (0x05|0x60|0x80),
      EBSTSD         =  (0x06|0x60),
      EBSTCON        =  (0x07|0x60),
      EBSTCSL        =  (0x08|0x60),
      EBSTCSH        =  (0x09|0x60),
      MISTAT         =  (0x0A|0x60|0x80),
      EREVID         =  (0x12|0x60),
      ECOCON         =  (0x15|0x60),
      EFLOCON        =  (0x17|0x60),
      EPAUSL         =  (0x18|0x60),
      EPAUSH         =  (0x19|0x60),

      // PHY registers
      PHCON1          =   0x00,
      PHSTAT1         =   0x01,
      PHHID1          =   0x02,
      PHHID2          =   0x03,
      PHCON2          =   0x10,
      PHSTAT2         =   0x11,
      PHIE            =   0x12,
      PHIR            =   0x13,
      PHLCON          =   0x14,

      // ENC28J60 ERXFCON Register Bit Definitions
      ERXFCON_UCEN    =   0x80,
      ERXFCON_ANDOR   =   0x40,
      ERXFCON_CRCEN   =   0x20,
      ERXFCON_PMEN    =   0x10,
      ERXFCON_MPEN    =   0x08,
      ERXFCON_HTEN    =   0x04,
      ERXFCON_MCEN    =   0x02,
      ERXFCON_BCEN    =   0x01,

      // ENC28J60 EIE Register Bit Definitions
      EIE_INTIE       =   0x80,
      EIE_PKTIE       =   0x40,
      EIE_DMAIE       =   0x20,
      EIE_LINKIE      =   0x10,
      EIE_TXIE        =   0x08,
      EIE_WOLIE       =   0x04,
      EIE_TXERIE      =   0x02,
      EIE_RXERIE      =   0x01,

      // ENC28J60 EIR Register Bit Definitions
      EIR_PKTIF       =   0x40,
      EIR_DMAIF       =   0x20,
      EIR_LINKIF      =   0x10,
      EIR_TXIF        =   0x08,
      EIR_WOLIF       =   0x04,
      EIR_TXERIF      =   0x02,
      EIR_RXERIF      =   0x01,

      // ENC28J60 ESTAT Register Bit Definitions
      ESTAT_INT       =   0x80,
      ESTAT_LATECOL   =   0x10,
      ESTAT_RXBUSY    =   0x04,
      ESTAT_TXABRT    =   0x02,
      ESTAT_CLKRDY    =   0x01,

      // ENC28J60 ECON2 Register Bit Definitions
      ECON2_AUTOINC   =   0x80,
      ECON2_PKTDEC    =   0x40,
      ECON2_PWRSV     =   0x20,
      ECON2_VRPS      =   0x08,

      // ENC28J60 ECON1 Register Bit Definitions
      ECON1_TXRST     =   0x80,
      ECON1_RXRST     =   0x40,
      ECON1_DMAST     =   0x20,
      ECON1_CSUMEN    =   0x10,
      ECON1_TXRTS     =   0x08,
      ECON1_RXEN      =   0x04,
      ECON1_BSEL1     =   0x02,
      ECON1_BSEL0     =   0x01,

      // ENC28J60 MACON1 Register Bit Definitions
      MACON1_LOOPBK   =   0x10,
      MACON1_TXPAUS   =   0x08,
      MACON1_RXPAUS   =   0x04,
      MACON1_PASSALL  =   0x02,
      MACON1_MARXEN   =   0x01,

      // ENC28J60 MACON2 Register Bit Definitions
      MACON2_MARST    =   0x80,
      MACON2_RNDRST   =   0x40,
      MACON2_MARXRST  =   0x08,
      MACON2_RFUNRST  =   0x04,
      MACON2_MATXRST  =   0x02,
      MACON2_TFUNRST  =   0x01,

      // ENC28J60 MACON3 Register Bit Definitions
      MACON3_PADCFG2  =   0x80,
      MACON3_PADCFG1  =   0x40,
      MACON3_PADCFG0  =   0x20,
      MACON3_TXCRCEN  =   0x10,
      MACON3_PHDRLEN  =   0x08,
      MACON3_HFRMLEN  =   0x04,
      MACON3_FRMLNEN  =   0x02,
      MACON3_FULDPX   =   0x01,

      // ENC28J60 MICMD Register Bit Definitions
      MICMD_MIISCAN   =   0x02,
      MICMD_MIIRD     =   0x01,

      // ENC28J60 MISTAT Register Bit Definitions
      MISTAT_NVALID   =   0x04,
      MISTAT_SCAN     =   0x02,
      MISTAT_BUSY     =   0x01,

      // ENC28J60 PHY PHCON1 Register Bit Definitions
      PHCON1_PRST     =   0x8000,
      PHCON1_PLOOPBK  =   0x4000,
      PHCON1_PPWRSV   =   0x0800,
      PHCON1_PDPXMD   =   0x0100,

      // ENC28J60 PHY PHSTAT1 Register Bit Definitions
      PHSTAT1_PFDPX   =   0x1000,
      PHSTAT1_PHDPX   =   0x0800,
      PHSTAT1_LLSTAT  =   0x0004,
      PHSTAT1_JBSTAT  =   0x0002,

      // ENC28J60 PHY PHCON2 Register Bit Definitions
      PHCON2_FRCLINK  =   0x4000,
      PHCON2_TXDIS    =   0x2000,
      PHCON2_JABBER   =   0x0400,
      PHCON2_HDLDIS   =   0x0100,

      // ENC28J60 Packet Control Byte Bit Definitions
      PKTCTRL_PHUGEEN   =   0x08,
      PKTCTRL_PPADEN    =   0x04,
      PKTCTRL_PCRCEN    =   0x02,
      PKTCTRL_POVERRIDE =   0x01,

      // SPI operation codes
      ENC28J60_READ_CTRL_REG   =   0x00,
      ENC28J60_READ_BUF_MEM    =   0x3A,
      ENC28J60_WRITE_CTRL_REG  =   0x40,
      ENC28J60_WRITE_BUF_MEM   =   0x7A,
      ENC28J60_BIT_FIELD_SET   =   0x80,
      ENC28J60_BIT_FIELD_CLR   =   0xA0,
      ENC28J60_SOFT_RESET      =   0xFF
    } ControlReg;

   // The RXSTART_INIT should be zero. See Rev. B4 Silicon Errata
   // buffer boundaries applied to internal 8K ram
   // the entire available packet buffer space is allocated
   //
   // start with recbuf at 0/
   const WORD RXSTART_INIT   = (WORD)0x0000;

   // receive buffer end
   const WORD RXSTOP_INIT    = (WORD)(0x1FFF-0x0600-1);

   // start TX buffer at 0x1FFF-0x0600, pace for one full ethernet frame (~1500 bytes)
   const WORD TXSTART_INIT   =  (WORD)(0x1FFF-0x0600);

   // stp TX buffer at end of mem
   const WORD TXSTOP_INIT    =  (WORD)0x1FFF;

   // max frame length which the conroller will accept:
   const WORD  MAX_FRAMELEN  =      1500;       // (note: maximum ethernet frame length would be 1518)

	public:
    //---------------------------------------------------------------
	  /*! ...
        \param ...
    */
    Ethernet_ENC28J60( const NetAddr<6>  &addrPhy,
                       SPImaster::Device &spi,
                       Timer             &timer );

  private:
    //---------------------------------------------------------------
    // override
    virtual void create( const NetAddr<6> destAddr, const WORD type );

    //---------------------------------------------------------------
    // override
    virtual bool isLinked( void )
    {
        // todo
        return( true );
    };

    //---------------------------------------------------------------
    // override
    virtual void PacketSend( BYTE* packet, WORD len );

    //---------------------------------------------------------------
    // override
		virtual bool PacketReceive( void );

    //---------------------------------------------------------------
    // override
    virtual WORD getType( void );

  private:
    //---------------------------------------------------------------
    void InitEth(const NetAddr<6>  &addrPhy);

    //---------------------------------------------------------------
    virtual bool Init (void);

    //---------------------------------------------------------------
	  BYTE ReadOp( BYTE op, BYTE address );

    //---------------------------------------------------------------
		void WriteOp( BYTE op, BYTE address, BYTE data );

    //---------------------------------------------------------------
		void ReadBuffer( WORD len, BYTE* data );

    //---------------------------------------------------------------
		void WriteBuffer( WORD len, BYTE* data );

    //---------------------------------------------------------------
		void SetBank( BYTE address );

    //---------------------------------------------------------------
		BYTE Read( BYTE address );

    //---------------------------------------------------------------
		void WriteByte( BYTE address, BYTE data );

    //---------------------------------------------------------------
		void WriteWord( BYTE address, WORD data );

    //---------------------------------------------------------------
		void PhyWrite( BYTE address, WORD data );

    //---------------------------------------------------------------
		void clkout( BYTE clk );

    //---------------------------------------------------------------
		BYTE getrev( void );

	private:
    //---------------------------------------------------------------
    SPImaster::Device &spi;
    const NetAddr<6>  &addrPhy;
    BYTE               bank;
    WORD               nextPacketPtr;

}; //class Ethernet_ENC28J60

}  } //namespace

#endif
