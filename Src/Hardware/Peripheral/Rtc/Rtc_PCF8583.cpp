//*******************************************************************
/*!
\file   Rtc_PCF8583.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   29.12.2022

License: See file "LICENSE"
*/
 
//*******************************************************************
#include "Rtc_PCF8583.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// Rtc_PCF8583
//
//*******************************************************************
//-------------------------------------------------------------------
Rtc_PCF8583::Rtc_PCF8583( I2Cmaster &i2cIn, BYTE hwAddrIn )

: Rtc( ReportID_Hw::Module::RTC_PERIPHERAL )

, i2c( i2cIn, baseAdr | ((hwAddrIn&maskAdr)<<1) )

{
  errorFlag = false;
}

//-------------------------------------------------------------------
void Rtc_PCF8583::set( const Rtc::Properties &clock )
{
  BYTE dow = clock.dow;

  if( dow == 0 )
  {
    dow = getDayOfWeek( clock.day, clock.month, clock.year );
  }

  // Konfiguration
  i2c.write(  (BYTE)0x00, // Control/status register
              (BYTE)(  (0x00<<7)    // stop flag: count pulses
                      |(0x00<<6)    // hold flag: count
                      |(0x00<<4)    // fuction mode: clock mode 32.768kHz
                      |(0x00<<3)    // mask flag: read locations 05 to 06 unmasked
                      |(0x00<<2)    // alarm enable: alarm disabled
                      |(0x00<<1)    // alarm flag: ---
                      |(0x00<<0))); // timer flag: ---

  Register reg = 
  {
    (BYTE)(Dual_2_BCD( clock.second)),
    (BYTE)(Dual_2_BCD( clock.minute)),
    (BYTE)(Dual_2_BCD( clock.hour  ) | (0x00<<6)   // AM
                                     | (0x00<<7)),// 24h-format
    (BYTE)(Dual_2_BCD( clock.day   ) | (BYTE)((clock.year%4)<<6)),
    (BYTE)(Dual_2_BCD( clock.month ) | (BYTE)((dow-1)<<5)        )
  };

  i2c.write(  (BYTE)0x02, &reg );
}

//-------------------------------------------------------------------
BYTE Rtc_PCF8583::get( Rtc::Properties &clock )
{
  Register reg;
  
  i2c.read( (BYTE)0x02, &reg );
  
  clock.second = BCD_2_Dual( reg.seconds              );
  clock.minute = BCD_2_Dual( reg.minutes              );
  clock.hour   = BCD_2_Dual( reg.hours         & 0x3F );
  clock.day    = BCD_2_Dual( reg.year_date     & 0x3F );
  clock.year   = BCD_2_Dual( reg.year_date     >> 6   );
  clock.month  = BCD_2_Dual( reg.weekday_month & 0x1F );
  clock.dow    = BCD_2_Dual( reg.weekday_month >> 5   )+1;
  
  errorFlag = i2c.isError();
  
  return( errorFlag );
}

//-------------------------------------------------------------------
inline BYTE Rtc_PCF8583::BCD_2_Dual( const BYTE bcd )
{
  return( 10*((bcd>>4)&0x0F) + (bcd&0x0F) );
}

//-------------------------------------------------------------------
inline BYTE Rtc_PCF8583::Dual_2_BCD( const BYTE dual )
{
  return( ((dual/10)<<4) | (dual%10) );
}

}  } //namespace

//EOF
