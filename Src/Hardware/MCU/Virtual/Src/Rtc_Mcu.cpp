//*******************************************************************
/*!
\file   Rtc_Mcu.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   20.07.2016

License: See file "LICENSE"
*/

//*******************************************************************
#include "Rtc_Mcu.h"
#include "Hardware/ReportID_Hw.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// Rtc_Mcu
//
//*******************************************************************
//-------------------------------------------------------------------
Rtc_Mcu::Rtc_Mcu( void )

: Rtc( ReportID_Hw::Module::RTC_MCU )

{
  offset = 0;
}

//-------------------------------------------------------------------
void Rtc_Mcu::set( const Rtc::Properties &clock )
{
  time_t    actTimeSec;
  time_t    newTimeSec;
  struct tm newTime;

  newTime.tm_hour  = clock.hour;
  newTime.tm_min   = clock.minute;
  newTime.tm_sec   = clock.second;
  newTime.tm_year  = clock.year%100 + 100*(clock.year/100-19);
  newTime.tm_mon   = clock.month-1;
  newTime.tm_mday  = clock.day;
  newTime.tm_wday  = 0;
  newTime.tm_isdst = 0;
  newTime.tm_yday  = 0;

  newTimeSec = mktime( &newTime );
  if( newTimeSec < 0 )
  {
    report.error( ReportID_Hw::Event::VALUE_OUT_OF_RANGE );
  }
  else
  {
    offset = newTimeSec - time( NULL );
  }
}

//-------------------------------------------------------------------
BYTE Rtc_Mcu::get( Rtc::Properties &clock )
{
  time_t rawtime;
  struct tm * ptm;

  time ( &rawtime );

  rawtime += offset;

  ptm = localtime ( &rawtime );

  clock.minute = ptm->tm_min;
  clock.hour   = ptm->tm_hour;
  clock.second = ptm->tm_sec;
  clock.dow    = (ptm->tm_wday==0)?7:ptm->tm_wday%8;
  clock.day    = ptm->tm_mday;
  clock.month  = ptm->tm_mon+1;
  clock.year   = ptm->tm_year+1900;

  return( true );
}

}  } //namespace

//EOF
