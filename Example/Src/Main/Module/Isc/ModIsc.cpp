//*******************************************************************
/*!
\file   ModISC.cpp
\author Thomas Breuer
\date
\brief  Sample of using ISC
*/

//*******************************************************************
/*
Usage:    ...
*/

//*******************************************************************

//*******************************************************************
#include <stdio.h>
#include "EmbSysLib.h"
#include "ReportHandler.h"

#include "Module/Isc/Isc_Uart.h"
#include "Module/Isc/Isc_USBdevice.h"
#include "Module/ISC/ISC_USBhost.h"

//-------------------------------------------------------------------
const WORD vid = 0x0023;
const WORD pid = 0x3000;

#define USE_USB
#include "descriptor.cpp"

#include "config.h"

//-----------------------------------------------------------------
class DataIO
{
  public:
    class Data
    {
      public:
        WORD  num     = 0;
        WORD  lastRec = 0;
    } data;

    void onReceive()
    {
      if( enableTerminal )
      {
        terminal.printf("num:%5hu lastRec:%5hu\r\n",data.num, -data.lastRec );
      }
    }

};

//*******************************************************************
#if ISC_INTERFACE == USB_HOST

  Isc_USBhost<60> isc( usb, 0, Crc::FAST );

#elif ISC_INTERFACE == USB_DEVICE

  Isc_USBdevice<60,5> isc( usb, 0, Crc::FAST);

#elif ISC_INTERFACE == UART

  Isc_Uart<60> isc( uartIsc );

#else
  #error "Compiler flag 'ISC_INTERFACE' not defined or wrong value"
#endif

Isc::Data<DataIO, '1'> in ( isc );
Isc::Data<DataIO, '1'> out( isc );

//*******************************************************************
int main(void)
{
  uart.set( "\r\n\nModISC," __DATE__ "," __TIME__ "\r\n\n" );

  #if ISC_INTERFACE == USB_HOST
    usb.open( vid, pid, 0 );
  #elif ISC_INTERFACE == USB_DEVICE
    usb.start();
  #endif

  while( 1 )
  {
    isc.update();

    if( in.isNew() )
    {
      out.data.lastRec = -in.data.num;
    }

    if( out.write() )
    {
      out.data.num++;
    }
  }
}

//EOF
