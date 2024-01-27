//*******************************************************************
/*!
\file   config.h
\author Thomas Breuer
\date   23.02.2023
\brief  Board specific configuration
*/

//*******************************************************************
/*
Board:    ...

\see /Example/Src/Board/.../board_pinout.txt
*/

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//
// SETUP:
// ======
//
/// Select:
///-----------------------------------
#define USB_DEVICE 'D'
#define UART       'U'

#define ISC_INTERFACE  UART

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//*******************************************************************
using namespace EmbSysLib::Hw;
using namespace EmbSysLib::Dev;
using namespace EmbSysLib::Ctrl;
using namespace EmbSysLib::Mod;

//-------------------------------------------------------------------
// Port
//-------------------------------------------------------------------
Port_Mcu   portB( Port_Mcu::PortB );
Port_Mcu   portD( Port_Mcu::PortD );
Port_Mcu   portE( Port_Mcu::PortE );

//-------------------------------------------------------------------
// Timer
//-------------------------------------------------------------------
Timer_4  timer( 100L/*us*/ );

TaskManager taskManager( timer );

//-------------------------------------------------------------------
// UART
//-------------------------------------------------------------------
Uart_1  uart( 9600, 100, 100 ); 

Terminal   terminal( uart, 64,64,"erw" );

//-------------------------------------------------------------------
// USB
//-------------------------------------------------------------------
#ifdef USE_USB
  USBdeviceDescriptor_0  desc;            // Project related descriptor
  USBdevice_Mcu          usb( desc );
#endif

//-------------------------------------------------------------------
// Digital
//-------------------------------------------------------------------
Digital    led_A   ( portD, 5, Digital::Out, 1 ); // TX (yellow)
Digital    btn_A   ( portE, 6, Digital::In,  1 ); // B1 (user) - (Arduino Port D7)

//-------------------------------------------------------------------
// Control
//-------------------------------------------------------------------
DigitalIndicator indicator( led_A, taskManager );
DigitalButton    button   ( btn_A, taskManager, 40, 1000 );

//-------------------------------------------------------------------
// Modul/Isc
//-------------------------------------------------------------------
#if ISC_INTERFACE  == UART
  const bool enableTerminal = false;
  Uart   &uartIsc = uart;
#elif ISC_INTERFACE  == USB_DEVICE
  const bool enableTerminal = true;
#endif
