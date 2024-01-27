//*******************************************************************
/*!
\file   ModUSBinterfClassHID.cpp
\author Thomas Breuer
\date
\brief  Sample of using USB as HID mouse
*/

//*******************************************************************
/*
Usage:    The USB device is connected as a mouse (HID class).
          After driver installation the mouse pointer will move around
          a square with left mouse button pressed
*/

//*******************************************************************
#define USE_USB

//*******************************************************************
#include <stdio.h>
#include "EmbSysLib.h"
#include "Module/USB/USBinterfClassHID.h"

//-------------------------------------------------------------------
#include "ReportHandler.h"
#include "ModUSBmouse/descriptor.cpp"
#include "config.h"

//-----------------------------------------------------------------
class cMyInterf : public USBinterfClassHID
{
  public:

    WORD cnt;
    //-----------------------------------------------------------------
    class __attribute__((packed)) cData
    {
      public:
        cData()
        {
          btn   = 0;
          X     = 0;
          Y     = 0;
          wheel = 0;
        }
      public:
        BYTE btn;
        BYTE X;
        BYTE Y;
        BYTE wheel;
    };

  public:
    cData dataIN;

    //---------------------------------------------------------------
    cMyInterf( USBdevice &usb,
               BYTE       configId,
               BYTE       interfId )
    : USBinterfClassHID( usb, configId, interfId, DataPointer(dataIN), DataPointer() )
    {
    	cnt = 0;
    }

    //---------------------------------------------------------------
    virtual bool onReportTransmit()
    {
      switch(cnt)
      {
        case 0:
          dataIN.btn = 1;
          dataIN.X   = 2;
          dataIN.Y   = 0;
          break;
        case 200:
          dataIN.X   = 0;
          dataIN.Y   = 2;
          break;
        case 400:
          dataIN.X   =(BYTE)-2;
          dataIN.Y   = 0;
          break;
        case 600:
          dataIN.X   =0;
          dataIN.Y   =(BYTE)-2;
          break;
        case 800:
          dataIN.btn = 0;
          dataIN.X   = 0;
          dataIN.Y   = 0;
          break;
      }

      if( cnt < 800 )
        cnt++;

      return( true );
    }

   //---------------------------------------------------------------
    virtual bool onReportReceive()
    {
      return( true );
    }

   //---------------------------------------------------------------
    bool isRunning()
    {
      return( cnt < 800 );
    }

}; //class cMyInterf


//*******************************************************************
int main(void)
{
  cMyInterf  interfaceHID( usb, 1/*configId*/, 0/*interfId*/ );
  usb.start();

  uart.set( "\r\n\nModUSBmouse," __DATE__ "," __TIME__ "\r\n\n" );

  uart.set( "\rwaiting ...." );
  while( !interfaceHID.isStarted ) {}

  uart.set( "\rrunning ...." );
  while( interfaceHID.isRunning() )  {}

  uart.set( "\rcompleted!   \n\r" );
  while( 1 )  {}
}

//EOF
