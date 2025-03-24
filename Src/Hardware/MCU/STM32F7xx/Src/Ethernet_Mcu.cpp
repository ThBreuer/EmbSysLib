//*******************************************************************
/*!
\file   Ethernet_Mcu.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   18.03.2024

License: See file "LICENSE"

This implementation is a redesign of
 "stm32f4x7_eth.h" and "stm32f4x7_eth.c"
 MCD Application Team, V1.0.0, 14-October-2011, COPYRIGHT 2011 STMicroelectronics
modified by
 CMP Team, V1.0.0, 28-December-2012, COPYRIGHT 2012 Embest Tech. Co., Ltd.
*/

//*******************************************************************
#include "Ethernet_Mcu.h"
#include "Hardware/Common/Net/Net.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************

//-------------------------------------------------------------------
/**

Ethernet DMA descriptors registers bits definition

\see STM32F76xxx Reference Manual, Chapter "Ethernet (ETH): media access control (MAC) with DMA controller"

**/
//-------------------------------------------------------------------
//
// Bit definition of TDES0 register: DMA Tx descriptor status register
//
#define ETH_DMATxDesc_OWN                     ((uint32_t)0x80000000)  // OWN bit: descriptor is owned by DMA engine
#define ETH_DMATxDesc_IC                      ((uint32_t)0x40000000)  // Interrupt on Completion
#define ETH_DMATxDesc_LS                      ((uint32_t)0x20000000)  // Last Segment
#define ETH_DMATxDesc_FS                      ((uint32_t)0x10000000)  // First Segment
#define ETH_DMATxDesc_DC                      ((uint32_t)0x08000000)  // Disable CRC
#define ETH_DMATxDesc_DP                      ((uint32_t)0x04000000)  // Disable Padding
#define ETH_DMATxDesc_TTSE                    ((uint32_t)0x02000000)  // Transmit Time Stamp Enable
#define ETH_DMATxDesc_CIC                     ((uint32_t)0x00C00000)  // Checksum Insertion Control: 4 cases
#define ETH_DMATxDesc_CIC_ByPass              ((uint32_t)0x00000000)  // Do Nothing: Checksum Engine is bypassed
#define ETH_DMATxDesc_CIC_IPV4Header          ((uint32_t)0x00400000)  // IPV4 header Checksum Insertion
#define ETH_DMATxDesc_CIC_TCPUDPICMP_Segment  ((uint32_t)0x00800000)  // TCP/UDP/ICMP Checksum Insertion calculated over segment only
#define ETH_DMATxDesc_CIC_TCPUDPICMP_Full     ((uint32_t)0x00C00000)  // TCP/UDP/ICMP Checksum Insertion fully calculated
#define ETH_DMATxDesc_TER                     ((uint32_t)0x00200000)  // Transmit End of Ring
#define ETH_DMATxDesc_TCH                     ((uint32_t)0x00100000)  // Second Address Chained
#define ETH_DMATxDesc_TTSS                    ((uint32_t)0x00020000)  // Tx Time Stamp Status
#define ETH_DMATxDesc_IHE                     ((uint32_t)0x00010000)  // IP Header Error
#define ETH_DMATxDesc_ES                      ((uint32_t)0x00008000)  // Error summary: OR of the following bits: UE || ED || EC || LCO || NC || LCA || FF || JT
#define ETH_DMATxDesc_JT                      ((uint32_t)0x00004000)  // Jabber Timeout
#define ETH_DMATxDesc_FF                      ((uint32_t)0x00002000)  // Frame Flushed: DMA/MTL flushed the frame due to SW flush
#define ETH_DMATxDesc_PCE                     ((uint32_t)0x00001000)  // Payload Checksum Error
#define ETH_DMATxDesc_LCA                     ((uint32_t)0x00000800)  // Loss of Carrier: carrier lost during transmission
#define ETH_DMATxDesc_NC                      ((uint32_t)0x00000400)  // No Carrier: no carrier signal from the transceiver
#define ETH_DMATxDesc_LCO                     ((uint32_t)0x00000200)  // Late Collision: transmission aborted due to collision
#define ETH_DMATxDesc_EC                      ((uint32_t)0x00000100)  // Excessive Collision: transmission aborted after 16 collisions
#define ETH_DMATxDesc_VF                      ((uint32_t)0x00000080)  // VLAN Frame
#define ETH_DMATxDesc_CC                      ((uint32_t)0x00000078)  // Collision Count
#define ETH_DMATxDesc_ED                      ((uint32_t)0x00000004)  // Excessive Deferral
#define ETH_DMATxDesc_UF                      ((uint32_t)0x00000002)  // Underflow Error: late data arrival from the memory
#define ETH_DMATxDesc_DB                      ((uint32_t)0x00000001)  // Deferred Bit

