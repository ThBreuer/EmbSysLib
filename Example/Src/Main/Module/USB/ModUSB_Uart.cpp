//*******************************************************************
/*!
\file   ModUSB_Uart.cpp
\author Thomas Breuer
\date   12.07.2023
\brief  Sample of using USB as a Virtual Com Port
*/

//*******************************************************************
/*
Usage:    The USB device is connected as an virtual COM port (VCP, see Windows device manager).
          Start a second terminal after driver installation and connect to the VCP.

          Every input on one of the both terminals is echoed on the other terminal.
*/

//*******************************************************************
#define USE_USB

//*******************************************************************
#include <stdio.h>
#include "EmbSysLib.h"
#include "Module/USB/USB_Uart.h"

//-------------------------------------------------------------------
#include "ReportHandler.h"
#include "ModUSB_Uart/descriptor.cpp"
#include "config.h"

//*******************************************************************
int main(void)
{
  USB_Uart  interfaceCDC( usb,   1,   // configId
                                 0,   // interfId
                               64 ); // uart fifo buffer size 

  usb.start();

  uart.set( "\r\n\nModUSB_Uart," __DATE__ "," __TIME__ "\r\n\n" );

  while(1)
  {
    switch( interfaceCDC.connection.getUnique() )
    {
      case USB_Uart::DISCONNECTED:     uart.set("\r\n<Terminal disconnected>\r\n"); break;
      case USB_Uart::CONNECTED:        uart.set("\r\n<Terminal connected>\r\n");    break;
      case USB_Uart::USB_READY:          uart.set("\r\n<USB ready>\r\n");           break;
      case USB_Uart::USB_DISCONNECTED: uart.set("\r\n<USB disconnected>\r\n");      break;
      default:                                                                      break;
    }

    BYTE c;
    if( uart.get( c ) )
    {
      interfaceCDC.set( c );
    }
    if( interfaceCDC.get( c ) )
    {
      uart.set( c );
    }
  }
}

//EOF
