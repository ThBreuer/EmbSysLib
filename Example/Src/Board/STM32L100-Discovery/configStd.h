//*******************************************************************
/*!
\file   configStd.h
\author Thomas Breuer
\date   26.09.2022
\brief  Board specific configuration
*/

//*******************************************************************
/*
Board:    STM32L100-Discovery

UART:     Connect UART to COM-Port and start a terminal (e.g. Putty)
          Configuration: 9600 baud,8 bit,1 stop bit, no parity, now flow control
                         line editing: force off
            TxD - PA9
            RxD - PA10
*/

//*******************************************************************
using namespace EmbSysLib::Hw;
using namespace EmbSysLib::Std;

//*******************************************************************
PinConfig::MAP PinConfig::table[] =
{
  // UART
  USART1_TX_PA9,
  USART1_RX_PA10,
  
  END_OF_TABLE
};

//-------------------------------------------------------------------
// Timer
//-------------------------------------------------------------------
Timer_Mcu  timer( Timer_Mcu::TIM_11, 100L/*us*/ );

//-------------------------------------------------------------------
// UART
//-------------------------------------------------------------------
Uart_Mcu   uart ( Uart_Mcu::USART_1, 9600, 100, 100 );
