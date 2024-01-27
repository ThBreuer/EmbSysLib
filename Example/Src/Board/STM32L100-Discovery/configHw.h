//*******************************************************************
/*!
\file   configHw.h
\author Thomas Breuer
\date   23.02.2023
\brief  Board specific configuration
*/

//*******************************************************************
/*
Board:    STM32L100-Discovery

\see /Example/Src/Board/STM32L100-Discovery/board_pinout.txt
*/

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//
// SETUP:
// ======

/// Select I2C emulation (bit banging):
///-----------------------------------
#define USE_I2C_MASTER_EMUL  false

/// Select a memory object:
///------------------------
#define USE_MEMORY_TYPE  'B' // use 'B': BKRAM, 'E': EEPROM or 'F': Flash

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//*******************************************************************
using namespace EmbSysLib::Hw;

//*******************************************************************
PinConfig::MAP PinConfig::table[] =
{
  // ADC
  ADC12_IN0_PA0,
  ADC12_IN1_PA1,
  ADC12_IN2_PA2,
  ADC12_IN3_PA3,
  ADC12_IN4_PA4,
  ADC12_IN5_PA5,
  ADC12_IN6_PA6,
  ADC12_IN7_PA7,
  ADC12_IN8_PB0,
  ADC12_IN9_PB1,
  ADC12_IN10_PC0,
  ADC12_IN11_PC1,
  ADC12_IN12_PC2,
  ADC12_IN13_PC3,
  ADC12_IN14_PC4,
  ADC12_IN15_PC5,

  // DAC
  DAC1_OUT_PA4,
  DAC2_OUT_PA5,
  
  // Encoder
  TIM2_CH1_PA0,
  TIM2_CH2_PA1,

  // I2C
  I2C2_SCL_PB10,
  I2C2_SDA_PB11,

  // PWM
  TIM3_CH3_PC8,

  // SPI
  SPI2_MOSI_PB15,
  SPI2_MISO_PB14,
  SPI2_SCK_PB13,
  SPI2_NSS_PB12,

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
Port_Mcu   portB( Port_Mcu::PB );
Port_Mcu   portC( Port_Mcu::PC );
Port_Mcu   portD( Port_Mcu::PD );

Port_Mcu   &btnPort = portA;
Port_Mcu   &ledPort = portC;

MTYPE      btnPinMask = (1<<0); // B1 (user button)
MTYPE      ledPinMask = (1<<8)  // LD4 (blue)
                       |(1<<9); // LD3 (green)

//-------------------------------------------------------------------
// Timer
//-------------------------------------------------------------------
Timer_Mcu  timerFast( Timer_Mcu::TIM_2,    100L/*us*/ );
Timer_Mcu  timerSlow( Timer_Mcu::TIM_10, 10000L/*us*/ );
Timer_Mcu  timerPWM ( Timer_Mcu::TIM_3,    512L/*us*/ );

BYTE timerPwmCh = Timer_Mcu::CH3; // LD4 (blue)

//-------------------------------------------------------------------
// Encoder
//-------------------------------------------------------------------
Encoder_Mcu enc( Encoder_Mcu::TIM_2, Encoder_Mcu::NORMAL );

//-------------------------------------------------------------------
// I2C
//-------------------------------------------------------------------
#if USE_I2C_MASTER_EMUL == true
  Port::Pin pinSCL( portB, 10 );
  Port::Pin pinSDA( portB, 11 );
  I2Cmaster_Emul i2cBus( pinSCL, pinSDA, 100 /*us*/ );
#else
  I2Cmaster_Mcu i2cBus( I2Cmaster_Mcu::I2C_2, 10 /*kHz*/ );
#endif

//-------------------------------------------------------------------
// ADC
//-------------------------------------------------------------------
Adc_Mcu  adc( timerFast );

BYTE AdcChannelList[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

//-------------------------------------------------------------------
// DAC
//-------------------------------------------------------------------
Dac_Mcu  dac;

BYTE DacChannelList[] = {Dac_Mcu::CH1,Dac_Mcu::CH2};

//-------------------------------------------------------------------
// Port::Pin
//-------------------------------------------------------------------

Port::Pin  ledA_Pin( portC, 8, Port::Out ); // LD4 (blue)
Port::Pin  ledB_Pin( portC, 9, Port::Out ); // LD3 (green)
Port::Pin  btnA_Pin( portA, 0, Port::In  ); // B1 (user button, pull down resistor onboard)


//-------------------------------------------------------------------
// SPI
//-------------------------------------------------------------------

SPImaster_Mcu     spi2      ( SPImaster_Mcu::SPI_2,
                              SPImaster_Mcu::CR_1000kHz,
                              SPImaster_Mcu::CPOL_H_CPHA_H );
SPImaster::Device spiDisplay( spi2, portB, 12 );
SPImaster::Device spiMaster ( spi2, portB, 12 );

//-------------------------------------------------------------------
// Memory
//-------------------------------------------------------------------
#if USE_MEMORY_TYPE == 'B'

  Memory_BKRAM  mem;

#elif USE_MEMORY_TYPE == 'E'

  Memory_EEPROM  mem;

#elif USE_MEMORY_TYPE == 'F'

  Memory_Flash  mem( 0x20000,   // Offset
                       0x400 ); // Size

#else
  #error "Compiler flag 'USE_MEMORY_TYPE' not defined or wrong value"
#endif

//-------------------------------------------------------------------
// RTC
//-------------------------------------------------------------------
Rtc_Mcu      rtc; // The RTC is NOT battery buffered on STM32L100-Discovery!

//-------------------------------------------------------------------
// UART
//-------------------------------------------------------------------
Uart_Mcu  uart( Uart_Mcu::USART_1,
                9600,       // Baud rate
                100, 100 ); // RX and TX buffer size

//-------------------------------------------------------------------
// USB
//-------------------------------------------------------------------
#ifdef USE_USB
  USBdeviceDescriptor_0   desc;           // Project related descriptor
  USBdevice_Mcu           usb( desc );
#endif
