//*******************************************************************
/*!
\file   configHw.h
\author Thomas Breuer
\date   18.03.2024
\brief  Board specific configuration
*/

//*******************************************************************
/*
Board:    STM32F769-Discovery

\see STM32F769-Discovery/board_pinout.txt
*/

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//
// SETUP:
// ======

#define MAC  {0x16,0x20,0x68,0x01,0x08,0x28}
#define IP   {192,168,178,22}
#define PORT 2999

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//*******************************************************************
using namespace EmbSysLib::Hw;

//*******************************************************************
PinConfig::MAP PinConfig::table[] =
{
  // UART
  USART1_TX_PA9,
  USART1_RX_PA10,

  // ETH
  ETH_TX_EN_PG11,
  ETH_TXD0_PG13,
  ETH_TXD1_PG14,
  ETH_RXD0_PC4,
  ETH_RXD1_PC5,
  //ETH_RX_ER_xxx,
  ETH_RX_CLK_PA1,
  ETH_RX_DV_PA7,
  ETH_MDC_PC1,
  ETH_MDIO_PA2,

  END_OF_TABLE
};

//-------------------------------------------------------------------
// Port
//-------------------------------------------------------------------
Port_Mcu   portE( Port_Mcu::PE );

//-------------------------------------------------------------------
// Timer
//-------------------------------------------------------------------
Timer_Mcu  timerSlow( Timer_Mcu::TIM_14, 10000L/*us*/ );

//-------------------------------------------------------------------
// UART
//-------------------------------------------------------------------
Uart_Mcu  uart( Uart_Mcu::USART_1,
                9600,
                0,
                100, 100 ); // RX and TX buffer size

//-------------------------------------------------------------------
// Ethernet
//-------------------------------------------------------------------
NetAddr<6> myMAC = {MAC};
NetAddr<4> addr  = {IP};

typedef NetEthTCP TCP_type;
typedef NetEthUDP UDP_type;

Ethernet_Mcu net( myMAC, timerSlow );
NetEthIP     ip ( net /*, myIP*/ );
NetEthICMP icmp ( ip );
NetEthTCP  tcp  ( net, ip );
NetEthUDP  udp  ( net, ip );
NetEthDHCP dhcp ( udp, "EmbSysLib" );
