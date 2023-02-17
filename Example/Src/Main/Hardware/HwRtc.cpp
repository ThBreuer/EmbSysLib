//*******************************************************************
/*!
\file   HwRtc.cpp
\author Thomas Breuer
\date   10.08.2022
\brief  Sample of using hardware related class 'RTC'
*/

//*******************************************************************
/*
Usage:    Real Time Clock
          Date and time are read and displayed continuously.
          Press any key on terminal to set date and time of the RTC.
*/

//*******************************************************************
#include <stdio.h>
#include "EmbSysLib.h"

//-------------------------------------------------------------------
#include "ReportHandler.h"
#include "configHw.h"

//*******************************************************************
int main(void)
{
  uart.set( "\r\n\nHwRtc," __DATE__ "," __TIME__ "\r\n\n" );

  while( 1 )
  {
    Rtc::Properties prop;

    if( uart.get() )
    {
      prop.year   = 2021;
      prop.month  =   12;
      prop.day    =   31;
      prop.hour   =   23;
      prop.minute =   59;
      prop.second =   50;

      rtc.set( prop );
    }

    rtc.get( prop );

    const char *day[8] = {"--","Mo","Di","Mi","Do","Fr","Sa","So"};
    char  str[40];
    sprintf( str, "RTC: %s %02u.%02u.%04u %02u:%02u:%02u\r",
                  day[prop.dow],
                  prop.day,
                  prop.month,
                  prop.year,
                  prop.hour,
                  prop.minute,
                  prop.second );
    uart.set( str );
  }
}
