//*******************************************************************
/*!
\file   Ethernet_Mcu.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   18.03.2024

License: See file "LICENSE"

This implementation is a redesign of
 "stm32f4x7_eth.h" and "stm32f4x7_eth.c"
 MCD Application Team, V1.0.0, 14-October-2011, COPYRIGHT 2011 STMicroelectronics
modified by
 CMP Team, V1.0.0, 28-December-2012, COPYRIGHT 2012 Embest Tech. Co., Ltd.

\todo This implementation is using LAN8742A. It must be redesigned to make it independent of PHY.
*/

//*******************************************************************
#ifndef _HW_ETHERNET_MCU_H
#define _HW_ETHERNET_MCU_H

//*******************************************************************
#include "Hardware/Common/Ethernet/Ethernet.h"
#include "Hardware/Common/Port/Port.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class Ethernet_Mcu
\brief ...
*/
class Ethernet_Mcu : public Ethernet
{
  private:
    //---------------------------------------------------------------
    // \todo see Ethernet.h, BUFFER_SIZE
    static const uint16_t ETH_MAX_PACKET_SIZE = 1524;   // ETH_HEADER + ETH_EXTRA + VLAN_TAG + MAX_ETH_PAYLOAD + ETH_CRC
    static const uint16_t ETH_RXBUFNB         =    5;
    static const uint16_t ETH_TXBUFNB         =    5;

  private:
    //***************************************************************
    /*
    \class DMA_Descriptor
    \brief ETH DMA Descriptor data structure definition
    */
    class DMA_Descriptor
    {
      public:
        __IO uint32_t        Status;               //!< Status
        __IO uint32_t        ControlBufferSize;    //!< Control and Buffer1, Buffer2 lengths
        __IO uint8_t        *Buffer1Addr;          //!< Buffer1 address pointer
        __IO DMA_Descriptor *Buffer2NextDescAddr;  //!< Buffer2 or next descriptor address pointer
    };
    //***************************************************************

    //***************************************************************
    /*
    \class DMA_Frame
    */
    class DMA_Frame
    {
      public:
        __IO uint32_t        length;
        __IO uint8_t        *buffer;
        __IO DMA_Descriptor *descriptor;
    };
    //***************************************************************

    //***************************************************************
    /*
    \class DMA_RxFrameInfos
    */
    class DMA_RxFrameInfos
    {
      public:
        __IO DMA_Descriptor *FS_Rx_Desc;  //!< First Segment Rx Desc
        __IO DMA_Descriptor *LS_Rx_Desc;  //!< Last Segment Rx Desc
        __IO uint32_t        Seg_Count;   //!< Segment count
    };
    //***************************************************************

    //***************************************************************
    /*
    \class DMA_Buffer
    */
    class DMA_Buffer
    {
      public:
        Ethernet_Mcu::DMA_Descriptor  RxDscrTab[ETH_RXBUFNB];                      // Ethernet Rx DMA Descriptor
        Ethernet_Mcu::DMA_Descriptor  TxDscrTab[ETH_TXBUFNB];                      // Ethernet Tx DMA Descriptor
        uint8_t                       RxBuff   [ETH_RXBUFNB][ETH_MAX_PACKET_SIZE]; // Ethernet Receive Buffer
        uint8_t                       TxBuff   [ETH_TXBUFNB][ETH_MAX_PACKET_SIZE]; // Ethernet Transmit Buffer
    };
    //***************************************************************

  public:
    //---------------------------------------------------------------
    /*! Instantiate an Ethernet_Mcu object
        \param addrPhy MAC address
        \param timer Reference to a timer
    */
    Ethernet_Mcu( const NetAddr<6> &addrPhy, Timer &timer );

  private:
    //---------------------------------------------------------------
    virtual void create( const NetAddr<6> destAddr, const WORD type );

    //---------------------------------------------------------------
    virtual void PacketSend( uint8_t* packet, uint16_t len );

    //---------------------------------------------------------------
    virtual bool PacketReceive( void );

