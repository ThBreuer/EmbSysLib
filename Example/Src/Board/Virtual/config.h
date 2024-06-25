//*******************************************************************
/*!
\file   config.h
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

/// Select encoder type:
///---------------------
#define USE_ROTARY_KNOB  false

/// Select:
///-----------------------------------
#define USB_HOST   'H'
#define UART       'U'

#define ISC_INTERFACE       UART
#define ISC_INTERFACE_PORT  "COM31"

/// Select font and bitmap sources:
///----------------------------------
#define USE_RESOURCE 'F'  // use 'C': Code, 'I': Image or 'F': File


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


//*******************************************************************
using namespace EmbSysLib::Hw;
using namespace EmbSysLib::Dev;
using namespace EmbSysLib::Ctrl;
using namespace EmbSysLib::Mod;

//-------------------------------------------------------------------
// Port
//-------------------------------------------------------------------
Port_Virtual port( "localhost:1000" );

// Timer
//-------------------------------------------------------------------
Timer_Mcu   timer( 1000L/*us*/ );

TaskManager taskManager( timer );

//-------------------------------------------------------------------
// Display
//-------------------------------------------------------------------
//*******************************************************************
#include "../../Resource/Color/Color.h"

//-------------------------------------------------------------------
// Display
//-------------------------------------------------------------------
#if USE_RESOURCE == 'C'

  #include "../../Resource/Font/Font_16x24.h"
  #include "../../Resource/Font/Font_10x20.h"
  #include "../../Resource/Font/Font_8x12.h"
  #include "../../Resource/Font/Font_8x8.h"
  #include "../../Resource/Bitmap/Bitmap_320x240.h"
  #include "../../Resource/Bitmap/Bitmap_32x32.bin"

#elif USE_RESOURCE == 'I'

  Memory_Mcu  image   ( "../image.bin" );
  Font        fontFont_10x20      ( MemoryImage( image, "Font_10x20"     ).getPtr() );
  Font        fontFont_16x24      ( MemoryImage( image, "Font_16x24"     ).getPtr() );
  Font        fontFont_8x12       ( MemoryImage( image, "Font_8x12"      ).getPtr() );
  Font        fontFont_8x8        ( MemoryImage( image, "Font_8x8"       ).getPtr() );
  Bitmap      bitmapBitmap_320x240( MemoryImage( image, "Bitmap_320x240" ).getPtr() );
  Bitmap      bitmapBitmap_32x32  ( MemoryImage( image, "Bitmap_32x32"   ).getPtr() );

#elif USE_RESOURCE == 'F'

  Font        fontFont_10x20      ( Memory_Mcu( "../../../Src/Resource/Font/font_10x20.bin"       ).getPtr() );
  Font        fontFont_16x24      ( Memory_Mcu( "../../../Src/Resource/Font/font_16x24.bin"       ).getPtr() );
  Font        fontFont_8x12       ( Memory_Mcu( "../../../Src/Resource/Font/font_8x12.bin"        ).getPtr() );
  Font        fontFont_8x8        ( Memory_Mcu( "../../../Src/Resource/Font/font_8x8.bin"         ).getPtr() );
  Bitmap      bitmapBitmap_320x240( Memory_Mcu( "../../../Src/Resource/Bitmap/Bitmap_320x240.bin" ).getPtr() );
  Bitmap      bitmapBitmap_32x32  ( Memory_Mcu( "../../../Src/Resource/Bitmap/Bitmap_32x32.bin"   ).getPtr() );

#else
  #error "Compiler flag 'USE_RESOURCE' not defined"
#endif


DisplayGraphic_Virtual  dispGraphic( 800, 480, "localhost:1000", fontFont_8x12, 1 );

DisplayChar_Virtual  disp( 8, 20, "localhost:1000" );

ScreenChar screenChar( disp );

ScreenGraphic screenGraphic( dispGraphic );

//-------------------------------------------------------------------
// UART
//-------------------------------------------------------------------
Uart_Stdio uart( true );

Terminal   terminal( uart, 255,255,"# +" );

//-------------------------------------------------------------------
// Touch
//-------------------------------------------------------------------
Touch_Virtual  touch( "localhost:1000", 320, 240 );

Pointer        pointer( touch );

//-------------------------------------------------------------------
// ADC
//-------------------------------------------------------------------
Adc_Virtual  adc( "localhost:1000", timer );

//-------------------------------------------------------------------
// DAC
//-------------------------------------------------------------------
Dac_Virtual  dac( "localhost:1000" );

//-------------------------------------------------------------------
// AnalogIn/Out
//-------------------------------------------------------------------
AnalogInAdc   analogIn   ( adc, 0 );
AnalogOutDac  analogOut_A( dac, 2 );
AnalogOutDac  analogOut_B( dac, 3 );

//-------------------------------------------------------------------
// Digital
//-------------------------------------------------------------------
Digital       led_A( port,16, Digital::Out , 0 ); // LED 0
Digital       btn_A( port, 5, Digital::In  , 0 ); // Button "A"

Digital       btnLeft ( port, 0, Digital::In  , 0 ); // Button "<<"
Digital       btnCtrl ( port, 1, Digital::In  , 0 ); // Button "o"
Digital       btnRight( port, 2, Digital::In  , 0 ); // Button ">>"

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
USBhost_Mcu usb;
Uart_Serial uartIsc( Uart_Serial::BR_9600, ISC_INTERFACE_PORT );

const bool enableTerminal = true;


