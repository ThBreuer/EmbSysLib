//*******************************************************************
/*!
\file   configHw.h
\author Thomas Breuer
\date   23.02.2023
\brief  Board specific configuration
*/

//*******************************************************************
/*
Board:    STM32F769-Discovery

\see STM32F769-Discovery/board_pinout.txt
*/

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//
// SETUP:
// ======

/// Select revision of LCD board MB1166:
///-------------------------------------
/// Rev A02: OTM8009A + FT6206
/// Rev A09: NT35510  + FT6306

#define MB1166_REV  2  // use '2' (Rev A02) or '9' (Rev A09)

/// Select I2C emulation (bit banging):
///-----------------------------------
#define USE_I2C_MASTER_EMUL  false

/// Select ADC:
///-----------------------------------
#define USE_ADC  1  // use '1' or '3'

/// Select a memory object:
///------------------------
#define USE_MEMORY_TYPE  'B' // use 'B': BKRAM or 'F': Flash

/// Select font and bitmap sources:
///----------------------------------
#define USE_RESOURCE 'I'  // use 'C': Code or 'I': Image

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//*******************************************************************
#if MB1166_REV == 2
  #include "Hardware/Peripheral/Display/DisplayGraphic_OTM8009A.cpp"
#endif
#if MB1166_REV == 9
  #include "Hardware/Peripheral/Display/DisplayGraphic_NT35510.cpp"
#endif

#include "Hardware/Peripheral/Touch/Touch_FT6206.cpp"

//*******************************************************************
#include "../../Resource/Color/Color.h"

//*******************************************************************
using namespace EmbSysLib::Hw;

//*******************************************************************
PinConfig::MAP PinConfig::table[] =
{
  // ADC
  ADC1_IN4_PA4,
  ADC1_IN6_PA6,
  ADC1_IN12_PC2,
  ADC3_IN6_PF8,
  ADC3_IN7_PF9,
  ADC3_IN8_PF10,
  ADC3_IN12_PC2,

  // DAC
  DAC_OUT1_PA4,

  // I2C
  I2C1_SCL_PB8,
  I2C1_SDA_PB9,

  I2C4_SCL_PD12,
  I2C4_SDA_PB7,

  // PWM
  TIM1_CH4_PA11,

  // UART
  USART1_TX_PA9,
  USART1_RX_PA10,

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
Port_Mcu   portB( Port_Mcu::PB );
Port_Mcu   portD( Port_Mcu::PD );
Port_Mcu   portJ( Port_Mcu::PJ );

Port_Mcu  &btnPort = portA;
Port_Mcu  &ledPort = portJ;

MTYPE      btnPinMask =  (1<< 0);  // B1  (user button)
MTYPE      ledPinMask =  (1<<13)   // LD1 (red)
                        |(1<< 5);  // LD2 (green)

//-------------------------------------------------------------------
// Timer
//-------------------------------------------------------------------
Timer_Mcu  timerFast( Timer_Mcu::TIM_2,    100L/*us*/ );
Timer_Mcu  timerSlow( Timer_Mcu::TIM_14, 10000L/*us*/ );
Timer_Mcu  timerPWM ( Timer_Mcu::TIM_1,    512L/*us*/ );

BYTE timerPwmCh = Timer_Mcu::CH4; // L...

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

I2Cmaster_Mcu i2cBusTouch( I2Cmaster_Mcu::I2C_4, 400/*kHz*/ );

//-------------------------------------------------------------------
// ADC
//-------------------------------------------------------------------
#if USE_ADC == 1

  Adc_Mcu  adc( Adc_Mcu::ADC_1, timerFast );
  BYTE AdcChannelList[] = {4,6,12};

#elif USE_ADC == 3

  Adc_Mcu  adc( Adc_Mcu::ADC_3, timerFast );
  BYTE AdcChannelList[] = {6,7,8,12};

#else
  #error "Wrong or undefined ADC_ID"
#endif

//-------------------------------------------------------------------
// DAC
//-------------------------------------------------------------------
Dac_Mcu  dac;

BYTE DacChannelList[] = {Dac_Mcu::CH1};

//-------------------------------------------------------------------
// Port::Pin
//-------------------------------------------------------------------
Port::Pin  ledA_Pin( ledPort,13, Port::Out ); // LD1 (red)
Port::Pin  ledB_Pin( ledPort, 5, Port::Out ); // LD2 (green)
Port::Pin  btnA_Pin( btnPort, 0, Port::In  ); // B1 (user button)

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

  Memory_Flash image( 10 ); // Size: 256k
  Font    fontFont_10x20      ( MemoryImage( image, "Font_10x20"     ).getPtr() );
  Font    fontFont_16x24      ( MemoryImage( image, "Font_16x24"     ).getPtr() );
  Font    fontFont_8x12       ( MemoryImage( image, "Font_8x12"      ).getPtr() );
  Font    fontFont_8x8        ( MemoryImage( image, "Font_8x8"       ).getPtr() );
  Bitmap  bitmapBitmap_320x240( MemoryImage( image, "Bitmap_320x240" ).getPtr() );

#else
  #error "Compiler flag 'USE_RESOURCE' not defined"
#endif

Port::Pin     lcdResetPin( portJ, 15 );
Fmc_Mcu       fmc        ( Fmc_Mcu::SDRAM_Bank1, Fmc_Mcu::BusConfig_type::DATA_BUS_WIDTH_32BIT );
Dsi_Mcu       hwDSI      ( fmc.startAddr() );

#if MB1166_REV == 2
	DisplayGraphic_OTM8009Aram dispGraphic( hwDSI,lcdResetPin,
											DisplayGraphic_OTM8009A::LANDSCAPE_90,
											fontFont_16x24,
											1 );
#endif

#if MB1166_REV == 9
  DisplayGraphic_NT35510ram dispGraphic( hwDSI,lcdResetPin,
                                        DisplayGraphic_NT35510::LANDSCAPE_90,
                                        fontFont_16x24,
                                        1 );
#endif
// Variante:
// DisplayGraphic_OTM8009Acmd

DisplayChar &disp = dispGraphic; // reuse as character display if needed

//-------------------------------------------------------------------
// Touch
//-------------------------------------------------------------------
#if MB1166_REV == 2
  Touch_FT6206 touch( i2cBusTouch, 480, 800, Touch::ROTATION_0 );
#endif

#if MB1166_REV == 9
  Touch_FT6206 touch( i2cBusTouch, 480, 800, Touch::ROTATION_0, Touch_FT6206::VARIANT_FT6306 );
#endif

//-------------------------------------------------------------------
// USB
//-------------------------------------------------------------------
#ifdef USE_USB
  USBdeviceDescriptor_0   desc;           // Project related descriptor
  USBdevice_Mcu           usb( desc );
#endif
