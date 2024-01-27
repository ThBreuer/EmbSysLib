//*******************************************************************
/*!
\file   HwUSBdevice.cpp
\author Thomas Breuer
\date   12.02.2022
\brief  Sample of using hardware related class 'USB'
*/

//*******************************************************************
/*
Usage:    USB device
          Start USBhost.exe (see HwUSBdevice/_USBhost.bat) on the PC, wait for
          a connection and use the keys to initiate an IN or OUT transfer.

          Every IN- or OUT transfer is counted and displayed on the terminal
*/

//*******************************************************************
#define USE_USB

//*******************************************************************
#include <stdio.h>
#include "EmbSysLib.h"

//-------------------------------------------------------------------
#include "ReportHandler.h"
#include "Resource/USB/USB_data.h"
#include "HwUSBdevice/descriptor.cpp"
#include "configHw.h"

//-----------------------------------------------------------------
class cMyInterf : public USBdeviceInterface
{
  public:
    //---------------------------------------------------------------
    cMyInterf( USBdevice &usb, BYTE interfId )

    : USBdeviceInterface( usb, 1/*configId*/, interfId )
    {
      isNew = false;
    }

    //---------------------------------------------------------------
    virtual void onConfigEndpoint( BYTE epId, DataPointer &data, WORD maxPacketSize )
    {
      if( epId == 0x81 )
      {
        data = writeBuffer;
      }
      else
      {
        data = readBuffer;
      }
    }

    //---------------------------------------------------------------
    virtual bool onTransmit(BYTE epId, DataPointer &data)
    {
      writeBuffer.onTransmit++;
      isNew = true;
      data = writeBuffer;
      return( true );
    }

    //---------------------------------------------------------------
    virtual void onTransmitCtrl( void )
    {
    }

    //---------------------------------------------------------------
    virtual bool onReceive(BYTE epId, WORD cnt, DataPointer &data)
    {
      writeBuffer.onReceive++;
      isNew = true;
      writeBuffer.cnt = readBuffer.cnt;
      data = readBuffer;
      return(true);
    }

    //---------------------------------------------------------------
    virtual bool onReceiveCtrl(WORD cnt)
    {
      writeBuffer.onReceiveCtrl++;
      isNew = true;
      writeBuffer.cnt = readBuffer.cnt;
      return(true);
    }

    //---------------------------------------------------------------
    virtual bool onRequestCtrl_IN( DataPointer &data,
                                   BYTE         request,
                                   WORD         value,
                                   WORD         length )
    {
      writeBuffer.onTransmitCtrl++;
      isNew = true;
      data = writeBuffer;
      return( true );
    }

    //---------------------------------------------------------------
    virtual bool onRequestCtrl_OUT( DataPointer &data,
                                    BYTE          request,
                                    WORD          value,
                                    WORD          length )
    {
      data = readBuffer;
      return( true );
    }

    //---------------------------------------------------------------
    cData      writeBuffer;
    cData      readBuffer;
    Flag<bool> isNew;
};
//*******************************************************************

//*******************************************************************
int main(void)
{
  uart.set( "\r\n\nHwUSBdevice," __DATE__ "," __TIME__ "\r\n\n" );

  cMyInterf               interface(usb, 0 );
  usb.start();

  while( 1 )
  {
    if( interface.isNew.getEvent() )
    {
      uart.set( interface.writeBuffer.toString() );
      uart.set( "\r\n");
    }
  }
}
