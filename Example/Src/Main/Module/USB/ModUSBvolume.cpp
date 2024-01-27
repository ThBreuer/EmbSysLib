//*******************************************************************
/*!
\file   ModUSBvolume.cpp
\author Thomas Breuer
\date
\brief  Sample of using USB to control volume
*/

//*******************************************************************
/*
Usage:    Set volume control
          Connect to USB (output) and to a terminal (command input)
          Send a command via terminal to the uC, this command is 
          redirected via USB to the Host:
            '+' - Increment volume
            '-' - Decrement volume
            ' ' - Toggle mute
*/

//*******************************************************************
#define USE_USB

//*******************************************************************
#include <stdio.h>
#include "EmbSysLib.h"
#include "Module/USB/USBinterfClassHID.h"

//-------------------------------------------------------------------
#include "ReportHandler.h"
#include "ModUSBvolume/descriptor.cpp"
#include "config.h"

//-----------------------------------------------------------------
class cMyInterf : public USBinterfClassHID
{
  public:

    Flag<BYTE> key;

    //-----------------------------------------------------------------
    class __attribute__((packed)) cData
    {
      public:
        cData()
        {
          modifier = 0;
        }

      public:
        BYTE modifier;
    };

  public:
    cData dataIN;

    //---------------------------------------------------------------
    cMyInterf( USBdevice &usb,
               BYTE       configId,
               BYTE       interfId )
    : USBinterfClassHID( usb, configId, interfId, DataPointer(dataIN), DataPointer() )
    {
    }

    //---------------------------------------------------------------
    virtual bool onReportTransmit()
    {
      switch( key.getEvent() )
      {
        case ' ':  dataIN.modifier = (1<<0); break;
        case '+':  dataIN.modifier = (1<<1); break;
        case '-':  dataIN.modifier = (1<<2); break;
        default:   dataIN.modifier = 0;      break;
      }
      return( true );
    }

   //---------------------------------------------------------------
    virtual bool onReportReceive()
    {
      return( true );
    }

}; //class cMyInterf

//*******************************************************************
int main(void)
{
  cMyInterf  interfaceHID( usb, 1/*configId*/, 0/*interfId*/ );
  usb.start();

  uart.set( "\r\n\nModUSBvolume," __DATE__ "," __TIME__ "\r\n\n" );
  uart.set( "\r\nwaiting ...." );

  while( !interfaceHID.isStarted ) {}

  uart.set( "\rstarted!    \r\n\n" );
  while(1)
  {
    BYTE c = uart.get();
    if( c >=  0x20 ) // skip control keys
    {
      interfaceHID.key = c;
    }
  }
}

//EOF
