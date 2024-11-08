//*******************************************************************
/*!
\file   configHw.h
\author Thomas Breuer
\date   04.11.2024
\brief  Board specific configuration
*/

//*******************************************************************
/*
Board:    STM32F746-Discovery

\see STM32F746-Discovery/board_pinout.txt
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
#define USE_ADC  3  // use '1', '2' or '3'

/// Select a memory object:
///------------------------
#define USE_MEMORY_TYPE  'B' // use 'B': BKRAM or 'F': Flash

/// Select font and bitmap sources:
///----------------------------------
#define USE_RESOURCE 'I'  // use 'C': Code or 'I': Image

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//*******************************************************************
#include "Hardware/Peripheral/Display/DisplayGraphic_GRAM.cpp"
#include "Hardware/Peripheral/Touch/Touch_FT5336.cpp"

//*******************************************************************
#include "../../Resource/Color/Color.h"

//*******************************************************************
using namespace EmbSysLib::Hw;

//*******************************************************************
PinConfig::MAP PinConfig::table[] =
{
  // ADC
  ADC1_IN0_PA0,
  ADC2_IN0_PA0,
  ADC3_IN0_PA0,
  ADC3_IN4_PF6,
  ADC3_IN5_PF7,
  ADC3_IN6_PF8,
  ADC3_IN7_PF9,
  ADC3_IN8_PF10,

  // I2C
  I2C1_SCL_PB8,
  I2C1_SDA_PB9,

  I2C3_SCL_PH7,
  I2C3_SDA_PH8,

  I2C4_SCL_PD12,
  I2C4_SDA_PB7,

  // PWM
  TIM1_CH1_PA8,

  // UART
  USART1_TX_PA9,
  USART1_RX_PB7,

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
Port_Mcu   portB( Port_Mcu::PB );
Port_Mcu   portD( Port_Mcu::PD );
Port_Mcu   portJ( Port_Mcu::PJ );
Port_Mcu   portI( Port_Mcu::PI );

Port_Mcu  &btnPort = portI;
Port_Mcu  &ledPort = portI;

MTYPE      btnPinMask =  (1<<11);  // B1  (user button)
MTYPE      ledPinMask =  (1<< 1);  // LD1 (green)

//-------------------------------------------------------------------
// Timer
//-------------------------------------------------------------------
Timer_Mcu  timerFast( Timer_Mcu::TIM_2,    100L/*us*/ );
Timer_Mcu  timerSlow( Timer_Mcu::TIM_14, 10000L/*us*/ );
Timer_Mcu  timerPWM ( Timer_Mcu::TIM_1,    512L/*us*/ );

BYTE timerPwmCh = Timer_Mcu::CH1; // LED (ext)

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

I2Cmaster_Mcu i2cBusTouch( I2Cmaster_Mcu::I2C_3, 100/*kHz*/ );

//-------------------------------------------------------------------
// ADC
//-------------------------------------------------------------------
#if USE_ADC == 1
  Adc_Mcu  adc( Adc_Mcu::ADC_1, timerFast );
  BYTE AdcChannelList[] = {0};
#elif USE_ADC == 2
  Adc_Mcu  adc( Adc_Mcu::ADC_2, timerFast );
  BYTE AdcChannelList[] = {0};
#elif USE_ADC == 3
  Adc_Mcu  adc( Adc_Mcu::ADC_3, timerFast );
  BYTE AdcChannelList[] = {0,4,5,6,7,8};
#else
  #error "Wrong or undefined ADC_ID"
#endif

//-------------------------------------------------------------------
// Port::Pin
//-------------------------------------------------------------------
Port::Pin  ledA_Pin( ledPort, 1, Port::Out ); // LD1 (green)
Port::Pin  ledB_Pin( portA,   8, Port::Out ); // LED (ext)
Port::Pin  btnA_Pin( btnPort,11, Port::In  ); // B1 (user button)

//-------------------------------------------------------------------
// Memory
//-------------------------------------------------------------------
#if USE_MEMORY_TYPE == 'B'

  Memory_BKRAM  mem;

#elif USE_MEMORY_TYPE == 'E'

  Memory_EEPROM  mem;

#elif USE_MEMORY_TYPE == 'F'

  Memory_Flash  mem( 3 ); // Sector

#else
  #error "Compiler flag 'USE_MEMORY_TYPE' not defined or wrong value"
#endif


//-------------------------------------------------------------------
// RTC
//-------------------------------------------------------------------
Rtc_Mcu  rtc( Rtc_Mcu::LSE) ;  // The RTC is NOT battery buffered on STM32L100-Discovery!

//-------------------------------------------------------------------
// UART
//-------------------------------------------------------------------
Uart_Mcu  uart( Uart_Mcu::USART_1,
                9600,
                0,
                100, 100 ); // RX and TX buffer size

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

  Memory_Flash image( 7 ); // Size: 256k
  Font    fontFont_10x20      ( MemoryImage( image, "Font_10x20"     ).getPtr() );
  Font    fontFont_16x24      ( MemoryImage( image, "Font_16x24"     ).getPtr() );
  Font    fontFont_8x12       ( MemoryImage( image, "Font_8x12"      ).getPtr() );
  Font    fontFont_8x8        ( MemoryImage( image, "Font_8x8"       ).getPtr() );
  Bitmap  bitmapBitmap_320x240( MemoryImage( image, "Bitmap_320x240" ).getPtr() );

#else
  #error "Compiler flag 'USE_RESOURCE' not defined"
#endif

#define HAS_LCD_ENABLE_PIN
Port::Pin lcdDisp( portI,12, Port::OutPP );

Fmc_Mcu       fmc        ( Fmc_Mcu::SDRAM_Both, Fmc_Mcu::DATA_BUS_WIDTH_16BIT );

DisplayGraphic_GRAM dispGraphic( (WORD*)fmc.startAddr(),
                                 480,272,
                                 fontFont_10x20,
                                 1 );

Ltdc_Mcu ltdc( fmc.startAddr(), true, dispGraphic.getWidth(), dispGraphic.getHeight() );

DisplayChar &disp = dispGraphic; // reuse as character display if needed

//-------------------------------------------------------------------
// Touch
//-------------------------------------------------------------------
Touch_FT5336 touch( i2cBusTouch, 480, 272, Touch::ROTATION_0);

//-------------------------------------------------------------------
// USB
//-------------------------------------------------------------------
#ifdef USE_USB
  USBdeviceDescriptor_0   desc;           // Project related descriptor
  USBdevice_Mcu           usb( desc );
#endif
