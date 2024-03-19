//*******************************************************************
/*!
\file   configHw.h
\author Thomas Breuer
\date   15.10.2022
\brief  Board specific configuration
*/

//*******************************************************************
/*
Board:    STM32F4-Discovery + Base-Board (STM32F4DIS-BB) + LCD-Board (STM32F4DIS-LCD)

\see STM32L100-Discovery/board_pinout.txt
*/

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//
// SETUP:
// ======

/// Select I2C emulation (bit banging):
///-----------------------------------
#define USE_I2C_MASTER_EMUL  false

/// Select ADC:
///-----------------------------------
#define USE_ADC  1  // use '1', '2' or '3'

/// Select a memory object:
///------------------------
#define USE_MEMORY_TYPE  'B' // use 'B': BKRAM or 'F': Flash

/// Select font and bitmap sources:
///----------------------------------
#define USE_RESOURCE 'I'  // use 'C': Code or 'I': Image

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//*******************************************************************
#include "Hardware/Peripheral/Display/DisplayGraphic_SSD2119.cpp"
#include "Hardware/Peripheral/Touch/Touch_STMPE811i2c.cpp"

//*******************************************************************
#include "../../Resource/Color/Color.h"

//*******************************************************************
using namespace EmbSysLib::Hw;

//*******************************************************************
PinConfig::MAP PinConfig::table[] =
{
  // ADC
  ADC123_IN3_PA3,
  ADC12_IN8_PB0,
  ADC12_IN9_PB1,

  // DAC
  DAC2_OUT_PA5,

  // Encoder
  TIM2_CH1_PA0,
  TIM2_CH2_PA1,

  // I2C
  I2C1_SCL_PB8,
  I2C1_SDA_PB9,

  // PWM
  TIM4_CH2_PD13,

  // UART
  USART6_TX_PC6,
  USART6_RX_PC7,

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
Port_Mcu   portB( Port_Mcu::PB );
Port_Mcu   portD( Port_Mcu::PD );
Port_Mcu   portE( Port_Mcu::PE );

Port_Mcu  &btnPort = portA;
Port_Mcu  &ledPort = portD;

MTYPE      btnPinMask =  (1<< 0); // BTN (USER)
MTYPE      ledPinMask =  (1<<12)  // LD4 (green)
                        |(1<<13); // LD3 (orange)

//-------------------------------------------------------------------
// Timer
//-------------------------------------------------------------------
Timer_Mcu timerFast( Timer_Mcu::TIM_2,    100L/*us*/ );
Timer_Mcu timerSlow( Timer_Mcu::TIM_3,  10000L/*us*/ );
Timer_Mcu timerPWM ( Timer_Mcu::TIM_4,    512L/*us*/ );

BYTE timerPwmCh = Timer_Mcu::CH2; /////////// LD6 (blue)

//-------------------------------------------------------------------
// Encoder
//-------------------------------------------------------------------
Encoder_Mcu enc( Encoder_Mcu::TIM_2, Encoder_Mcu::NORMAL );

//-------------------------------------------------------------------
// I2C
//-------------------------------------------------------------------
#if USE_I2C_MASTER_EMUL == true
  Port::Pin pinSCL( portB, 8 );
  Port::Pin pinSDA( portB, 9 );
  I2Cmaster_Emul i2cBus( pinSCL, pinSDA, 100/*us*/ );
#else
  I2Cmaster_Mcu i2cBus( I2Cmaster_Mcu::I2C_1, 100/*kHz*/ );
#endif

//-------------------------------------------------------------------
// ADC
//-------------------------------------------------------------------
#if USE_ADC == 1

  Adc_Mcu  adc( Adc_Mcu::ADC_1, timerFast );
  BYTE AdcChannelList[] = {3,8,9};

#elif USE_ADC == 2

  Adc_Mcu  adc( Adc_Mcu::ADC_2, timerFast );
  BYTE AdcChannelList[] = {3,8,9};

#elif USE_ADC == 3

  Adc_Mcu  adc( Adc_Mcu::ADC_3, timerFast );
  BYTE AdcChannelList[] = {3};

#else
  #error "Wrong or undefined USE_ADC"
#endif

//-------------------------------------------------------------------
// DAC
//-------------------------------------------------------------------
Dac_Mcu  dac;

BYTE DacChannelList[] = {Dac_Mcu::CH2};

//-------------------------------------------------------------------
// Port::Pin
//-------------------------------------------------------------------
Port::Pin  ledA_Pin( portD, 12, Port::Out ); // LD4 (green)
Port::Pin  ledB_Pin( portD, 13, Port::Out ); // LD3 (orange)
Port::Pin  btnA_Pin( portA,  0, Port::In  ); // B1  (USER)

//-------------------------------------------------------------------
// ExtInt
//-------------------------------------------------------------------
ExtInt_Mcu extInt( Port_Mcu::PA, 0 ); // External Interrupt on btnA_Pin

//-------------------------------------------------------------------
// Memory
//-------------------------------------------------------------------
#if USE_MEMORY_TYPE == 'B'

  Memory_BKRAM  mem;

#elif USE_MEMORY_TYPE == 'E'

  Memory_EEPROM  mem;

#elif USE_MEMORY_TYPE == 'F'

  Memory_Flash  mem( 10 ); // Sector

#else
  #error "Compiler flag 'USE_MEMORY_TYPE' not defined or wrong value"
#endif


//-------------------------------------------------------------------
// RTC
//-------------------------------------------------------------------
Rtc_Mcu    rtc( Rtc_Mcu::LSI );  // The RTC is NOT battery buffered on STM32F407-Discovery

//-------------------------------------------------------------------
// UART
//-------------------------------------------------------------------
Uart_Mcu   uart ( Uart_Mcu::USART_6,
                  9600,              // Baud rate
                  Uart_Mcu::NONE,    // no handshake, no parity
                  100, 100 );        // RX and TX buffer size

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

  Memory_Flash image( 8,   // first sector @0x08080000
                      9 ); // last sector
                           // total size: 256k

  Font    fontFont_10x20      ( MemoryImage( image, "Font_10x20"     ).getPtr() );
  Font    fontFont_16x24      ( MemoryImage( image, "Font_16x24"     ).getPtr() );
  Font    fontFont_8x12       ( MemoryImage( image, "Font_8x12"      ).getPtr() );
  Font    fontFont_8x8        ( MemoryImage( image, "Font_8x8"       ).getPtr() );
  Bitmap  bitmapBitmap_320x240( MemoryImage( image, "Bitmap_320x240" ).getPtr() );

#else
  #error "Compiler flag 'USE_RESOURCE' not defined"
#endif

Port::Pin     pinBackLight ( portD, 13 );
Port::Pin     pinReset     ( portD,  3 );
Fsmc_Mcu      fsmc         ( 0 );

DisplayGraphic_SSD2119  dispGraphic( fsmc.getPtr(),
                                     pinReset,
                                     pinBackLight ,
                                     fontFont_8x12, // font
                                     1 );           // actual zoom factor

DisplayChar &disp = dispGraphic; // reuse as character display if needed

//-------------------------------------------------------------------
// Touch
//-------------------------------------------------------------------
Touch_STMPE811i2c touch( i2cBus, 0, 320, 240, Touch::ROTATION_0 );

//-------------------------------------------------------------------
// USB
//-------------------------------------------------------------------
#ifdef USE_USB
  USBdeviceDescriptor_0   desc;           // Project related descriptor
  USBdevice_Mcu           usb( desc );
#endif
