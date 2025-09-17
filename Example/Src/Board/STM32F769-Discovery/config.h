//*******************************************************************
/*!
\file   config.h
\author Thomas Breuer
\date   23.02.2023
\brief  Board specific configuration
*/

//*******************************************************************
/*
Board:    STM32F769-Discovery

\see Example\Src\Board\STM32F769-Discovery/board_pinout.txt
*/

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//
// SETUP:
// ======

/// Select revision of LCD board MB1166:
#define A02	2  // OTM8009A + FT6206
#define A09	9  // NT35510  + FT6306

#define MB1166_REV  A02

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
#if MB1166_REV == A02
  #include "Hardware/Peripheral/Display/DisplayGraphic_OTM8009A.cpp"
#endif
#if MB1166_REV == A09
  #include "Hardware/Peripheral/Display/DisplayGraphic_NT35510.cpp"
#endif

#include "Hardware/Peripheral/Touch/Touch_FT6206.cpp"

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
  USART1_RX_PA10,
  
  // I2C
  I2C4_SCL_PD12,
  I2C4_SDA_PB7,

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
  FMC_D16_PH8,
  FMC_D17_PH9,
  FMC_D18_PH10,
  FMC_D19_PH11,
  FMC_D20_PH12,
  FMC_D21_PH13,
  FMC_D22_PH14,
  FMC_D23_PH15,
  FMC_D24_PI0,
  FMC_D25_PI1,
  FMC_D26_PI2,
  FMC_D27_PI3,
  FMC_D28_PI6,
  FMC_D29_PI7,
  FMC_D30_PI9,
  FMC_D31_PI10,

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
  FMC_A12_PG2,
  FMC_A13_PG3,
  FMC_A14_PG4,
  FMC_A15_PG5,

  FMC_SDCLK_PG8,
  FMC_SDNCAS_PG15,
  FMC_SDCKE0_PH2,
  FMC_SDNE0_PH3,
  FMC_SDNWE_PH5,
  FMC_SDNRAS_PF11,
  FMC_NBL0_PE0,
  FMC_NBL1_PE1, 
  FMC_NBL2_PI4,
  FMC_NBL3_PI5,
  
  END_OF_TABLE
};

//-------------------------------------------------------------------
// Port
//-------------------------------------------------------------------
Port_Mcu   portA( Port_Mcu::PA );
Port_Mcu   portC( Port_Mcu::PC );
Port_Mcu   portF( Port_Mcu::PF );
Port_Mcu   portJ( Port_Mcu::PJ );


//-------------------------------------------------------------------
// Timer
//-------------------------------------------------------------------
Timer_Mcu  timer( Timer_Mcu::TIM_11, 100L/*us*/ );

TaskManager taskManager( timer );

//-------------------------------------------------------------------
// I2C
//-------------------------------------------------------------------
I2Cmaster_Mcu i2cBusTouch( I2Cmaster_Mcu::I2C_4, 400/*kHz*/ );

//-------------------------------------------------------------------
// UART
//-------------------------------------------------------------------
Uart_Mcu   uart ( Uart_Mcu::USART_1, 9600, Uart_Mcu::NONE, 100, 100 );

Terminal   terminal( uart, 255,255,"# +" );

//-------------------------------------------------------------------
// Display
//-------------------------------------------------------------------
Memory_Flash image( 10 ); // Size: 256k
Font    fontFont_10x20      ( MemoryImage( image, "Font_10x20"     ).getPtr() );
Font    fontFont_16x24      ( MemoryImage( image, "Font_16x24"     ).getPtr() );
Font    fontFont_8x12       ( MemoryImage( image, "Font_8x12"      ).getPtr() );
Font    fontFont_8x8        ( MemoryImage( image, "Font_8x8"       ).getPtr() );
Bitmap  bitmapBitmap_320x240( MemoryImage( image, "Bitmap_320x240" ).getPtr() );
Bitmap  bitmapBitmap_32x32  ( MemoryImage( image, "Bitmap_32x32"   ).getPtr() );

Port::Pin     lcdResetPin( portJ, 15 );
Fmc_Mcu       fmc        ( Fmc_Mcu::SDRAM_Bank1, Fmc_Mcu::BusConfig_type::DATA_BUS_WIDTH_32BIT );  
Dsi_Mcu       hwDSI      ( fmc.startAddr() );

#if MB1166_REV == A02
	DisplayGraphic_OTM8009Aram dispGraphic( hwDSI,lcdResetPin,
											DisplayGraphic_OTM8009A::LANDSCAPE_90,
											fontFont_8x12,
											2 );
#endif

#if MB1166_REV == A09
  DisplayGraphic_NT35510ram dispGraphic( hwDSI,lcdResetPin,
                                         DisplayGraphic_NT35510::LANDSCAPE_90,
	                                     fontFont_8x12,
                                         2 );
#endif

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

#if MB1166_REV == A02
  Touch_FT6206 touch( i2cBusTouch, 480, 800, Touch::ROTATION_90 );
#endif

#if MB1166_REV == A09
  Touch_FT6206 touch( i2cBusTouch, 480, 800, Touch::ROTATION_90, Touch_FT6206::VARIANT_FT6306 );
#endif

Pointer        pointer( touch );

//-------------------------------------------------------------------
// Digital
//-------------------------------------------------------------------
Digital    led_A( portJ,13, Digital::Out, 0 ); // LD1 (red)
Digital    btn_A( portA, 0, Digital::In,  0 ); // B1 (user button)

Digital    btnLeft ( portF, 8, Digital::InPU  , 1 );
Digital    btnCtrl ( portF, 9, Digital::InPU  , 1 );
Digital    btnRight( portA, 6, Digital::InPU  , 1 );

//-------------------------------------------------------------------
// Control
//-------------------------------------------------------------------
DigitalIndicator indicator( led_A, taskManager );
DigitalButton    button   ( btn_A, taskManager, 40, 1000 );

#if USE_ROTARY_KNOB == true
  DigitalEncoderRotaryknob  encoder( &btnLeft, &btnRight, &btnCtrl, taskManager );
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