//-------------------------------------------------------------------
//
// Bit definition of TDES1 register
//
#define ETH_DMATxDesc_TBS2  ((uint32_t)0x1FFF0000)  // Transmit Buffer2 Size
#define ETH_DMATxDesc_TBS1  ((uint32_t)0x00001FFF)  // Transmit Buffer1 Size

//-------------------------------------------------------------------
//
// Bit definition of RDES0 register: DMA Rx descriptor status register
//
#define ETH_DMARxDesc_OWN         ((uint32_t)0x80000000)  // OWN bit: descriptor is owned by DMA engine
#define ETH_DMARxDesc_AFM         ((uint32_t)0x40000000)  // DA Filter Fail for the rx frame
#define ETH_DMARxDesc_FL          ((uint32_t)0x3FFF0000)  // Receive descriptor frame length
#define ETH_DMARxDesc_ES          ((uint32_t)0x00008000)  // Error summary: OR of the following bits: DE || OE || IPC || LC || RWT || RE || CE
#define ETH_DMARxDesc_DE          ((uint32_t)0x00004000)  // Descriptor error: no more descriptors for receive frame
#define ETH_DMARxDesc_SAF         ((uint32_t)0x00002000)  // SA Filter Fail for the received frame
#define ETH_DMARxDesc_LE          ((uint32_t)0x00001000)  // Frame size not matching with length field
#define ETH_DMARxDesc_OE          ((uint32_t)0x00000800)  // Overflow Error: Frame was damaged due to buffer overflow
#define ETH_DMARxDesc_VLAN        ((uint32_t)0x00000400)  // VLAN Tag: received frame is a VLAN frame
#define ETH_DMARxDesc_FS          ((uint32_t)0x00000200)  // First descriptor of the frame
#define ETH_DMARxDesc_LS          ((uint32_t)0x00000100)  // Last descriptor of the frame
#define ETH_DMARxDesc_IPV4HCE     ((uint32_t)0x00000080)  // IPC Checksum Error: Rx Ipv4 header checksum error
#define ETH_DMARxDesc_LC          ((uint32_t)0x00000040)  // Late collision occurred during reception
#define ETH_DMARxDesc_FT          ((uint32_t)0x00000020)  // Frame type - Ethernet, otherwise 802.3
#define ETH_DMARxDesc_RWT         ((uint32_t)0x00000010)  // Receive Watchdog Timeout: watchdog timer expired during reception
#define ETH_DMARxDesc_RE          ((uint32_t)0x00000008)  // Receive error: error reported by MII interface
#define ETH_DMARxDesc_DBE         ((uint32_t)0x00000004)  // Dribble bit error: frame contains non int multiple of 8 bits
#define ETH_DMARxDesc_CE          ((uint32_t)0x00000002)  // CRC error
#define ETH_DMARxDesc_MAMPCE      ((uint32_t)0x00000001)  // Rx MAC Address/Payload Checksum Error: Rx MAC address matched/ Rx Payload Checksum Error

#define ETH_DMARxDesc_FL_Pos      (16U)  // Position of ETH_DMARxDesc_FL

//-------------------------------------------------------------------
//
// Bit definition of RDES1 register
//
#define ETH_DMARxDesc_DIC   ((uint32_t)0x80000000)  // Disable Interrupt on Completion
#define ETH_DMARxDesc_RBS2  ((uint32_t)0x1FFF0000)  // Receive Buffer2 Size
#define ETH_DMARxDesc_RER   ((uint32_t)0x00008000)  // Receive End of Ring
#define ETH_DMARxDesc_RCH   ((uint32_t)0x00004000)  // Second Address Chained
#define ETH_DMARxDesc_RBS1  ((uint32_t)0x00001FFF)  // Receive Buffer1 Size

//-------------------------------------------------------------------
/**

 Description of common PHY registers

\see Microchip LAN8742A Datasheet, Chapter "Control and Status Registers"

**/
//-------------------------------------------------------------------
//
// PHY Basic Control Register
// Register Index: 0
//
#define PHY_Reset                       ((uint16_t)0x8000)  // PHY Reset
#define PHY_Loopback                    ((uint16_t)0x4000)  // Select loop-back mode
#define PHY_FULLDUPLEX_100M             ((uint16_t)0x2100)  // Set the full-duplex mode at 100 Mb/s
#define PHY_HALFDUPLEX_100M             ((uint16_t)0x2000)  // Set the half-duplex mode at 100 Mb/s
#define PHY_FULLDUPLEX_10M              ((uint16_t)0x0100)  // Set the full-duplex mode at 10 Mb/s
#define PHY_HALFDUPLEX_10M              ((uint16_t)0x0000)  // Set the half-duplex mode at 10 Mb/s
#define PHY_AutoNegotiation             ((uint16_t)0x1000)  // Enable auto-negotiation function
#define PHY_Restart_AutoNegotiation     ((uint16_t)0x0200)  // Restart auto-negotiation function
#define PHY_Powerdown                   ((uint16_t)0x0800)  // Select the power down mode
#define PHY_Isolate                     ((uint16_t)0x0400)  // Isolate PHY from MII

