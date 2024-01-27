//*******************************************************************
/*!
\file   config.h
\author Thomas Breuer
\date   23.02.2023
\brief  Board specific configuration
*/

//*******************************************************************
/*
Board:    STM32L100-Discovery

\see STM32L100-Discovery/board_pinout.txt
*/

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//
// SETUP:
// ======

/// Select encoder type:
///---------------------
#define USE_ROTARY_KNOB  false

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

//*******************************************************************
PinConfig::MAP PinConfig::table[] =
{
  // UART
  USART1_TX_PA9,
  USART1_RX_PA10,

  // USB
  USB_DM_PA11,
  USB_DP_PA12,

  END_OF_TABLE
};

//-------------------------------------------------------------------
// Port
//-------------------------------------------------------------------
Port_Mcu   portA( Port_Mcu::PA );
Port_Mcu   portC( Port_Mcu::PC );

//-------------------------------------------------------------------
// Timer
//-------------------------------------------------------------------
Timer_Mcu  timer( Timer_Mcu::TIM_11, 100L/*us*/ );

TaskManager taskManager( timer );

//-------------------------------------------------------------------
// UART
//-------------------------------------------------------------------
Uart_Mcu   uart ( Uart_Mcu::USART_1, 9600, 100, 100 );

Terminal   terminal( uart, 255,255,"erw" );

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
Digital    led_A   ( portC, 8, Digital::Out, 0 ); // LD4 (blue)
Digital    btn_A   ( portA, 0, Digital::In,  0 ); // B1 (user button)

#if USE_ROTARY_KNOB == true
  Digital    rotA    ( portA, 1, Digital::InPU  , 1 );
  Digital    rotB    ( portA, 8, Digital::InPU  , 1 );
  Digital    rotCtrl ( portA,15, Digital::InPU  , 1 );
#else
  Digital    btnLeft ( portC, 1, Digital::InPU  , 1 );
  Digital    btnRight( portC, 7, Digital::InPU  , 1 );
  Digital    btnCtrl ( portC, 6, Digital::InPU  , 1 );
#endif

//-------------------------------------------------------------------
// Control
//-------------------------------------------------------------------
DigitalIndicator indicator( led_A, taskManager );
DigitalButton    button   ( btn_A, taskManager, 40, 1000 );

#if USE_ROTARY_KNOB == true
  DigitalEncoderRotaryknob  encoder( &rotA, &rotB, &rotCtrl, taskManager );
#else
  DigitalEncoderJoystick    encoder( &btnLeft, &btnRight, &btnCtrl, taskManager, 150 );
#endif

//-------------------------------------------------------------------
// Modul/Isc
//-------------------------------------------------------------------
#if ISC_INTERFACE  == UART
  const bool enableTerminal = false;
  Uart   &uartIsc = uart;
#elif ISC_INTERFACE  == USB_DEVICE
  const bool enableTerminal = true;
#endif
