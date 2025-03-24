//*******************************************************************
/*!
\file   configHw.h
\author Thomas Breuer
\date   23.02.2023
\brief  Board specific configuration
*/

//*******************************************************************
/*
Board:    STM32-Nucleo32-L432

\see STM32-Nucleo32-L432/board_pinout.txt
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
#define USE_MEMORY_TYPE  'B' /// use 'B': BKRAM or 'F': Flash

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//*******************************************************************
using namespace EmbSysLib::Hw;

//*******************************************************************
PinConfig::MAP PinConfig::table[] =
{
  // ADC
  ADC1_IN5_PA0,
  //ADC1_IN6_PA1,
  ADC1_IN8_PA3,
  ADC1_IN9_PA4,
  ADC1_IN10_PA5,
  ADC1_IN11_PA6,
  ADC1_IN12_PA7,
  ADC1_IN15_PB0,
  ADC1_IN16_PB1,

  // DAC
  //DAC1_OUT1_PA4,
  DAC1_OUT2_PA5,

  // Encoder
  TIM2_CH1_PA0,
  TIM2_CH2_PA1,

  // I2C
  I2C1_SCL_PB6,
  I2C1_SDA_PB7,

  // PWM
  TIM2_CH2_PB3,

  // SPI
  SPI1_MOSI_PB5,
  SPI1_MISO_PB4,
  SPI1_SCK_PA1,
  SPI1_NSS_PA4,

  // UART
  USART2_TX_PA2,
  USART2_RX_PA15,

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

Port_Mcu  &btnPort = portA;
Port_Mcu  &ledPort = portB;

MTYPE      btnPinMask = (1<<9); // BTN (ext)
MTYPE      ledPinMask = (1<<3); // LD3 (green)

//-------------------------------------------------------------------
// Timer
//-------------------------------------------------------------------
Timer_Mcu timerFast( Timer_Mcu::TIM_7,    100L/*us*/ );
Timer_Mcu timerSlow( Timer_Mcu::TIM_6,  10000L/*us*/ );
Timer_Mcu timerPWM ( Timer_Mcu::TIM_2,    512L/*us*/ );

BYTE timerPwmCh = Timer_Mcu::CH2; // LD3 (green)

//-------------------------------------------------------------------
// Encoder
//-------------------------------------------------------------------
Encoder_Mcu enc( Encoder_Mcu::TIM_2, Encoder_Mcu::NORMAL );

//-------------------------------------------------------------------
// I2C
//-------------------------------------------------------------------
#if USE_I2C_MASTER_EMUL == true
  Port::Pin pinSCL( portB, 6 );
  Port::Pin pinSDA( portB, 7 );
  I2Cmaster_Emul i2cBus( pinSCL, pinSDA, 10/*us*/ );
#else
  I2Cmaster_Mcu i2cBus( I2Cmaster_Mcu::I2C_1, 400/*kHz*/ );
#endif

//-------------------------------------------------------------------
// ADC
//-------------------------------------------------------------------
Adc_Mcu  adc( timerFast );

BYTE AdcChannelList[] = {5,/*6,*/8,9,10,11,12,15,16};

//-------------------------------------------------------------------
// DAC
//-------------------------------------------------------------------
Dac_Mcu  dac;

BYTE DacChannelList[] = {Dac_Mcu::CH1,Dac_Mcu::CH2};

//-------------------------------------------------------------------
// Port::Pin
//-------------------------------------------------------------------
Port::Pin  ledA_Pin( portB, 3, Port::Out  ); // LD3 (green)
Port::Pin  ledB_Pin( portB, 1, Port::Out  ); // LED (ext)
Port::Pin  btnA_Pin( portA, 9, Port::InPU ); // BTN (ext)

//-------------------------------------------------------------------
// ExtInt
//-------------------------------------------------------------------
ExtInt_Mcu extInt( Port_Mcu::PA, 9 ); // External Interrupt on btnA_Pin

//-------------------------------------------------------------------
// SPI
//-------------------------------------------------------------------
SPImaster_Mcu     spi2      ( SPImaster_Mcu::SPI_1,
                              SPImaster_Mcu::CR_312kHz,
                              SPImaster_Mcu::CPOL_H_CPHA_H,
                              true );
SPImaster::Device spiDisplay( spi2, portA, 4 );

SPImaster::Device &spiMaster   = spiDisplay;
SPImaster::Device &spiENC28J60 = spiDisplay;

//-------------------------------------------------------------------
// Memory
//-------------------------------------------------------------------
#if USE_MEMORY_TYPE == 'B'

  Memory_BKRAM  mem;

#elif USE_MEMORY_TYPE == 'F'

Memory_Flash    memFlash( 0x3f800, // Offset
                          0x800 ); // Size

#else
  #error "Compiler flag 'USE_MEMORY_TYPE' not defined or wrong value"
#endif

//-------------------------------------------------------------------
// RTC
//-------------------------------------------------------------------
Rtc_Mcu      rtc( Rtc_Mcu::LSI );  // The RTC is NOT battery buffered on STM32-Nucleo!

//-------------------------------------------------------------------
// UART
//-------------------------------------------------------------------
Uart_Mcu   uart ( Uart_Mcu::USART_2,
                  9600,       // Baud rate
                  100, 100 ); // RX and TX buffer size

//-------------------------------------------------------------------
// USB
//-------------------------------------------------------------------
#ifdef USE_USB
  USBdeviceDescriptor_0   desc;           // Project related descriptor
  USBdevice_Mcu           usb( desc );
#endif
