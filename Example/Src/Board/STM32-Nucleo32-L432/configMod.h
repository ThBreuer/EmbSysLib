//*******************************************************************
/*!
\file   configMod.h
\author Thomas Breuer
\date   15.10.2022
\brief  Board specific configuration
*/

//*******************************************************************
/*
Board:    STM32-Nucleo32-L432

UART:     Virtual COM port, available via ST-Link/USB
          Connect USB and start a terminal (e.g. Putty)
          Configuration: 9600 baud,8 bit,1 stop bit, no parity, now flow control
                         line editing: force off

USB:      Connect USB to a PC
            D- - PA11 (CN3.13 = white wire) 
            D+ - PA12 (CN3.5  = green wire)
*/

//*******************************************************************
using namespace EmbSysLib::Hw;
using namespace EmbSysLib::Mod;

//*******************************************************************
PinConfig::MAP PinConfig::table[] =
{
  // UART
  USART2_TX_PA2,
  USART2_RX_PA15,
  
  // USB
  USB_DM_PA11,
  USB_DP_PA12,

  END_OF_TABLE
};

//-------------------------------------------------------------------
// UART
//-------------------------------------------------------------------
Uart_Mcu   uart ( Uart_Mcu::USART_2, 9600, 100, 100 );

//-------------------------------------------------------------------
// USB
//-------------------------------------------------------------------
#ifdef USB_DEVICE_ENABLE
USBdeviceDescriptor_0  desc;            // Project related descriptor
USBdevice_Mcu          usb( desc );
#endif
