//*******************************************************************
/*!
\file   configHw.h
\author Thomas Breuer
\date   15.10.2022
\brief  Board specific configuration
*/

//*******************************************************************
/*
Board:    STM32-Nucleo32-L432

LED:      LD3 (green) - PB3 (TIM2_CH2)
          LED (ext)   - PB1, connect LED to CN3.9

Button:   BTN (ext)   - PB4, connect external button to CN3.15

UART:     Virtual COM port, available via ST-Link/USB
          Connect USB and start a terminal (e.g. Putty)
          Configuration: 9600 baud,8 bit,1 stop bit, no parity, now flow control
                         line editing: force off

ADC:      Connect at least one voltage source (0,...,3V or potentiometer)
            Channel  5  - PA0 (CN4.12)
            Channel  6  - PA1 (CN4.11)
            Channel  7  - not usable, occupied by USART2_TX
            Channel  8  - PA3 (CN4.10)
            Channel  9  - PA4 (CN4.9)
            Channel 10  - PA5 (CN4.8)
            Channel 11  - PA6 (CN4.7) (see note 1)
            Channel 12  - PA7 (CN4.6)
            Channel 15  - PB0 (CN3.6)
            Channel 16  - PB1 (CN3.9)

DAC:      Connect at least one port pin to a voltmeter
            Channel 1 - PA4 (CN4.9) 
            Channel 2 - PA5 (CN4.8) (see note 1)

Display:  The UART terminal is used to emulate a display

USB:      Connect USB to a PC
            D- - PA11 (CN3.13 = white wire)
            D+ - PA12 (CN3.5  = green wire)

I2C:      Connect I2C devices to
            SCL - PB6 (CN3.8) (see note 1)
            SDA - PB7 (CN3.7) (see note 1)
            All sub addresses of the devices must be set to 0 (Gnd)

          - PCF8574 I/O expander, used as additional Hw::Port
                    Connect 2 LEDs at Pin 0,1 and 1 Button at 7 of PCF8574
          - PCF8583 Battery buffered real time clock, used as additonal Hw:Rtc
          - PCF8583 Battery buffered RAM, used as additional Hw::Memory
		  
note 1: Jumper SB16 connects PB6 with PA6. 
        Jumper SB18 connects PB7 with PA5. 		
*/

//*******************************************************************
#include "Hardware/Peripheral/Display/DisplayChar_Terminal.h"
#include "Hardware/Peripheral/Rtc/Rtc_PCF8583.h"
#include "Hardware/Peripheral/Memory/Memory_PCF8583.h"
#include "Hardware/Peripheral/Port/Port_PCF8574.h"

//*******************************************************************
using namespace EmbSysLib::Hw;

//*******************************************************************
PinConfig::MAP PinConfig::table[] =
{
  // ADC
  ADC1_IN5_PA0,
  ADC1_IN6_PA1,
  ADC1_IN8_PA3,
  ADC1_IN9_PA4,
  ADC1_IN10_PA5,
  ADC1_IN11_PA6,
  ADC1_IN12_PA7,
  ADC1_IN15_PB0,
  ADC1_IN16_PB1,

  // DAC
  DAC1_OUT1_PA4,
  DAC1_OUT2_PA5,

  // I2C
  I2C1_SCL_PB6,
  I2C1_SDA_PB7,

  // PWM
  TIM2_CH2_PB3,

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
Port_Mcu   portB( Port_Mcu::PB );

Port_Mcu  &btnPort = portB;
Port_Mcu  &ledPort = portB;

MTYPE      btnPinMask = (1<<4); // BTN (ext)
MTYPE      ledPinMask = (1<<3); // LD3 (green)

//-------------------------------------------------------------------
// Timer
//-------------------------------------------------------------------
Timer_Mcu timerFast( Timer_Mcu::TIM_7,    100L/*us*/ );
Timer_Mcu timerSlow( Timer_Mcu::TIM_6,  10000L/*us*/ );
Timer_Mcu timerPWM ( Timer_Mcu::TIM_2,    512L/*us*/ );

BYTE timerPwmCh = Timer_Mcu::CH2; // LD3 (green)

//-------------------------------------------------------------------
// I2C
//-------------------------------------------------------------------
#if USE_I2C_MASTER_EMUL == true
Port::Pin pinSCL( portB, 6 );
Port::Pin pinSDA( portB, 7 );
I2Cmaster_Emul i2cBus( pinSCL, pinSDA, 10/*us*/ );
#else
I2Cmaster_Mcu i2cBus( I2Cmaster_Mcu::I2C_1, 10/*kHz*/ );
#endif

//-------------------------------------------------------------------
// ADC
//-------------------------------------------------------------------
Adc_Mcu  adc( timerFast );

BYTE AdcChannelList[] = {5,6,8,9,10,11,12,15,16};

//-------------------------------------------------------------------
// DAC
//-------------------------------------------------------------------
Dac_Mcu  dac;

BYTE DacChannelList[] = {Dac_Mcu::CH1,Dac_Mcu::CH2};

//-------------------------------------------------------------------
// Port::Pin
//-------------------------------------------------------------------

/// Select usage of I2C I/O expander:
///----------------------------------
#define USE_PORT_EXPANDER false
///----------------------------------

#if USE_PORT_EXPANDER == true
Port_PCF8574 portExp( i2cBus,   // I2C I/O expander
                           0 ); // sub address

Port::Pin  ledA_Pin( portExp, 0, Port::Out ); //
Port::Pin  ledB_Pin( portExp, 1, Port::Out ); //
Port::Pin  btnA_Pin( portExp, 2, Port::In  ); //
#else
Port::Pin  ledA_Pin( portB, 3, Port::Out  ); // LD3 (green)
Port::Pin  ledB_Pin( portB, 1, Port::Out  ); // LED (ext)
Port::Pin  btnA_Pin( portB, 4, Port::InPU ); // BTN (ext)
#endif

//-------------------------------------------------------------------
// Memory
//-------------------------------------------------------------------
Memory_BKRAM    memBKRAM;

Memory_Flash    memFlash( 0x3f800,   // Offset
                          0x800 ); // Size

Memory_PCF8583  memPCF8583( i2cBus,   //
                                 0 ); // sub address

/// Select a memory object:
///-----------------------
Memory &mem = memBKRAM;
///-----------------------

//-------------------------------------------------------------------
// RTC
//-------------------------------------------------------------------
Rtc_Mcu      rtcMcu( Rtc_Mcu::LSI );  // The RTC is NOT battery buffered on STM32-Nucleo!

Rtc_PCF8583  rtcI2C( i2cBus,   //
                          0 ); // sub address

/// Select a RTC object:
///---------------------
Rtc &rtc = rtcMcu;
///---------------------

//-------------------------------------------------------------------
// UART
//-------------------------------------------------------------------
Uart_Mcu   uart ( Uart_Mcu::USART_2,
                  9600,       // Baud rate
                  100, 100 ); // RX and TX buffer size

//-------------------------------------------------------------------
// USB
//-------------------------------------------------------------------
#ifdef USB_DEVICE_ENABLE
USBdeviceDescriptor_0   desc;           // Project related descriptor
USBdevice_Mcu           usb( desc );
#endif

//-------------------------------------------------------------------
// Display
//-------------------------------------------------------------------
DisplayChar_Terminal  disp( uart );
