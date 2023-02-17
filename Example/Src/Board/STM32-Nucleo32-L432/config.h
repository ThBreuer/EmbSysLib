//*******************************************************************
/*!
\file   config.h
\author Thomas Breuer
\date   15.10.2022
\brief  Board specific configuration
*/

//*******************************************************************
/*
Board:    STM32-Nucleo32-L432

LED:      LD3 (green) - PB3

UART:     Virtual COM port, available via ST-Link/USB
          Connect USB and start a terminal (e.g. Putty)
          Configuration: 9600 baud,8 bit,1 stop bit, no parity, now flow control
                         line editing: force off
*/

//*******************************************************************
using namespace EmbSysLib::Hw;
using namespace EmbSysLib::Dev;

//*******************************************************************
PinConfig::MAP PinConfig::table[] =
{
  // UART
  USART2_TX_PA2,
  USART2_RX_PA15,

  END_OF_TABLE
};

//-------------------------------------------------------------------
// Port
//-------------------------------------------------------------------
Port_Mcu   portB( Port_Mcu::PB );

Digital    led_A( portB, 3, Digital::Out,  0 ); // LD3 (green)

//-------------------------------------------------------------------
// Timer
//-------------------------------------------------------------------
Timer_Mcu  timer( Timer_Mcu::TIM_2, 100L/*us*/ );

//-------------------------------------------------------------------
// UART
//-------------------------------------------------------------------
Uart_Mcu   uart ( Uart_Mcu::USART_2, 9600, 100, 100 );
