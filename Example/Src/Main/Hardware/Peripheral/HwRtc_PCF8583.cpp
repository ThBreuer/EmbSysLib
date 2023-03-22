//*******************************************************************
/*!
\file   HwRtc_PCF8583.cpp
\author Thomas Breuer
\date   20.02.2023
\brief  Sample of using hardware related classes
*/

//*******************************************************************
/*
Usage:    connect board with terminal.

          press 'r' to read date and time from PCF8583 real time clock.
          press 'w' to set date and time to predefinded value, this action is confirmed with "ready".
					
          PCF8583 only saves 2 bit for current year (0,1,2,3 valid)
*/

//*******************************************************************
#include <stdio.h>
#include "EmbSysLib.h"

//-------------------------------------------------------------------
#include "ReportHandler.h"
#include "configHw.h"

using namespace EmbSysLib::Dev;

Terminal   terminal( uart, 255,255, "erw" );

//================================================
#include "Hardware/Peripheral/Rtc/Rtc_PCF8583.cpp"

Rtc_PCF8583  rtcPCF8583( i2cBus, 0 /*sub address*/ );
//================================================

//*******************************************************************
int main(void)
{
  terminal.printf( "\r\n\nHwRtc_PCF8583," __DATE__ "," __TIME__ "\r\n\n" );

  const char *day[8] = {"--","Mo","Di","Mi","Do","Fr","Sa","So"};

  while( 1 )
  {
    Rtc::Properties prop;

    switch( terminal.get() )
    {
      case 'r':
        terminal.printf( "\r\nread: ");
        rtcPCF8583.get( prop );
        terminal.printf( "RTC: %s %02u.%02u.%04u %02u:%02u:%02u\r\n",
                          day[prop.dow],
                          prop.day,
                          prop.month,
                          prop.year,
                          prop.hour,
                          prop.minute,
                          prop.second );
        break;
        
      case 'w':
        terminal.printf( "\r\nwrite: ");
        prop.year   = 2021;
        prop.month  =   12;
        prop.day    =   31;
        prop.hour   =   23;
        prop.minute =   59;
        prop.second =   50;

        rtcPCF8583.set( prop );
        terminal.printf( "ready\r\n");
        break;
    }
    
    if( rtcPCF8583.isError() )
    {
      terminal.printf( "\r\nRTC access error\r\n" );
    }
  }
}
