//*******************************************************************
/*!
\file   Rtc_Mcu.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   16.10.2022

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
Rtc_Mcu::Rtc_Mcu( RTC_ClockConfig_t config )
: Rtc( ReportID_Hw::Module::RTC_MCU )
{
  if( config == LSI )
  {
    RCC->CSR |= RCC_CSR_LSION;
    while( !(RCC->CSR & RCC_CSR_LSIRDY) );
    
    // Initialize only, if LSI is NOT selected
    if( ((RCC->BDCR >> 8/*RCC_BDCR_RTCSEL_Pos*/) & 0x03) != 2 ) 
    {
      PWR->CR |= PWR_CR_DBP; // Disable write protection

      // BDCR reset
      RCC->BDCR |=  RCC_BDCR_BDRST; 
      RCC->BDCR &= ~RCC_BDCR_BDRST; 
      
      RCC->CSR |= RCC_CSR_LSION;
      
      while( !(RCC->CSR & RCC_CSR_LSIRDY) );
      
      // RTC clock selection
      RCC->BDCR &= ~(0x03<<8); // reset RTCSEL
      RCC->BDCR |=  (0x02<<8); // LSI select
      RCC->BDCR |=  RCC_BDCR_RTCEN;
      PWR->CR &=  ~PWR_CR_DBP; // Enble write protection

    }
  }
  else if( config == LSE )
  {
    // Initialize only, if LSE is NOT selected
    if( ((RCC->BDCR >> 8/*RCC_BDCR_RTCSEL_Pos*/) & 0x03) != 1 ) 
    {
      PWR->CR |= PWR_CR_DBP; // Disable write protection
     
      // BDCR reset
      RCC->BDCR |=  RCC_BDCR_BDRST; 
      RCC->BDCR &= ~RCC_BDCR_BDRST; 

      RCC->BDCR |= RCC_BDCR_LSEON;
      
      while( !(RCC->BDCR & RCC_BDCR_LSERDY) );
      
      // RTC clock selection
      RCC->BDCR &= ~(0x03<<8); // reset RTCSEL
      RCC->BDCR |=  (0x01<<8); // LSE select
      RCC->BDCR |=  RCC_BDCR_RTCEN;
      PWR->CR &=  ~PWR_CR_DBP; // Enble write protection
    }
  }  
}

//-------------------------------------------------------------------
void Rtc_Mcu::set( const Rtc::Properties &clock )
{
  BYTE dow;

  // unlock write protection:
  PWR->CR |= PWR_CR_DBP;

  RTC->WPR = 0xCA;
  RTC->WPR = 0x53;

  // initialization
  RTC->ISR |= RTC_ISR_INIT;

  while( !(RTC->ISR & RTC_ISR_INITF) );

  if( clock.dow == 0 )
  {
    dow = getDayOfWeek( clock.day, clock.month, clock.year );
  }
  else
  {
    dow = clock.dow;
  }

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
            |((DWORD)((dow  % 8)&0x7) <<13)
            |((DWORD)((year %10)&0xF) <<16)
            |((DWORD)((year /10)&0xF) <<20);

  // exit initialization
  RTC->ISR &= ~RTC_ISR_INIT;

  PWR->CR &= ~PWR_CR_DBP; // Enable write protection
}

//-------------------------------------------------------------------
BYTE Rtc_Mcu::get( Rtc::Properties &clock )
{
  BYTE sec;

  clock.second = (BYTE)(((RTC->TR >> 0)&0xF) + 10*((RTC->TR >> 4)&0x7));
  clock.minute = (BYTE)(((RTC->TR >> 8)&0xF) + 10*((RTC->TR >>12)&0x7));
  clock.hour   = (BYTE)(((RTC->TR >>16)&0xF) + 10*((RTC->TR >>20)&0x3));

  clock.dow    = (BYTE)(((RTC->DR >>13)&0x7)                          );

  clock.day    = (BYTE)(((RTC->DR >> 0)&0xF) + 10*((RTC->DR >> 4)&0x3));
  clock.month  = (BYTE)(((RTC->DR >> 8)&0xF) + 10*((RTC->DR >>12)&0x1));
  clock.year   = (BYTE)(((RTC->DR >>16)&0xF) + 10*((RTC->DR >>20)&0xF));

  // reread sec to check overflow
  sec          = (BYTE)(((RTC->TR >> 0)&0xF) + 10*((RTC->TR >> 4)&0x7));

  clock.year += 2000;

  return( (clock.second <= sec) ? true : false );
}

}  } //namespace

//EOF
