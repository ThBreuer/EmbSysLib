//*******************************************************************
/*!
\file   config.h
\author Thomas Breuer
\date   23.02.2023
\brief  Board specific configuration
*/

//*******************************************************************
/*
Board:    STM32F4-Discovery + Base-Board (STM32F4DIS-BB) + LCD-Board (STM32F4DIS-LCD)

\see STM32F4-Discovery/board_pinout.txt
*/

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//
// SETUP:
// ======

/// Select:
///-----------------------------------
#define USB_DEVICE 'D'
#define UART       'U'

#define ISC_INTERFACE  UART

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//*******************************************************************
#include "Hardware/Peripheral/Display/DisplayGraphic_SSD2119.cpp"
#include "Hardware/Peripheral/Touch/Touch_STMPE811i2c.cpp"

//*******************************************************************
#include "../../Resource/Color/Color.h"

//*******************************************************************
using namespace EmbSysLib::Hw;
using namespace EmbSysLib::Dev;
using namespace EmbSysLib::Ctrl;
using namespace EmbSysLib::Mod;

//*******************************************************************
PinConfig::MAP PinConfig::table[] =
{
  // UART
  USART6_TX_PC6,
  USART6_RX_PC7,

  // I2C
  I2C1_SCL_PB8,
  I2C1_SDA_PB9,

  // USB
  OTG_FS_DM_PA11,
  OTG_FS_DP_PA12,

  // Display
  FSMC_A19_PE3,
  FSMC_D0_PD14,
  FSMC_D1_PD15,
  FSMC_D2_PD0,
  FSMC_D3_PD1,
  FSMC_D4_PE7,
  FSMC_D5_PE8,
  FSMC_D6_PE9,
  FSMC_D7_PE10,
  FSMC_D8_PE11,
  FSMC_D9_PE12,
  FSMC_D10_PE13,
  FSMC_D11_PE14,
  FSMC_D12_PE15,
  FSMC_D13_PD8,
  FSMC_D14_PD9,
  FSMC_D15_PD10,
  FSMC_NOE_PD4,
  FSMC_NWE_PD5,
  FSMC_NCE2_PD7,

  END_OF_TABLE
};

//-------------------------------------------------------------------
// Port
//-------------------------------------------------------------------
Port_Mcu   portA( Port_Mcu::PA );
Port_Mcu   portD( Port_Mcu::PD );

//-------------------------------------------------------------------
// Timer
//-------------------------------------------------------------------
Timer_Mcu  timer( Timer_Mcu::TIM_2, 100L/*us*/ );

TaskManager taskManager( timer );

//-------------------------------------------------------------------
// I2C
//-------------------------------------------------------------------
I2Cmaster_Mcu i2cBus( I2Cmaster_Mcu::I2C_1, 100/*kHz*/ );

//-------------------------------------------------------------------
// Display
//-------------------------------------------------------------------
  Memory_Flash image( 8,   // first sector @0x08080000
                      9 ); // last sector
                           // total size: 256k

  Font    fontFont_10x20      ( MemoryImage( image, "Font_10x20"     ).getPtr() );
  Font    fontFont_16x24      ( MemoryImage( image, "Font_16x24"     ).getPtr() );
  Font    fontFont_8x12       ( MemoryImage( image, "Font_8x12"      ).getPtr() );
  Font    fontFont_8x8        ( MemoryImage( image, "Font_8x8"       ).getPtr() );
  Bitmap  bitmapBitmap_320x240( MemoryImage( image, "Bitmap_320x240" ).getPtr() );
  Bitmap  bitmapBitmap_32x32  ( MemoryImage( image, "Bitmap_32x32"   ).getPtr() );


Port::Pin     pinBackLight ( portD, 13 );
Port::Pin     pinReset     ( portD,  3 );
Fsmc_Mcu      fsmc         ( 0 );

DisplayGraphic_SSD2119  dispGraphic( fsmc.getPtr(),
                                     pinReset,
                                     pinBackLight ,
                                     fontFont_8x12, // font
                                     1 );           // actual zoom factor

DisplayChar &disp = dispGraphic; // reuse as character display if needed

ScreenChar    screenChar   ( dispGraphic );
ScreenGraphic screenGraphic( dispGraphic );

//-------------------------------------------------------------------
// UART
//-------------------------------------------------------------------
Uart_Mcu   uart ( Uart_Mcu::USART_6, 9600, Uart_Mcu::NONE, 100, 100);

Terminal   terminal( uart, 255,255,"# +" );

//-------------------------------------------------------------------
// USB
//-------------------------------------------------------------------
#ifdef USE_USB
  USBdeviceDescriptor_0  desc;            // Project related descriptor
  USBdevice_Mcu          usb( desc );
#endif

//-------------------------------------------------------------------
// Touch
//-------------------------------------------------------------------
Touch_STMPE811i2c touch( i2cBus, 0, 320, 240, Touch::ROTATION_0);

Pointer        pointer( touch );

//-------------------------------------------------------------------
// Digital
//-------------------------------------------------------------------
Digital    led_A( portD, 12, Digital::Out, 0 ); // LD4 (green)
Digital    btn_A( portA,  0, Digital::In,  0 ); // B1  (USER)

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
