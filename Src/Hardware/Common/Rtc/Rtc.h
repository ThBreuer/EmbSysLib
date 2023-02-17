//*******************************************************************
/*!
\file   Rtc.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_RTC_H
#define _HW_RTC_H

//*******************************************************************
#include "Mcu_Types.h"
#include "Std/Report.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class Rtc

\brief Abstract class supporting a real time clock

This base class is an interface to a RTC.
\remark When setting or getting, date and time are not checked
\remark Depending on hardware, the value of \a year is maybe limited

Derived classes should at least:
 - initialize the hardware inteface to the RTC
 - implement the methods set() and get()

\example HwRtc.cpp
*/
class Rtc
{
  public:
    //---------------------------------------------------------------
    /*! Data structure of real time clock (RTC Properties)
    */
    class Properties
    {
      public:
        WORD year;   //!< year         (0,..   )
        BYTE month;  //!< month        (1,..,12)
        BYTE day;    //!< day of month (1,..,31)
        BYTE dow;    //!< day of week  (1,.., 7)  0: not defined
        BYTE hour;   //!< hour         (0,..,23)
        BYTE minute; //!< minute       (0,..,59)
        BYTE second; //!< second       (0,..,59)
    };

  protected:
    //---------------------------------------------------------------
    // Standard constructor
    //
    Rtc( WORD moduleId );

    // Zeller's congruence for the Gregorian calendar
    // 1: Monday,...,7=Sunday
    //
    // \see Torsten.Sillke@uni-bielefeld.de    1999-03-15
    //
    static BYTE getDayOfWeek( BYTE day, BYTE month, WORD year )
    {
       if (month < 3)
       {
         month += 12;
         year--;
       }
       return( ( ( (13*month+3)/5 + day + year + year/4 - year/100 + year/400) % 7) + 1 );
    }

  public:
    //---------------------------------------------------------------
    /*! Set actual date and time \n
        Date and time stored in \a clock are written to the RTC
        \param clock Reference to cRtcprop
    */
    virtual void set( const Rtc::Properties &clock ) = 0;

    //---------------------------------------------------------------
    /*! Get actual date and time \n
        Date and time are read from RTC and returned in \a clock
        \param clock Reference to cRtcprop
        \return
                - true:  OK, date and time are valid
                - false: Error, date and time are not valid
    */
    virtual BYTE get( Rtc::Properties &clock ) = 0;

  public:
    //---------------------------------------------------------------
    Std::Report report;

}; //class Rtc

}  } //namespace

#endif
