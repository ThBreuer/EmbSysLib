//*******************************************************************
/*!
\file   Rtc_Mcu.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_RTC_MCU_H
#define _HW_RTC_MCU_H

//*******************************************************************
#include "Hardware/Common/Rtc/Rtc.h"
#include <time.h>

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class Rtc_Mcu

\brief Implementation of the real time clock (RTC)

\example HwRtc.cpp
*/
class Rtc_Mcu : public Rtc
{
  public:
    //---------------------------------------------------------------
    /*! Initialize the RTC hardware
    */
    Rtc_Mcu( void );

    //---------------------------------------------------------------
    virtual void set( const Rtc::Properties &clock );

    //---------------------------------------------------------------
    virtual BYTE get( Rtc::Properties &clock );

  private:
    //---------------------------------------------------------------
    time_t  offset;

}; //class Rtc_Mcu

}  } //namespace

#endif
