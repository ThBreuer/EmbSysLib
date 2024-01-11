//*******************************************************************
/*!
\file   HwUSBhost.cpp
\author Thomas Breuer
\date   12.02.2022
\brief  Sample of using hardware related class 'USB'
*/

//*******************************************************************
/*
Usage:    USB host
          Attempts to connect to the USB device. If the device is
          found, OUT and IN transfers are repeatedly initiated and
          transfered data are displayed.

Code:     Select control or interrupt transfer with
          compiler flag TRANSFER_TYPE
*/

//*******************************************************************
#define USB_CTRL          1
#define USB_INTER         2

#define USB_DEVICE_ENABLE


///------------------------------------------------------------------
/// Select an USB transfer type (CTRL|INTER):
#define TRANSFER_TYPE USB_INTER

//*******************************************************************
#include <stdio.h>
#include "EmbSysLib.h"

//-------------------------------------------------------------------
#include "ReportHandler.h"
#include "Resource/USB/USB_data.h"
#include "configHw.h"

//*******************************************************************
int main(int argc, char** argv)
{
  uart.set( "\r\n\nHwUSBhost," __DATE__ "," __TIME__ "\r\n\n" );

  cData writeBuffer;
  cData readBuffer;

  usb.open( vid, pid, 0 ); // vid and pid: see USB_data.h,
                           // also used in Example HwUSBdevice

  while( true )
  {
    bool err = true;

    #if TRANSFER_TYPE == USB_INTER
    //------------------------
    if( usb.writeInterrupt( writeBuffer ) )
    {
      writeBuffer.cnt++;
      if( usb.readInterrupt( readBuffer ) )
      {
        printf( "Interrupt-Transfer %d: OUT=%u IN=%u\r", readBuffer.cnt, readBuffer.onReceive, readBuffer.onTransmit );
        err = false;
      }
    }

    #elif TRANSFER_TYPE == USB_CTRL
    //------------------------
    if( usb.writeCtrl( writeBuffer ) )
    {
      writeBuffer.cnt++;
      if( usb.readCtrl( readBuffer ) )
      {
        printf( "Control-Transfer %d: OUT=%u IN=%u\r", readBuffer.cnt, readBuffer.onReceiveCtrl, readBuffer.onTransmitCtrl );
        err = false;
      }
    }

    #else
      #error "transfer type unknown or undefined!"
    #endif

    if( err || !usb.isConnected() )
    {
      usb.close();
      System::delayMilliSec( 1000 );
      usb.open( vid, pid, 0 );
    }
  }
}
