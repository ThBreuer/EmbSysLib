//*******************************************************************
/*!
\file   Rtc_Mcu.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   16.10.2022

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_RTC_MCU_H
#define _HW_RTC_MCU_H

//*******************************************************************
#include "Hardware/Common/Rtc/Rtc.h"

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
    /*! \enum RTC_ClockConfig_t
        \brief RTC configuration
    */
    typedef enum
    {
      LSI = 0,  //!< LSI as clock source
      LSE       //!< LSE as clock source
    } RTC_ClockConfig_t;
	
    //---------------------------------------------------------------
    /*! Initialize the RTC hardware    
        \param config RTC configuration (clock source)
    */
    Rtc_Mcu( RTC_ClockConfig_t config );

    //---------------------------------------------------------------
    virtual void set( const Rtc::Properties &clock );

    //---------------------------------------------------------------
    virtual BYTE get( Rtc::Properties &clock );

}; //class Rtc_Mcu

}  } //namespace

#endif
