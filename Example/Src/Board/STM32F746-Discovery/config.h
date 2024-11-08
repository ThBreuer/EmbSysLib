//*******************************************************************
/*!
\file   config.h
\author Thomas Breuer
\date   04.11.2024
\brief  Board specific configuration
*/

//*******************************************************************
/*
Board:    STM32F746-Discovery

\see Example\Src\Board\STM32F746-Discovery/board_pinout.txt
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
#include "Hardware/Peripheral/Display/DisplayGraphic_GRAM.cpp"
#include "Hardware/Peripheral/Touch/Touch_FT5336.cpp"

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
  USART1_TX_PA9,
  USART1_RX_PB7,

  // I2C
  I2C3_SCL_PH7,
  I2C3_SDA_PH8,

  // USB
  USB_OTG_FS_DM_PA11,
  USB_OTG_FS_DP_PA12,

  // FMC
  FMC_D0_PD14,
  FMC_D1_PD15,
  FMC_D2_PD0,
  FMC_D3_PD1,
  FMC_D4_PE7,
  FMC_D5_PE8,
  FMC_D6_PE9,
  FMC_D7_PE10,
  FMC_D8_PE11,
  FMC_D9_PE12,
  FMC_D10_PE13,
  FMC_D11_PE14,
  FMC_D12_PE15,
  FMC_D13_PD8,
  FMC_D14_PD9,
  FMC_D15_PD10,

  FMC_A0_PF0,
  FMC_A1_PF1,
  FMC_A2_PF2,
  FMC_A3_PF3,
  FMC_A4_PF4,
  FMC_A5_PF5,
  FMC_A6_PF12,
  FMC_A7_PF13,
  FMC_A8_PF14,
  FMC_A9_PF15,
  FMC_A10_PG0,
  FMC_A11_PG1,

  FMC_SDCLK_PG8,
  FMC_SDNCAS_PG15,
  FMC_SDCKE0_PC3,
  FMC_SDNE0_PH3,
  FMC_SDNWE_PH5,
  FMC_SDNRAS_PF11,
  FMC_NBL0_PE0,
  FMC_NBL1_PE1,

  FMC_BA0_PG4,
  FMC_BA1_PG5,

  LTDC_R0_PI15,
  LTDC_R1_PJ0,
  LTDC_R2_PJ1,
  LTDC_R3_PJ2,
  LTDC_R4_PJ3,
  LTDC_R5_PJ4,
  LTDC_R6_PJ5,
  LTDC_R7_PJ6,
  LTDC_G0_PJ7,
  LTDC_G1_PJ8,
  LTDC_G2_PJ9,
  LTDC_G3_PJ10,
  LTDC_G4_PJ11,
  LTDC_G5_PK0,
  LTDC_G6_PK1,
  LTDC_G7_PK2,
  LTDC_B0_PE4,
  LTDC_B1_PJ13,
  LTDC_B2_PJ14,
  LTDC_B3_PJ15,
  LTDC_B4_PG12,
  LTDC_B5_PK4,
  LTDC_B6_PK5,
  LTDC_B7_PK6,

  LTDC_CLK_PI14,
  LTDC_DE_PK7,
  LTDC_HSYNC_PI10,
  LTDC_VSYNC_PI9,

  END_OF_TABLE
};

//-------------------------------------------------------------------
// Port
//-------------------------------------------------------------------
Port_Mcu   portA( Port_Mcu::PA );
Port_Mcu   portC( Port_Mcu::PC );
Port_Mcu   portF( Port_Mcu::PF );
Port_Mcu   portJ( Port_Mcu::PJ );
Port_Mcu   portI( Port_Mcu::PI );


//-------------------------------------------------------------------
// Timer
//-------------------------------------------------------------------
Timer_Mcu  timer( Timer_Mcu::TIM_11, 100L/*us*/ );

TaskManager taskManager( timer );

//-------------------------------------------------------------------
// I2C
//-------------------------------------------------------------------
I2Cmaster_Mcu i2cBusTouch( I2Cmaster_Mcu::I2C_3, 100/*kHz*/ );

//-------------------------------------------------------------------
// UART
//-------------------------------------------------------------------
Uart_Mcu   uart ( Uart_Mcu::USART_1, 9600, Uart_Mcu::NONE, 100, 100 );

Terminal   terminal( uart, 255,255,"# +" );

//-------------------------------------------------------------------
// Display
//-------------------------------------------------------------------
Memory_Flash image( 7 ); // Size: 256k
Font    fontFont_10x20      ( MemoryImage( image, "Font_10x20"     ).getPtr() );
Font    fontFont_16x24      ( MemoryImage( image, "Font_16x24"     ).getPtr() );
Font    fontFont_8x12       ( MemoryImage( image, "Font_8x12"      ).getPtr() );
Font    fontFont_8x8        ( MemoryImage( image, "Font_8x8"       ).getPtr() );
Bitmap  bitmapBitmap_320x240( MemoryImage( image, "Bitmap_320x240" ).getPtr() );
Bitmap  bitmapBitmap_32x32  ( MemoryImage( image, "Bitmap_32x32"   ).getPtr() );

Digital lcdDisp( portI, 12, Digital::Out, 1 );

Fmc_Mcu       fmc        ( Fmc_Mcu::SDRAM_Both, Fmc_Mcu::BusConfig_type::DATA_BUS_WIDTH_16BIT );


DisplayGraphic_GRAM dispGraphic( (WORD*)fmc.startAddr(),
                                 480,272,
                                 fontFont_10x20,
                                 1 );

Ltdc_Mcu ltdc( fmc.startAddr(), true, dispGraphic.getWidth(), dispGraphic.getHeight() );

ScreenChar    screenChar   ( dispGraphic );
ScreenGraphic screenGraphic( dispGraphic );

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
Touch_FT5336 touch( i2cBusTouch, 480, 272, Touch::ROTATION_0 );

Pointer        pointer( touch );

//-------------------------------------------------------------------
// Digital
//-------------------------------------------------------------------
Digital    led_A( portI, 1, Digital::Out, 0 ); // LD1 (green)
Digital    btn_A( portI,11, Digital::In,  0 ); // B1 (user button)

Digital    btnLeft ( portF, 8, Digital::InPU  , 1 );
Digital    btnCtrl ( portF, 9, Digital::InPU  , 1 );
Digital    btnRight( portA, 6, Digital::InPU  , 1 );

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

