//*******************************************************************
/*!
\file   ModUSBdeviceSimpleIO.cpp
\author Thomas Breuer
\date
\brief  Sample of using USB to control volume
*/

//*******************************************************************
/*
Usage:    USB device
          Start USBhost.exe (see HwUSBdevice/_USBhost.bat) on the PC, wait for
          a connection and use the keys 'i' and 'o' to initiate an IN or OUT 
          interrupt transfer.

          Every IN- or OUT transfer is counted and displayed on the terminal
*/

//*******************************************************************
#define USE_USB

//*******************************************************************
#include <stdio.h>
#include "EmbSysLib.h"
#include "Module/USB/USBdeviceSimpleIO.h"

//-------------------------------------------------------------------
#include "ReportHandler.h"
#include "Resource/USB/USB_data.h"
#include "ModUSBdeviceSimpleIO/descriptor.cpp"
#include "config.h"

//*******************************************************************
int main(void)
{
  USBdeviceSimpleIO<cData, cData>  interface( usb, 1/*configId*/, 0/*interfId*/ );
  usb.start();

  uart.set( "\r\n\nModUSBdeviceSimpleIO," __DATE__ "," __TIME__ "\r\n\n" );
  uart.set( "\r\nwaiting ...." );
   
  cData  writeBuffer;
  cData  readBuffer;

  while( 1 )
  {
    // Get USb connection state and print
    switch((USBdeviceSimpleIO<>::Status) interface.status.getEvent() )
    {
      case USBdeviceSimpleIO<>::STARTED: uart.set( "\rUSB started  " ); break;
      case USBdeviceSimpleIO<>::STOPPED: uart.set( "\rUSB stopped  " ); break;
    }
    
    // Data in OUT queue?
    if( interface.receive( readBuffer ) )
    {
      writeBuffer.onReceive++;
      writeBuffer.onTransmit++;
      writeBuffer.cnt = readBuffer.cnt;
      
      uart.set( writeBuffer.toString() );
      uart.set( "\r\n");
    }

    // Write data to IN queue
    interface.transmit( writeBuffer );
  }
}

//EOF
