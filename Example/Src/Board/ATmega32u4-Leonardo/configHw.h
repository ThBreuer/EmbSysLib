//*******************************************************************
/*!
\file   configHw.h
\author Thomas Breuer
\date   23.02.2023
\brief  Board specific configuration
*/

//*******************************************************************
/*
Board:    ...

\see /Example/Src/Board/.../board_pinout.txt
*/

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//
// SETUP:
// ======

/// Select I2C emulation (bit banging):
///-----------------------------------
#define USE_I2C_MASTER_EMUL  false

/// select a memory object:
///------------------------
#define USE_MEMORY_TYPE  'E' // use 'E': EEPROM or 'F': Flash

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//*******************************************************************
using namespace EmbSysLib::Hw;

//-------------------------------------------------------------------
// Port
//-------------------------------------------------------------------
Port_Mcu   portB( Port_Mcu::PortB );
Port_Mcu   portD( Port_Mcu::PortD );
Port_Mcu   portE( Port_Mcu::PortE );

Port_Mcu   &btnPort = portE;
Port_Mcu   &ledPort = portD;

MTYPE      btnPinMask = (1<<6); // ...
MTYPE      ledPinMask = (1<<5); // RX (yellow)

////-------------------------------------------------------------------
//// Timer
////-------------------------------------------------------------------
Timer_4  timerFast(     1000L/*us*/ );
Timer_1  timerSlow(    10000L/*us*/ );
Timer_0  timerPWM (    512/*us*/, Timer_Mcu::PWM );
//
BYTE timerPwmCh = timerPWM.OC0A; // ...
//
//-------------------------------------------------------------------
// I2C
//-------------------------------------------------------------------
#if USE_I2C_MASTER_EMUL == true
  Port::Pin pinSCL( portD, 0 );
  Port::Pin pinSDA( portD, 1 );
  I2Cmaster_Emul i2cBus( pinSCL, pinSDA, 100 /*us*/ );
#else
  I2Cmaster_Mcu i2cBus(  100000 /*kHz*/ );
#endif

//-------------------------------------------------------------------
// ADC
//-------------------------------------------------------------------
Adc_Mcu  adc( timerFast );

BYTE AdcChannelList[] = {0,1};

////-------------------------------------------------------------------
//// DAC
////-------------------------------------------------------------------
//Dac_Mcu  dac;
//
//BYTE DacChannelList[] = {Dac_Mcu::CH1,Dac_Mcu::CH2};

//---------------------------------------------------------------------
// Port::Pin
//---------------------------------------------------------------------

Port::Pin  ledA_Pin( portD, 5, Port::Out ); // TX (yellow)
Port::Pin  ledB_Pin( portB, 0, Port::Out ); // RX (yellow)

Port::Pin  btnA_Pin( portE, 6, Port::In  ); // ...

//-------------------------------------------------------------------
// SPI
//-------------------------------------------------------------------

SPImaster_0     spi2      ( SPImaster_0::CR_1000kHz,
                            SPImaster_0::CPOL_H_CPHA_H );
SPImaster::Device spiDisplay( spi2, portB, 12 );

//-------------------------------------------------------------------
// memory
//-------------------------------------------------------------------
#if USE_MEMORY_TYPE == 'E'

  Memory_EEPROM  mem;

//#elif USE_MEMORY_TYPE == 'F'
//
  //Memory_Flash  mem( 0x20000,   // offset
                       //0x400 ); // size
//
#else
  #error "compiler flag 'use_memory_type' not defined or wrong value"
#endif

////-------------------------------------------------------------------
//// RTC
////-------------------------------------------------------------------
//Rtc_Mcu      rtc; // The RTC is NOT battery buffered on STM32L100-Discovery!
//
////-------------------------------------------------------------------
//// UART
////-------------------------------------------------------------------
Uart_1  uart( 
                9600,       // Baud rate
                100, 100 ); // RX and TX buffer size

//-------------------------------------------------------------------
// USB
//-------------------------------------------------------------------
#ifdef USB_DEVICE_ENABLE
  USBdeviceDescriptor_0   desc;           // Project related descriptor
  USBdevice_Mcu           usb( desc );
#endif
