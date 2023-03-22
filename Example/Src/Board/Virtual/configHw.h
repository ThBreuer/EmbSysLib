//*******************************************************************
/*!
\file   configHw.h
\author Thomas Breuer
\date   26.09.2022
\brief  Board specific configuration
*/

//*******************************************************************
/*
Board:    Virtual

\see Virtual/board_pinout.txt
*/

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//
// SETUP:
// ======

/// Select a UART type:
///--------------------
#define USE_UART_TYPE  'S'     // use 'S': Stdio, 'C': serial COM, 'F': File
#define USE_COM_PORT   "COM1"

/// Select font and bitmap sources:
///----------------------------------
#define USE_RESOURCE 'C'  // use 'C': Code, 'I': Image or 'F': File

/// Select character display emulation:
///------------------------------------
#define USE_CHAR_DISP_EMUL  false

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//*******************************************************************
using namespace EmbSysLib::Hw;
using namespace EmbSysLib::Dev;

//*******************************************************************
#include "../../Resource/Color/Color.h"

//-------------------------------------------------------------------
// Port
//-------------------------------------------------------------------
Port_Virtual  port( "localhost:1000" );

Port_Virtual &ledPort = port;
Port_Virtual &btnPort = port;

MTYPE        ledPinMask = (1<<16)  // LED 0
                         |(1<<17); // LED 1
MTYPE        btnPinMask = (1<< 5); // Button "A"

//-------------------------------------------------------------------
// Timer
//-------------------------------------------------------------------
Timer_Mcu  timerFast(   100L/*us*/ );
Timer_Mcu  timerSlow( 10000L/*us*/ );
Timer_Mcu  timerPWM (   512L/*us*/ ); // todo: PWM not implemented

BYTE timerPwmCh = 0;

//-------------------------------------------------------------------
// ADC
//-------------------------------------------------------------------
Adc_Virtual  adc( "localhost:1000", timerFast );

BYTE AdcChannelList[] = {0,1,2,3};

//-------------------------------------------------------------------
// DAC
//-------------------------------------------------------------------
Dac_Virtual  dac( "localhost:1000" );

BYTE DacChannelList[] = {0,1,2,3};

//-------------------------------------------------------------------
// Display
//-------------------------------------------------------------------
#if USE_RESOURCE == 'C'

  #include "../../Resource/Font/Font_16x24.h"
  #include "../../Resource/Font/Font_10x20.h"
  #include "../../Resource/Font/Font_8x12.h"
  #include "../../Resource/Font/Font_8x8.h"
  #include "../../Resource/Bitmap/Bitmap_320x240.h"

#elif USE_RESOURCE == 'I'

  Memory_Mcu  image   ( "../image.bin" );
  Font        fontFont_10x20      ( MemoryImage( image, "Font_10x20"     ).getPtr() );
  Font        fontFont_16x24      ( MemoryImage( image, "Font_16x24"     ).getPtr() );
  Font        fontFont_8x12       ( MemoryImage( image, "Font_8x12"      ).getPtr() );
  Font        fontFont_8x8        ( MemoryImage( image, "Font_8x8"       ).getPtr() );
  Bitmap      bitmapBitmap_320x240( MemoryImage( image, "Bitmap_320x240" ).getPtr() );

#elif USE_RESOURCE == 'F'

  Font        fontFont_10x20      ( Memory_Mcu( "../../../Src/Resource/Font/font_10x20.bin"       ).getPtr() );
  Font        fontFont_16x24      ( Memory_Mcu( "../../../Src/Resource/Font/font_16x24.bin"       ).getPtr() );
  Font        fontFont_8x12       ( Memory_Mcu( "../../../Src/Resource/Font/font_8x12.bin"        ).getPtr() );
  Font        fontFont_8x8        ( Memory_Mcu( "../../../Src/Resource/Font/font_8x8.bin"         ).getPtr() );
  Bitmap      bitmapBitmap_320x240( Memory_Mcu( "../../../Src/Resource/Bitmap/Bitmap_320x240.bin" ).getPtr() );

#else
  #error "Compiler flag 'USE_RESOURCE' not defined"
#endif

DisplayGraphic_Virtual  dispGraphic( 320, 240,         // Size (w,h)
                                     "localhost:1000",
                                     fontFont_8x12,   // actual font
                                     1 );              // actual zoom factor

#if USE_CHAR_DISP_EMUL == true
  DisplayChar         &disp = dispGraphic;
#else
  DisplayChar_Virtual  disp( 8, 20, "localhost:1000" );
#endif

//-------------------------------------------------------------------
// Port::Pin
//-------------------------------------------------------------------
Port::Pin  ledA_Pin( port,16, Port::Out ); // LED 0
Port::Pin  ledB_Pin( port,17, Port::Out ); // LED 1
Port::Pin  btnA_Pin( port, 5, Port::In  ); // Button "A"

//-------------------------------------------------------------------
// Memory
//-------------------------------------------------------------------
Memory_Mcu  mem( "mem.bin", 100 );

//-------------------------------------------------------------------
// RTC
//-------------------------------------------------------------------
Rtc_Mcu  rtc; //The RTC of the OS is read out but not affected by RTC::set().

//-------------------------------------------------------------------
// Touch
//-------------------------------------------------------------------
Touch_Virtual  touch( "localhost:1000", 320, 240 );

//-------------------------------------------------------------------
// UART
//-------------------------------------------------------------------
#if USE_UART_TYPE == 'S'
  Uart_Stdio   uart;
#elif USE_UART_TYPE == 'C'
  Uart_Serial  uart( Uart_Serial::BR_9600, USE_COM_PORT );
#elif USE_UART_TYPE == 'F'
  Uart_File    uart( "RxD.txt", "TxD.txt" );
#else
  #error "Compiler flag 'USE_UART_TYPE' not defined or wrong value"
#endif

//-------------------------------------------------------------------
// USB
//-------------------------------------------------------------------
USBhost_Mcu  usb;
