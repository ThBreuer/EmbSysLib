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
//<! \todo
/**
 I2C emul is not running. OutOD has to been implemented:
  config, state=(1):
    DDR = 0
    PORT = 0
    (same as INPUT)

  set pin, state=(1):
    DDR  = 0 (input)

  clr pin, state=0 
    DDR  = 1 (output)
**/

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

//-------------------------------------------------------------------
// Timer
//-------------------------------------------------------------------
Timer_4  timerFast(     1000L/*us*/ );
Timer_1  timerSlow(    10000L/*us*/ );
Timer_0  timerPWM (       1024/*us*/, Timer_Mcu::PWM );
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
  I2Cmaster_Mcu i2cBus(  100 /*kHz*/ );
#endif

//-------------------------------------------------------------------
// ADC
//-------------------------------------------------------------------
Adc_Mcu  adc( timerFast );

BYTE AdcChannelList[] = {0,1};

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
SPImaster::Device spiDisplay( spi2, portD, 4 );
SPImaster::Device spiMaster ( spi2, portD, 4 );

//-------------------------------------------------------------------
// memory
//-------------------------------------------------------------------
Memory_EEPROM  mem;

//-------------------------------------------------------------------
// UART
//-------------------------------------------------------------------
Uart_1  uart( 
                9600,       // Baud rate
                100, 100 ); // RX and TX buffer size

//-------------------------------------------------------------------
// USB
//-------------------------------------------------------------------
#ifdef USE_USB
  USBdeviceDescriptor_0   desc;           // Project related descriptor
  USBdevice_Mcu           usb( desc );
#endif