    //---------------------------------------------------------------
    virtual WORD getType( void );

  public:
    //---------------------------------------------------------------
    virtual bool isLinked( void );

  private:
    //---------------------------------------------------------------
    // Global ETH MAC/DMA functions
    //---------------------------------------------------------------
    //---------------------------------------------------------------
    /* Initializes the ETHERNET peripheral
       \param PHYAddress: external PHY address
       \return false: Ethernet initialization failed\n
               true:  Ethernet successfully initialized
    */
    bool InitEth( void );

  public:
    virtual bool Init( void );

  private:
    //---------------------------------------------------------------
    /* Enables ENET MAC and DMA reception/transmission
    */
    void Start( void );

    //---------------------------------------------------------------
    /* Configures the MAC address.
       Write to MAC Address register 0 only.
       \param  Addr MAC address
    */
    void MACAddressConfig( const NetAddr<6> &Addr );

    //---------------------------------------------------------------
    // DMA Descriptors functions
    //---------------------------------------------------------------
    //---------------------------------------------------------------
    /* This function should be called to get the received frame (to be used
       with polling method only).
       \return Structure of type DMA_Frame
      */
    DMA_Frame Get_Received_Frame( void );

    //---------------------------------------------------------------
    /* This function polls for a frame reception
       \return true when a frame is received, false if none.
      */
    bool CheckFrameReceived(void);

    //---------------------------------------------------------------
    /* Prepares DMA Tx descriptors to transmit an ethernet frame
       \param frameLength Length of the frame to send
       \return true if ok, false on error
      */
    bool Prepare_Transmit_Descriptors( uint16_t frameLength );

    //---------------------------------------------------------------
    /* Initializes the DMA Rx descriptors in chain mode.
    */
    void DMARxDescChainInit( void );

    //---------------------------------------------------------------
    /* Initializes the DMA Tx descriptors in chain mode.
    */
    void DMATxDescChainInit( void );

    //---------------------------------------------------------------
    // PHY functions
    //---------------------------------------------------------------
    //---------------------------------------------------------------
    /* Read a PHY register
       \param PHY_Id Index of one of supported 32 PHY devices.
                     This parameter can be one of the following values: 0,..,31
       \param PHYReg This parameter can be one of the following values:
                        PHY_BCR: Transceiver Basic Control Register
                        PHY_BSR: Transceiver Basic Status Register
                        PHY_SR : Transceiver Status Register
                        More PHY register could be read depending on the used PHY
        \return MAC MIIDR register value: Data read from the selected PHY register (correct read )
                or 0 if timeout
    */
    uint16_t ReadPHYRegister( uint16_t PHY_Id, uint16_t PHYReg );

    //---------------------------------------------------------------
    /* Write to a PHY register
       \param PHY_Id Index of one of supported 32 PHY devices.
                     This parameter can be one of the following values: 0,..,31
       \param PHYReg This parameter can be one of the following values:
                        PHY_BCR    : Transceiver Control Register
                        More PHY register could be written depending on the used PHY
       \param PHYValue The value to write
       \return true for correct write
               false in case of timeout
    */
    bool WritePHYRegister( uint16_t PHY_Id, uint16_t PHYReg, uint16_t PHYValue );

  private:
    //---------------------------------------------------------------
    BYTE     PhyAddr;
    bool     isAutoNegotiation;
    uint32_t speed;
    uint32_t mode;

    //---------------------------------------------------------------
    Std::Flag<LinkState>  linkState; //!< Connection state



    //---------------------------------------------------------------
    /* Global pointers on Tx and Rx descriptor used to track transmit and receive descriptors */
    __IO DMA_Descriptor  *TxDescToSet;
    __IO DMA_Descriptor  *RxDescToGet;

    /* Structure used to hold the last received packet descriptors info */
    __IO DMA_RxFrameInfos  rxFrame;

    static DMA_Buffer dmaBuffer;

    static const DWORD TIMEOUT = 0x0004FFFF;

}; //class Ethernet_Mcu

}  } //namespace

#endif
