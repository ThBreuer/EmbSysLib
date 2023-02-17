//*******************************************************************
/*!
\file   Rtc_Mcu.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"
*/

//*******************************************************************
#include "Mcu_Types.h"
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
  PWR->CR  |= PWR_CR_DBP;
  RCC->CSR &= ~(0x03<<16);   // reset RTCSEL
  RCC->CSR |=  RCC_CSR_RTCEN // RTC enable
              |(0x03<< 16);  // HSE select

  // unlock write protection:
  RTC->WPR = 0xCA;
  RTC->WPR = 0x53;

  // initialization
  RTC->ISR |= RTC_ISR_INIT;

  while( !(RTC->ISR & RTC_ISR_INITF) );

  RTC->PRER  = ((MTYPE)(32000-1)<< 0); // sychronous
  RTC->PRER |= ((MTYPE)(  125-1)<<16); // asynchronous

  // exit initialization
  RTC->ISR &= ~RTC_ISR_INIT;
}

//-------------------------------------------------------------------
void Rtc_Mcu::set( const Rtc::Properties &clock )
{
  // unlock write protection:
  RTC->WPR = 0xCA;
  RTC->WPR = 0x53;

  // initialization
  RTC->ISR |= RTC_ISR_INIT;

  while( !(RTC->ISR & RTC_ISR_INITF) );

  WORD year = clock.year%100;

  RTC->TR =  ((DWORD)((clock.second%10)&0xF) << 0)
            |((DWORD)((clock.second/10)&0x7) << 4)
            |((DWORD)((clock.minute%10)&0xF) << 8)
            |((DWORD)((clock.minute/10)&0x7) <<12)
            |((DWORD)((clock.hour  %10)&0xF) <<16)
            |((DWORD)((clock.hour  /10)&0x3) <<20);

  RTC->DR =  ((DWORD)((clock.day  %10)&0xF) << 0)
            |((DWORD)((clock.day  /10)&0x3) << 4)
            |((DWORD)((clock.month%10)&0xF) << 8)
            |((DWORD)((clock.month/10)&0x1) <<12)
            |((DWORD)((      year %10)&0xF) <<16)
            |((DWORD)((      year /10)&0xF) <<20);

  // exit initialization
  RTC->ISR &= ~RTC_ISR_INIT;
}

//-------------------------------------------------------------------
BYTE Rtc_Mcu::get( Rtc::Properties &clock )
{
  BYTE sec;

  clock.second = (BYTE)(((RTC->TR >> 0)&0xF) + 10*((RTC->TR >> 4)&0x7));
  clock.minute = (BYTE)(((RTC->TR >> 8)&0xF) + 10*((RTC->TR >>12)&0x7));
  clock.hour   = (BYTE)(((RTC->TR >>16)&0xF) + 10*((RTC->TR >>20)&0x3));

  clock.day    = (BYTE)(((RTC->DR >> 0)&0xF) + 10*((RTC->DR >> 4)&0x3));
  clock.month  = (BYTE)(((RTC->DR >> 8)&0xF) + 10*((RTC->DR >>12)&0x1));
  clock.year   = (BYTE)(((RTC->DR >>16)&0xF) + 10*((RTC->DR >>20)&0xF));

  clock.year += 2000;
  clock.dow    = getDayOfWeek( clock.day, clock.month, clock.year );

  // reread sec to check overflow
  sec          = (BYTE)(((RTC->TR >> 0)&0xF) + 10*((RTC->TR >> 4)&0x7));

  return( (clock.second <= sec) ? true : false );
}

}  } //namespace

//EOF
