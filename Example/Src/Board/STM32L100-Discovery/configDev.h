//*******************************************************************
/*!
\file   configDev.h
\author Thomas Breuer
\date   26.09.2022
\brief  Board specific configuration
*/

//*******************************************************************
/*
Board:    STM32L100-Discovery

LED:      LD3 (green) - PC9
          LD4 (blue)  - PC8

Button:   B1 (user)   - PA0 (with pull down resistor)

UART:     Connect UART to COM-Port and start a terminal (e.g. Putty)
          Configuration: 9600 baud,8 bit,1 stop bit, no parity, now flow control
                         line editing: force off
            TxD - PA9
            RxD - PA10
*/

//*******************************************************************
using namespace EmbSysLib::Hw;
using namespace EmbSysLib::Dev;

//*******************************************************************
PinConfig::MAP PinConfig::table[] =
{
  // UART
  USART1_TX_PA9,
  USART1_RX_PA10,
  
  END_OF_TABLE
};

//-------------------------------------------------------------------
// Port
//-------------------------------------------------------------------
Port_Mcu   portA( Port_Mcu::PA );
Port_Mcu   portC( Port_Mcu::PC );

Digital    led_A( portC, 8, Digital::Out, 0 ); // LD4 (blue)
Digital    btn_A( portA, 0, Digital::In,  0 ); // B1 (user button)

//-------------------------------------------------------------------
// UART
//-------------------------------------------------------------------
Uart_Mcu   uart ( Uart_Mcu::USART_1, 9600, 100, 100 );
