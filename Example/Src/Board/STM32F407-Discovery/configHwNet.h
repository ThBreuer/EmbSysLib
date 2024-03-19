//*******************************************************************
/*!
\file   configHw.h
\author Thomas Breuer
\date   18.03.2024
\brief  Board specific configuration
*/

//*******************************************************************
/*
Board:    STM32F4-Discovery + Base-Board (STM32F4DIS-BB) + LCD-Board (STM32F4DIS-LCD)

\see STM32F407-Discovery/board_pinout.txt
*/

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//
// SETUP:
// ======

#define MAC  {0x16,0x20,0x68,0x01,0x08,0x30}
#define IP   {192,168,178,22}
#define PORT 2999

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//*******************************************************************
using namespace EmbSysLib::Hw;

//*******************************************************************
PinConfig::MAP PinConfig::table[] =
{
  // UART
  USART6_TX_PC6,
  USART6_RX_PC7,

  // ETH
  ETH_MII_TX_EN_PB11,
  ETH_MII_TXD0_PB12,
  ETH_MII_TXD1_PB13,
  ETH_MII_RXD0_PC4,
  ETH_MII_RXD1_PC5,
  ETH_MII_RX_ER_PB10,
  ETH_MII_RX_CLK_PA1,
  ETH_MII_RX_DV_PA7,
  ETH_MDC_PC1,
  ETH_MDIO_PA2,

  END_OF_TABLE
};

//-------------------------------------------------------------------
// Port
//-------------------------------------------------------------------
Port_Mcu   portA( Port_Mcu::PA );
Port_Mcu   portB( Port_Mcu::PB );
Port_Mcu   portD( Port_Mcu::PD );
Port_Mcu   portE( Port_Mcu::PE );

//-------------------------------------------------------------------
// Timer
//-------------------------------------------------------------------
Timer_Mcu timerSlow( Timer_Mcu::TIM_3,  10000L/*us*/ );

//-------------------------------------------------------------------
// UART
//-------------------------------------------------------------------
Uart_Mcu   uart ( Uart_Mcu::USART_6,
                  9600,              // Baud rate
                  Uart_Mcu::NONE,    // no handshake, no parity
                  100, 100 );        // RX and TX buffer size

//-------------------------------------------------------------------
// Ethernet
//-------------------------------------------------------------------
NetAddr<6> myMAC = {MAC};
NetAddr<4> addr  = {IP};

typedef NetEthTCP TCP_type;
typedef NetEthUDP UDP_type;

//Port::Pin  resetPin( portE,  2, Port::Out  );
Ethernet_Mcu net  ( myMAC, timerSlow );
NetEthIP     ip   ( net /*, myIP*/ );
NetEthICMP   icmp ( ip );
NetEthTCP    tcp  ( net, ip );
NetEthUDP    udp  ( net, ip );
NetEthDHCP   dhcp ( udp, "EmbSysLib" );