//-------------------------------------------------------------------
//
// PHY Basic Status Register
// Register Index: 1
//
#define PHY_AutoNego_Complete           ((uint16_t)0x0020)  // Auto-Negotiation process completed
#define PHY_Linked_Status               ((uint16_t)0x0004)  // Valid link established
#define PHY_Jabber_detection            ((uint16_t)0x0002)  // Jabber condition detected

//-------------------------------------------------------------------
//
// PHY Special Control/Status Register
// Register Index: 31 (vendor specific)
//
/* The Speed and Duplex mask values change from a PHY to another, so the user
   have to update this value depending on the used external PHY */
#define PHY_DUPLEX_SPEED_STATUS_MASK  (0b111<<2)
#define PHY_10M_HALF                  (0b001<<2)
#define PHY_10M_FULL                  (0b101<<2)
#define PHY_100BTX_HALF               (0b010<<2)
#define PHY_100BTX_FULL               (0b110<<2)

//-------------------------------------------------------------------
//
// PHY Register Index
//
//*--------------------------------------------------------------------------*
#define PHY_BCR   ((uint16_t) 0)  // Transceiver Basic Control Register
#define PHY_BSR   ((uint16_t) 1)  // Transceiver Basic Status Register

// The PHY status register value change from a PHY to another,
// so the user have to update this value depending on the used external PHY
#define PHY_SR    ((uint16_t)31)  // Value for DP83848 PHY

//*******************************************************************
//
// DMA Buffer allocation
//
//*******************************************************************
#if defined   (__CC_ARM) // ARM Compiler

  #warning "not tested !!!"
  Ethernet_Mcu::DMA_Buffer Ethernet_Mcu::dmaBuffer __attribute__((at(0x20000000)));;

#elif defined ( __ICCARM__ ) // IAR Compiler


  #pragma data_alignment=4
  Ethernet_Mcu::DMA_Buffer Ethernet_Mcu::dmaBuffer;

#elif defined (__GNUC__) // GNU Compiler

  Ethernet_Mcu::DMA_Buffer Ethernet_Mcu::dmaBuffer __attribute__ ((aligned (4)));

#elif defined  (__TASKING__) // TASKING Compiler

  #warning "not tested !!!"
  __align(4)
  Ethernet_Mcu::DMA_Buffer Ethernet_Mcu::dmaBuffer;

#else
  #error "Compiler not defined"
#endif

//*******************************************************************
//
// Ethernet_Mcu
//
//*******************************************************************
//-------------------------------------------------------------------
Ethernet_Mcu::Ethernet_Mcu( const NetAddr<6> &addrPhyIn, Timer &timer )

: Ethernet( addrPhyIn, timer )

{
  isAutoNegotiation =  true;

  //MAC Control Register
  speed =  ETH_MACCR_FES; // fast speed (100M)
  mode  =  ETH_MACCR_DM;  // full duplex mode = true

  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;  // Enable SYSCFG clock

  // MII/RMII Media interface selection
  SYSCFG->PMC |= SYSCFG_PMC_MII_RMII_SEL; // MediaInterface RMII = true;

  const uint16_t pmode = PinConfig::HIGH_SPEED | PinConfig::NO_PUPD;

  PinConfig::set( PinConfig::ETH_TX_EN,  pmode | PinConfig::PUSH_PULL  );
  PinConfig::set( PinConfig::ETH_TXD0,   pmode | PinConfig::PUSH_PULL  );
  PinConfig::set( PinConfig::ETH_TXD1,   pmode | PinConfig::PUSH_PULL  );
  PinConfig::set( PinConfig::ETH_RXD0,   pmode | PinConfig::INPUT      );
  PinConfig::set( PinConfig::ETH_RXD1,   pmode | PinConfig::INPUT      );

  //ETH_MII_RX_ER_PD_5 rxer 7 output from LAN
  //PinConfig::set( PinConfig::ETH_RX_ER, pmode | PinConfig::INPUT      );

  PinConfig::set( PinConfig::ETH_RX_DV,  pmode | PinConfig::OPEN_DRAIN );
  PinConfig::set( PinConfig::ETH_MDC,    pmode | PinConfig::PUSH_PULL  );
  PinConfig::set( PinConfig::ETH_MDIO,   pmode | PinConfig::OPEN_DRAIN );
  PinConfig::set( PinConfig::ETH_RX_CLK, pmode | PinConfig::OPEN_DRAIN );

  // Clock enable
  RCC->AHB1ENR |= (  RCC_AHB1ENR_ETHMACEN
                   | RCC_AHB1ENR_ETHMACTXEN
                   | RCC_AHB1ENR_ETHMACRXEN
                   | RCC_AHB1ENR_ETHMACPTPEN );

  // Reset ETHERNET on AHB Bus
  RCC->AHB1RSTR |= RCC_AHB1RSTR_ETHMACRST;
  RCC->AHB1RSTR &= ~RCC_AHB1RSTR_ETHMACRST;

  // Software reset
  // Set the SWR bit: resets all MAC subsystem internal registers and logic
  // After reset all the registers holds their respective reset values
  ETH->DMABMR |= ETH_DMABMR_SR;

  while( ETH->DMABMR & ETH_DMABMR_SR )  // Wait for software reset
  {
    asm("NOP");
  }

  /*------------------------   DMA   -----------------------------------*/

  for( BYTE id = 0;id<32;id++ )
  {
    WritePHYRegister(id, PHY_BCR, PHY_Reset);
  }
  System::delayMilliSec(10);

  PhyAddr = 0;
  while( 32 > PhyAddr )
  {
    DWORD id =   ((DWORD)ReadPHYRegister(PhyAddr,2)<<16)
               | ((DWORD)ReadPHYRegister(PhyAddr,3)    );

    // datasheet for the ks8721bl ethernet controller (http://www.micrel.com/_PDF/Ethernet/datasheets/ks8721bl-sl.pdf)
    // page 20 --> PHY Identifier 1 and 2
    // \todo mask model and revision number
    if(    (0x00221619 == id)
        || (0x0007C0F1 == id)
        || (0x0007C131 == id) )
    {
      break;
    }
    PhyAddr++;
  }

  if( 32 <= PhyAddr )
  {
    // \todo reportError, Ethernet Phy Not Found
    while(1);
  }

  // Configure Ethernet
  if ( !InitEth() )
  {
    // \todo reportError
    while(1);
  }
}

