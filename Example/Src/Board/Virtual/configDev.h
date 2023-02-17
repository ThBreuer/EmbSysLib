//*******************************************************************
/*!
\file   configDev.h
\author Thomas Breuer
\date   26.09.2022
\brief  Board specific configuration
*/

//*******************************************************************
/*
Board:    Virtual

          Start the Virtual Device Server (see "Example/Project/_VirtualServer.bat")

          LED:      LED 0  - Port_Virtual, Bit 16

          Button:    "A"   - Port_Virtual, Bit 5

UART:     Terminal (stdio)
*/

//*******************************************************************
using namespace EmbSysLib::Hw;
using namespace EmbSysLib::Dev;

//-------------------------------------------------------------------
// Port
//-------------------------------------------------------------------
Port_Virtual  port( "localhost:1000" );

Digital       led_A( port,16, Digital::Out , 0 ); // LED 0
Digital       btn_A( port, 5, Digital::In  , 0 ); // Button "A"

//-------------------------------------------------------------------
// UART
//-------------------------------------------------------------------
Uart_Stdio  uart;
