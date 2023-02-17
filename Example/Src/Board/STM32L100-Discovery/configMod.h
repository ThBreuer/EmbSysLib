//*******************************************************************
/*!
\file   configMod.h
\author Thomas Breuer
\date   26.09.2022
\brief  Board specific configuration
*/

//*******************************************************************
/*
Board:    STM32L100-Discovery

UART:     Connect UART to COM-Port and start a terminal (e.g. Putty)
          Configuration: 9600 baud,8 bit,1 stop bit, no parity, now flow control
                         line editing: force off
            TxD - PA9
            RxD - PA10

USB:      Connect USB to a PC
            D- - PA11 (white wire)
            D+ - PA12 (green wire)
*/

//*******************************************************************
using namespace EmbSysLib::Hw;
using namespace EmbSysLib::Mod;

//*******************************************************************
PinConfig::MAP PinConfig::table[] =
{
  // UART
  USART1_TX_PA9,
  USART1_RX_PA10,
  
  // USB
  USB_DM_PA11,
  USB_DP_PA12,
  
  END_OF_TABLE
};

//-------------------------------------------------------------------
// UART
//-------------------------------------------------------------------
Uart_Mcu  uart ( Uart_Mcu::USART_1, 9600, 100, 100 );

//-------------------------------------------------------------------
// USB
//-------------------------------------------------------------------
#ifdef USB_DEVICE_ENABLE
USBdeviceDescriptor_0  desc;            // Project related descriptor
USBdevice_Mcu          usb( desc );
#endif
