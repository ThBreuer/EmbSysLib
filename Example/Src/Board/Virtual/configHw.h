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

          Start the Virtual Device Server (see "Example/Project/_VirtualServer.bat")

          LED:      LED 0  - Port_Virtual, Bit 16
                    LED 1  - Port Virtual, Bit 17

          Button:    "A"   - Port_Virtual, Bit 5

          ADC:      Use the slider to set analog values (channel 0,...,3)

          DAC:      Observe the slider representing the analog values (channel 0,...,3)

          Display:  Display area

          Touch:    Move mouse over display area and press left mouse button

          UART:     Terminal (Uart_Stdio)
                  OR
                    Serial IO via COM-Port (Uart_Serial)
                    Connect a serial port (COMx) with another serial com port (COMy), 
                    if necessary using USB to serial converter.
                    Configure target with COMx (e.g. "Uart_Serial  uart( Uart_Serial::BR_9600, "COM1" );".
                    Start a terminal (e.g. PuTTY) on port COMy
                  OR
                    File IO (Uart_File)
                    Create a file (e.g. RxD.txt). This file is read as "serial input". 
                    The serial output is redirected to another file ("TxD.txt").
                    The file names must be defined during instantiation.
                    Use an appropiate editor to edit/watch the files.
                    
          USB:      Connect a microcontroller running the software
                    "Example\Board\...\Src\Hardware\HwUSBdevice.cpp"
                    to the USB port
*/

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

/// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
/// Select font and bitmap sources:
///----------------------------------
//
#define GET_RESOURCE_FROM_CODE
//#define GET_RESOURCE_FROM_FILE
///>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

#if defined GET_RESOURCE_FROM_CODE

  #include "../../Resource/Font/Font_16x24.h"
  #include "../../Resource/Font/Font_10x20.h"
  #include "../../Resource/Font/Font_8x12.h"
  #include "../../Resource/Font/Font_8x8.h"
  #include "../../Resource/Bitmap/Bitmap_320x240.h"

#elif defined GET_RESOURCE_FROM_FILE

  Font        fontFont_10x20      ( Memory_Mcu( "../../../Src/Resource/Font/font_10x20.bin"       ).getPtr() );
  Font        fontFont_16x24      ( Memory_Mcu( "../../../Src/Resource/Font/font_16x24.bin"       ).getPtr() );
  Font        fontFont_8x12       ( Memory_Mcu( "../../../Src/Resource/Font/font_8x12.bin"        ).getPtr() );
  Font        fontFont_8x8        ( Memory_Mcu( "../../../Src/Resource/Font/font_8x8.bin"         ).getPtr() );
  Bitmap      bitmapBitmap_320x240( Memory_Mcu( "../../../Src/Resource/Bitmap/Bitmap_320x240.bin" ).getPtr() );

#else
  #error "Compiler flag 'GET_RESOURCE...' not defined"
#endif

DisplayGraphic_Virtual  dispGraphic( 320, 240,         // Size (w,h)
                                     "localhost:1000",
                                     fontFont_8x12,   // actual font
                                     1 );              // actual zoom factor

DisplayChar_Virtual     dispChar   ( 8, 20, "localhost:1000" );

/// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
/// Select a char display:
///-----------------------
//
DisplayChar &disp = dispChar;
//DisplayChar &disp = dispGraphic;
/// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

//-------------------------------------------------------------------
// Port::Pin
//-------------------------------------------------------------------
Port::Pin  ledA_Pin( port,16, Port::Out ); // LED 0
Port::Pin  ledB_Pin( port,17, Port::Out ); // LED 1
Port::Pin  btnA_Pin( port, 5, Port::In  ); // Button "A"

//-------------------------------------------------------------------
// Memory
//-------------------------------------------------------------------
Memory_Mcu  memFile( "mem.bin", 100 );

Memory &mem = memFile;

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
/// Select an uart object:
///-----------------------
///
Uart_Stdio   uart;
///Uart_Serial  uart( Uart_Serial::BR_9600, "COM1" );
///Uart_File    uart( "RxD.txt", "TxD.txt" );
///-----------------------

//-------------------------------------------------------------------
// USB
//-------------------------------------------------------------------
USBhost_Mcu  usb;
