//*******************************************************************
/*!
\file   Rtc_PCF8583.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   29.12.2022

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_RTC_PCF8583_H
#define _HW_RTC_PCF8583_H

//*******************************************************************
#include "Hardware/Common/Rtc/Rtc.h"
#include "Hardware/Common/I2C/I2Cmaster.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class Rtc_PCF8583

\brief Implementation of the real time clock (RTC) PCF8583

\example HwRtc.cpp
*/
class Rtc_PCF8583 : public Rtc
{
  private:
    //---------------------------------------------------------------
    /* Data structure of PCF8583 registers
    */
    class Register
    {
      public:
        BYTE seconds;
        BYTE minutes;
        BYTE hours;
        BYTE year_date;
        BYTE weekday_month;
    };

  public:
    //---------------------------------------------------------------
    /*! Initialize PCF8574, part RTC
        \param i2c    Reference to I2C interface
        \param hwAddr Sub address of PCF8574 (address pin A0)
    */
    Rtc_PCF8583( I2Cmaster &i2c, BYTE hwAddr );

    //---------------------------------------------------------------
    virtual void set( const Rtc::Properties &clock );

    //---------------------------------------------------------------
    virtual BYTE get( Rtc::Properties &clock );

    //---------------------------------------------------------------
    bool isError( void )
    {
      
      return( errorFlag.getEvent() || i2c.isError() );
    }

  private:
    //---------------------------------------------------------------
    // Convert BCD coded number into dual coded number
    BYTE BCD_2_Dual( BYTE bcd );

    //---------------------------------------------------------------
    // Convert dual coded number into BCD coded number
    BYTE Dual_2_BCD( BYTE dual );

    //----------------------------------------
    I2Cmaster::Device i2c;
  
    Flag<bool> errorFlag;

    static const BYTE baseAdr = 0xA0;
    static const BYTE maskAdr = 0x01;

}; //class Rtc_PCF8583

}  } //namespace

#endif
