//*******************************************************************
/*!
\file   configHw.h
\author Thomas Breuer
\date   26.09.2022
\brief  Board specific configuration
*/

//*******************************************************************
/*
Board:    STM32L100-Discovery

LED:      LD3 (green) - PC9
          LD4 (blue)  - PC8 (TIM3_CH3)

Button:   B1 (user)   - PA0 (with pull down resistor)

UART:     Connect UART to COM-Port and start a terminal (e.g. Putty)
          Configuration: 9600 baud,8 bit,1 stop bit, no parity, now flow control
                         line editing: force off
            TxD - PA9
            RxD - PA10

ADC:      Connect at least one voltage source (0,...,3V or potentiometer)
            Channel 0 to 7   - PA1 to PA7
            Channel 8        - PB0
            Channel 9        - PB1
            Channel 10 to 15 - PC0 to PC5

DAC:      Connect at least one port pin to a voltmeter
            Channel 1 - PA4
            Channel 2 - PA5

Display:  The UART terminal is used to emulate a display

USB:      Connect USB to a PC
            D- - PA11 (white wire)
            D+ - PA12 (green wire)

I2C:      Connect I2C devices to
            SCL - PB10
            SDA - PB11
            All sub addresses of the devices must be set to 0 (Gnd)

          - PCF8574 I/O expander, used as additional Hw::Port
                    Connect 2 LEDs at Pin 0,1 and 1 Button at 6 of PCF8574
          - PCF8583 Battery buffered real time clock, used as additonal Hw:Rtc
          - PCF8583 Battery buffered RAM, used as additional Hw::Memory
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

  // I2C
  I2C2_SCL_PB10,
  I2C2_SDA_PB11,

  // PWM
  TIM3_CH3_PC8,

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

/// Select usage of I2C I/O expander:
///----------------------------------
#define USE_PORT_EXPANDER false
///----------------------------------

#if USE_PORT_EXPANDER == true
Port_PCF8574 portExp( i2cBus,   // I2C I/O expander
                           0 ); // sub address

Port::Pin  ledA_Pin( portExp, 0, Port::Out ); //
Port::Pin  ledB_Pin( portExp, 1, Port::Out ); //
Port::Pin  btnA_Pin( portExp, 6, Port::In  ); //
#else
Port::Pin  ledA_Pin( portC, 8, Port::Out ); // LD4 (blue)
Port::Pin  ledB_Pin( portC, 9, Port::Out ); // LD3 (green)
Port::Pin  btnA_Pin( portA, 0, Port::In  ); // B1 (user button, pull down resistor onboard)
#endif

//-------------------------------------------------------------------
// Memory
//-------------------------------------------------------------------
Memory_BKRAM    memBKRAM;
Memory_EEPROM   memEEPROM;
Memory_Flash    memFlash( 0x20000,   // Offset
                            0x400 ); // Size

Memory_PCF8583  memPCF8583( i2cBus,   //
                                 0 ); // sub address

/// Select a memory object:
///-----------------------
Memory &mem = memBKRAM;
///-----------------------

//-------------------------------------------------------------------
// RTC
//-------------------------------------------------------------------
Rtc_Mcu      rtcMcu; // The RTC is NOT battery buffered on STM32L100-Discovery!

Rtc_PCF8583  rtcI2C( i2cBus,   //
                          0 ); // sub address

/// Select a RTC object:
///---------------------
Rtc &rtc = rtcMcu;
///---------------------

//-------------------------------------------------------------------
// UART
//-------------------------------------------------------------------
Uart_Mcu  uart( Uart_Mcu::USART_1,
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