//-------------------------------------------------------------------
WORD Ethernet_Mcu::getType( )
{
  Ethernet::Header *eth =(Ethernet::Header *)buf;

  if     ( plen >= sizeof( NetEthIP::Msg ) && eth->type == TYPE_IP )
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
bool Ethernet_Mcu::isLinked( void )
{
  return( ReadPHYRegister(PhyAddr, PHY_BSR) & PHY_Linked_Status );
}

//-------------------------------------------------------------------
void Ethernet_Mcu::PacketSend(  uint8_t* packet, uint16_t len)
{
  // \todo only len < ETH_MAX_PACKET_SIZE fits, use multiple frames
  memcpy( (uint8_t*)TxDescToSet->Buffer1Addr, packet, len );
  Prepare_Transmit_Descriptors( len );
}

//-------------------------------------------------------------------
// Gets a packet from the network receive buffer, if one is available.
// The packet will be headed by an ethernet header.
//      maxlen  The maximum acceptable length of a retrieved packet.
//      packet  Pointer where packet data should be stored.
// Returns: Packet length in bytes if a packet was retrieved, zero otherwise.
bool Ethernet_Mcu::PacketReceive()
{
  if( CheckFrameReceived() )
  {
    DMA_Frame frame = Get_Received_Frame();

    plen = frame.length;  // Obtain the size of the packet

    // todo: pruefen auf maxLen (BUFFER_SIZE)

    memcpy( buf, (uint8_t *)frame.buffer, plen );

    // Release descriptors to DMA
    // Check if frame with multiple DMA buffer segments
    __IO DMA_Descriptor *DMARxNextDesc = (rxFrame.Seg_Count > 1) ? rxFrame.FS_Rx_Desc : frame.descriptor;

    // Set Own bit in Rx descriptors: gives the buffers back to DMA
    for( uint32_t i=0; i<rxFrame.Seg_Count; i++ )
    {
      DMARxNextDesc->Status = ETH_DMARxDesc_OWN;
      DMARxNextDesc = (DMA_Descriptor *)(DMARxNextDesc->Buffer2NextDescAddr);
    }

    rxFrame.Seg_Count = 0; // Clear Segment_Count

    if( ETH->DMASR & ETH_DMASR_RBUS )  // When Rx Buffer unavailable flag is set: clear it and resume reception
    {
      ETH->DMASR   = ETH_DMASR_RBUS;   // Clear RBUS ETHERNET DMA flag */
      ETH->DMARPDR = 0;                // Resume DMA reception */
    }
    return( true );
  }
  return( false );
}

//-------------------------------------------------------------------
void Ethernet_Mcu::create( const NetAddr<6> destAddr,
                                   const WORD    type )
{
  Ethernet::Header *eth = (Ethernet::Header*)buf;

  eth->destinationAddr = destAddr;
  eth->sourceAddr      = addrPhy;
  eth->type            = type;
}

//-------------------------------------------------------------------
// DMA Descriptors functions
//-------------------------------------------------------------------
//-------------------------------------------------------------------
Ethernet_Mcu::DMA_Frame Ethernet_Mcu::Get_Received_Frame( void )
{
  uint32_t framelength = 0;

  DMA_Frame frame = {0,0,0};

  // Get the Frame Length of the received packet: substruct 4 bytes of the CRC
  framelength  = ((RxDescToGet->Status & ETH_DMARxDesc_FL) >> ETH_DMARxDesc_FL_Pos) - 4;
  frame.length = framelength;

  // Get the address of the buffer start address
  if( rxFrame.Seg_Count > 1 )  // Check if more than one segment in the frame
  {
    frame.buffer = (rxFrame.FS_Rx_Desc)->Buffer1Addr;
  }
  else
  {
    frame.buffer = RxDescToGet->Buffer1Addr;
  }

  frame.descriptor = RxDescToGet;

  // Update the ETHERNET DMA global Rx descriptor with next Rx descriptor
  // Chained Mode
  // Selects the next DMA Rx descriptor list for next buffer to read
  RxDescToGet = RxDescToGet->Buffer2NextDescAddr;

  return( frame );  // Return Frame
}

//-------------------------------------------------------------------
bool Ethernet_Mcu::Prepare_Transmit_Descriptors( uint16_t frameLength )
{

  if( TxDescToSet->Status & ETH_DMATxDesc_OWN ) // Check if the descriptor is owned by the
                                                // ETHERNET DMA (when set) or CPU (when reset)
  {
    return false;  // Return ERROR: OWN bit set
  }

  // Set at least one segment, even frameLength is zero
  uint16_t res = frameLength;
  do
  {
    uint32_t status = 0;

    if( res == frameLength)  // First segment ?
    {
      status |= ETH_DMATxDesc_FS;
    }

    uint32_t size = MIN( res, (uint16_t)ETH_MAX_PACKET_SIZE );
    res -= size;

    if( !res )  // Last segment ?
    {
      status |=  ETH_DMATxDesc_LS;
    }

    TxDescToSet->Status            &= ~(ETH_DMATxDesc_FS | ETH_DMATxDesc_LS);  // Clear FIRST and LAST segment bits
    TxDescToSet->Status            |= status;                                  // Set new segment bits
    TxDescToSet->ControlBufferSize  = (size & ETH_DMATxDesc_TBS1);             // Set size
    TxDescToSet->Status            |= ETH_DMATxDesc_OWN;                       // Set Own bit of the Tx descriptor Status:
                                                                               // gives the buffer back to ETHERNET DMA
    TxDescToSet = TxDescToSet->Buffer2NextDescAddr;                            // Next Descriptor
  } while( res );

  if( ETH->DMASR & ETH_DMASR_TBUS )  // Tx Buffer unavailable ?
  {
    ETH->DMASR   = ETH_DMASR_TBUS;   // Clear TBUS ETHERNET DMA flag
    ETH->DMATPDR = 0;                // Resume DMA transmission
  }
  return true; // Return SUCCESS
}

//-------------------------------------------------------------------
bool Ethernet_Mcu::CheckFrameReceived(void)
{
  if( !(RxDescToGet->Status & ETH_DMARxDesc_OWN)  )
  {
    if( !(RxDescToGet->Status & ETH_DMARxDesc_LS) )
    {
      if( !(RxDescToGet->Status & ETH_DMARxDesc_FS) )
      {
        // !OWN and !LS and !FS
        rxFrame.Seg_Count++;
        RxDescToGet = RxDescToGet->Buffer2NextDescAddr;
      }
      else
      {
        // !OWN and !LS and FS
        rxFrame.FS_Rx_Desc = RxDescToGet;
        rxFrame.LS_Rx_Desc = NULL;
        rxFrame.Seg_Count = 1;
        RxDescToGet = RxDescToGet->Buffer2NextDescAddr;
      }
    }
    else
    {
      // !OWN and LS
      rxFrame.LS_Rx_Desc = RxDescToGet;
      rxFrame.Seg_Count++;
      return( true );
    }
  }
  return( false );
}

//-------------------------------------------------------------------
void Ethernet_Mcu::DMARxDescChainInit( void )
{
  RxDescToGet = dmaBuffer.RxDscrTab; // Set the RxDescToGet pointer with the first one of the RxDescTab list

  for( uint32_t i=0; i < ETH_RXBUFNB; i++ )  // Fill each RxDesc descriptor with the right values
  {
    dmaBuffer.RxDscrTab[i].Status              = ETH_DMARxDesc_OWN;                              // Set Own bit of the Rx descriptor Status
    dmaBuffer.RxDscrTab[i].ControlBufferSize   = ETH_DMARxDesc_RCH | (uint32_t)ETH_MAX_PACKET_SIZE;  // Set Buffer1 size and Second Address Chained bit
    dmaBuffer.RxDscrTab[i].Buffer1Addr         = &dmaBuffer.RxBuff[i][0];                        // Set Buffer1 address pointer */
    dmaBuffer.RxDscrTab[i].Buffer2NextDescAddr = &dmaBuffer.RxDscrTab[(i+1)%ETH_RXBUFNB];        // Set next descriptor address register with next descriptor base address
  }

  ETH->DMARDLAR = (uint32_t)dmaBuffer.RxDscrTab;  // Set Receive Descriptor List Address Register
}

//-------------------------------------------------------------------
void Ethernet_Mcu::DMATxDescChainInit( void )
{
  TxDescToSet = dmaBuffer.TxDscrTab;  // Set the TxDescToSet pointer with the first one of the TxDescTab list

  for( uint32_t i=0; i < ETH_TXBUFNB; i++ )  // Fill each DMATxDesc descriptor with the right values
  {
    dmaBuffer.TxDscrTab[i].Status              = ETH_DMATxDesc_TCH;                        // Set Second Address Chained bit
    dmaBuffer.TxDscrTab[i].Buffer1Addr         = &dmaBuffer.TxBuff[i][0];                  // Set Buffer1 address pointer
    dmaBuffer.TxDscrTab[i].Buffer2NextDescAddr = &dmaBuffer.TxDscrTab[(i+1)%ETH_TXBUFNB];  // Set next descriptor address register with next descriptor base address, wrap around
  }

  ETH->DMATDLAR = (uint32_t)dmaBuffer.TxDscrTab;  // Set Transmit Desciptor List Address Register
}

//-------------------------------------------------------------------
// Global ETH MAC/DMA functions
//-------------------------------------------------------------------
//-------------------------------------------------------------------
bool Ethernet_Mcu::InitEth( void )
{
  uint32_t tmpreg = 0;

  //-----------------------------------------------------------------
  // MAC Config
  //-----------------------------------------------------------------
  // Configure the ETHERNET CSR Clock Range
  tmpreg  = ETH->MACMIIAR;
  tmpreg &= ~ETH_MACMIIAR_CR_Msk;  // Clear CSR Clock Range CR[2:0] bits

  // Set CR bits depending on hclk value
  if     ( system_PCLK <  20000000 )    { /* no change*/                    }
  else if( system_PCLK <  35000000 )    { tmpreg |= ETH_MACMIIAR_CR_Div16;  }
  else if( system_PCLK <  60000000 )    { tmpreg |= ETH_MACMIIAR_CR_Div26;  }
  else if( system_PCLK < 100000000 )    { tmpreg |= ETH_MACMIIAR_CR_Div42;  }
  else if( system_PCLK < 150000000 )    { tmpreg |= ETH_MACMIIAR_CR_Div62;  }
  else                                  { tmpreg |= ETH_MACMIIAR_CR_Div102; }

  ETH->MACMIIAR = tmpreg;

  //-----------------------------------------------------------------
  // PHY initialization and configuration
  //-----------------------------------------------------------------
  System::delayMilliSec(10);  // Delay to assure PHY reset


  //-----------------------------------------------------------------
  tmpreg  = ETH->MACCR;

  tmpreg &= ~(   ETH_MACCR_WD // Clear
               | ETH_MACCR_JD
               | ETH_MACCR_IFG
               | ETH_MACCR_CSD
               | ETH_MACCR_FES
               | ETH_MACCR_ROD
               | ETH_MACCR_LM
               | ETH_MACCR_DM
               | ETH_MACCR_IPCO
               | ETH_MACCR_RD
               | ETH_MACCR_APCS
               | ETH_MACCR_BL
               | ETH_MACCR_DC
               | ETH_MACCR_TE
               | ETH_MACCR_RE );

  tmpreg |= (   !ETH_MACCR_WD   // watchdog disable
              | !ETH_MACCR_JD   // Jabber disable
              |  ETH_MACCR_IFG_96Bit
              | !ETH_MACCR_CSD  // carrier sense disable
              |  speed
              | !ETH_MACCR_ROD  // receive own disable = false
              | !ETH_MACCR_LM   // loop back mode = false
              |  mode
              | !ETH_MACCR_IPCO // hardware checksum = disabled
              |  ETH_MACCR_RD   // retry transmission disable = true
              | !ETH_MACCR_APCS
              |  ETH_MACCR_BL_10
              | !ETH_MACCR_DC );

  ETH->MACCR = tmpreg;

  //-----------------------------------------------------------------
  ETH->MACFFR = (   !ETH_MACFFR_RA
                  | !ETH_MACFFR_SAF    // source address filter = false
                  | !ETH_MACFFR_SAIF   // SA inverse filtering = false
                  |  ETH_MACFFR_PCF_BlockAll
                  | !ETH_MACFFR_BFD    // broadcast frame disable = false
                  | !ETH_MACFFR_DAIF
                  | !ETH_MACFFR_PM
                  | !ETH_MACFFR_HPF
                  | !ETH_MACFFR_PAM
                  | !ETH_MACFFR_HM     //perfect multicast filter
                  | !ETH_MACFFR_HPF
                  | !ETH_MACFFR_HU );  // perfect unicast filter

  // ETHERNET Hash table
  ETH->MACHTHR = 0; // ETH_HashTableHigh
  ETH->MACHTLR = 0; // ETH_HashTableLow

  //-----------------------------------------------------------------
  tmpreg  = ETH->MACFCR;

  tmpreg &= (  0xFF00               // Reserved, must be kept at reset value.
             | ETH_MACFCR_RFCE      // Keep RFCE
             | ETH_MACFCR_FCBBPA ); // Keep FCBBPA

  tmpreg |= (    (0  << 16)            // ETH_PauseTime
              |  ETH_MACFCR_ZQPD       // Zero quanta pause disable = true
              |  ETH_MACFCR_PLT_Minus4 // ETH_PauseLowThreshold_Minus4;
              | !ETH_MACFCR_UPFD
              | !ETH_MACFCR_RFCE
              | !ETH_MACFCR_TFCE );

  ETH->MACFCR = tmpreg;

  //-----------------------------------------------------------------
  ETH->MACVLANTR = (   !ETH_MACVLANTR_VLANTC  // VLAN 12bit = false
                     |  0 );                  //ETH_VLANTagIdentifier

  //-----------------------------------------------------------------
  // DMA Config
  //-----------------------------------------------------------------
  tmpreg  = ETH->DMAOMR;

  tmpreg &= ~(   ETH_DMAOMR_DTCEFD  // Clear
               | ETH_DMAOMR_RSF
               | ETH_DMAOMR_DFRF
               | ETH_DMAOMR_TSF
               | ETH_DMAOMR_TTC
               | ETH_DMAOMR_FEF
               | ETH_DMAOMR_FUGF
               | ETH_DMAOMR_RTC
               | ETH_DMAOMR_OSF );

  tmpreg |= (   !ETH_DMAOMR_DTCEFD   // droping TCP/IP checksum error frames disable = false
              |  ETH_DMAOMR_RSF
              | !ETH_DMAOMR_DFRF     // flushing of received frames disable = fals
              |  ETH_DMAOMR_TSF
              |  ETH_DMAOMR_TTC_64Bytes
              | !ETH_DMAOMR_FEF
              | !ETH_DMAOMR_FUGF
              |  ETH_DMAOMR_RTC_64Bytes
              |  ETH_DMAOMR_OSF );

  ETH->DMAOMR = tmpreg;

  //-----------------------------------------------------------------
  ETH->DMABMR = (    ETH_DMABMR_AAB
                  |  ETH_DMABMR_FB
                  |  ETH_DMABMR_RDP_32Beat  // if 4xPBL is selected for Tx or Rx it is applied for the other
                  |  ETH_DMABMR_PBL_32Beat
                  |  (0 << 2)
                  |  ETH_DMABMR_RTPR_1_1    // Rx Tx priority ratio = 1:1
                  | !ETH_DMABMR_DA          // DMA arbitration scheme, RxPriorTx = false
                  |  ETH_DMABMR_USP );      // Enable use of separate PBL for Rx and Tx


  MACAddressConfig( addrPhy ); // initialize MAC address in ethernet MAC

  DMATxDescChainInit(); // Initialize Tx Descriptors list: Chain Mode
  DMARxDescChainInit(); // Initialize Rx Descriptors list: Chain Mode

  Start();/* Enable MAC and DMA transmission and reception */

  return true; // Return Ethernet configuration success
}

//-------------------------------------------------------------------
bool Ethernet_Mcu::Init()
{
  if( isAutoNegotiation )
  {
    // We wait for linked status...
    //    for( uint32_t timeout = 0; !(ReadPHYRegister(PhyAddr, PHY_BSR) & PHY_Linked_Status); timeout++ )
    //    {
    //      if( timeout >= TIMEOUT )
    //      {
    //        return false;  // Return ERROR in case of timeout
    //      }
    //    }

    // Enable Auto-Negotiation
    if( !WritePHYRegister(PhyAddr, PHY_BCR, PHY_AutoNegotiation) )
    {
      return false; // Return ERROR in case of write timeout
    }

    // Wait until the auto-negotiation will be completed
    for( uint32_t timeout = 0; !(ReadPHYRegister(PhyAddr, PHY_BSR) & PHY_AutoNego_Complete); timeout++ )
    {
      if( timeout >= TIMEOUT )
      {
        return false;
      }
    }
  
    uint16_t r = ReadPHYRegister(PhyAddr, PHY_SR) ;
    switch( r& PHY_DUPLEX_SPEED_STATUS_MASK )  //Read the result of the auto-negotiation
    {
      case PHY_100BTX_FULL:
        mode  = ETH_MACCR_DM; // full duplex mode
        speed = ETH_MACCR_FES; // fast mode (100M) = true
        break;

      case PHY_100BTX_HALF:
        mode  = !ETH_MACCR_DM; // full duplex mode = false
        speed = ETH_MACCR_FES; // fast mode (100M) = true
        break;

      case PHY_10M_FULL:
        mode  = ETH_MACCR_DM; // full duplex mode
        speed = !ETH_MACCR_FES;// fast mode (100M) = false
        break;

      case PHY_10M_HALF:
        mode  = !ETH_MACCR_DM; // full duplex mode = false
        speed = !ETH_MACCR_FES;// fast mode (100M) = false
        break;

      default:
        break;
    }
  }
  else
  {
    // \todo magic bit shifting, sped and mode bit positions differ in PHY BCR and ETH MACCR
  /*
     ETH MACCR    PHY BCR   both
  speed  bit 14       bit 13    0=10Mbps, 1=100Mbps
  mode   bit 11       bit 8     0=half, 1=full duplex
  */

    if( !WritePHYRegister( PhyAddr, PHY_BCR, (mode>>3)|(speed>>1) ) )
    {
      return false;  // Return ERROR in case of write timeout
    }
    System::delayMilliSec(100); // Delay to assure PHY configuration
  }
  return( true );
}

//-------------------------------------------------------------------
void Ethernet_Mcu::Start(void)
{
  ETH->MACCR  |= ETH_MACCR_TE;    // Enable transmit state machine of the MAC for transmission on the MII
  ETH->DMAOMR |= ETH_DMAOMR_FTF;  // Set the Flush Transmit FIFO bit
  ETH->MACCR  |= ETH_MACCR_RE;    // Enable receive state machine of the MAC for reception from the MII
  ETH->DMAOMR |= ETH_DMAOMR_ST;   // Enable the DMA transmission
  ETH->DMAOMR |= ETH_DMAOMR_SR;   // Enable the DMA reception
}

//-------------------------------------------------------------------
void Ethernet_Mcu::MACAddressConfig( const NetAddr<6> &Addr )
{
  ETH->MACA0HR =   ((uint32_t)Addr.addr[5] <<  8)   // MAC address high register
                 | ((uint32_t)Addr.addr[4]      );

  ETH->MACA0LR =   ((uint32_t)Addr.addr[3] << 24)   // MAC address low register
                 | ((uint32_t)Addr.addr[2] << 16)
                 | ((uint32_t)Addr.addr[1] <<  8)
                 | ((uint32_t)Addr.addr[0]      );
}

//-------------------------------------------------------------------
uint16_t Ethernet_Mcu::ReadPHYRegister( uint16_t PHY_Id,
                                        uint16_t PHYReg )
{
  uint32_t tmpreg = ETH->MACMIIAR;  // Get the ETHERNET MACMIIAR value

  tmpreg &= ETH_MACMIIAR_CR_Msk;    // Keep only the CSR Clock Range CR[2:0] bits value

  // Prepare the MII address register value
  tmpreg |=  (((uint32_t)PHY_Id<<11) & ETH_MACMIIAR_PA);  // Set the PHY device address
  tmpreg |=  (((uint32_t)PHYReg<< 6) & ETH_MACMIIAR_MR);  // Set the PHY register address
  tmpreg &= ~ETH_MACMIIAR_MW;                             // Set the read mode
  tmpreg |=  ETH_MACMIIAR_MB;                             // Set the MII Busy bit

  ETH->MACMIIAR = tmpreg;  // MII Address register

  // Check for the Busy flag
  for( uint32_t timeout = 0; timeout < TIMEOUT; timeout++ )
  {
    if( !(ETH->MACMIIAR & ETH_MACMIIAR_MB) )
    {
      return( ETH->MACMIIDR );  // MII data register
    }
  }
  return( 0 );
}

//-------------------------------------------------------------------
bool Ethernet_Mcu::WritePHYRegister( uint16_t PHY_Id,
                                     uint16_t PHYReg,
                                     uint16_t PHYValue )
{
  uint32_t tmpreg = ETH->MACMIIAR;  // Get the ETHERNET MACMIIAR value

  tmpreg &= ETH_MACMIIAR_CR_Msk;    // Keep only the CSR Clock Range CR[2:0] bits value

  // Prepare the MII register address value
  tmpreg |= (((uint32_t)PHY_Id<<11) & ETH_MACMIIAR_PA);  // Set the PHY device address
  tmpreg |= (((uint32_t)PHYReg<< 6) & ETH_MACMIIAR_MR);  // Set the PHY register address
  tmpreg |= ETH_MACMIIAR_MW;                             // Set the write mode
  tmpreg |= ETH_MACMIIAR_MB;                             // Set the MII Busy bit

  ETH->MACMIIDR = PHYValue;  // MII data register
  ETH->MACMIIAR = tmpreg;    // MII Address register

  // Check for the Busy flag
  for( uint32_t timeout = 0; timeout < TIMEOUT; timeout++ )
  {
    if( !(ETH->MACMIIAR & ETH_MACMIIAR_MB) )
    {
      return( true );
    }
  }
  return( false );
}

}  } //namespace

//EOF
