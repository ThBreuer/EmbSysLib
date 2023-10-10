//*******************************************************************
/*!
\file   System.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_System_H
#define _HW_System_H

//*******************************************************************
#include "Mcu_Types.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class System

\brief Low level system functions

This class has to been instantiate once.
*/
class System
{
  public:
    //---------------------------------------------------------------
    /*! Hardware initialization
        \param disableInterrupts
    */
    System( bool disableInterrupts = false );

    //---------------------------------------------------------------
    /*! Start Hardware
    */
    static void start( void );

    //---------------------------------------------------------------
    /*! Global interrupt disabling

        Each call to this function increments an internal counter,
        see \a enableInterrupt()
    */
    static void disableInterrupt( void );

    //---------------------------------------------------------------
    /*! Global interrupt enabling

        Decrements the internal counter, see \a disableInterrupt().
        The interrupts are enabled, if this counter reaches 0
    */
    static void enableInterrupt( void );

    //---------------------------------------------------------------
    /*! \todo add comment
        
    */
    static void enterISR( void );

    //---------------------------------------------------------------
    /*! \todo add comment
       
    */
    static void leaveISR( void );

    //---------------------------------------------------------------
    /*! Enable system's watchdog
        \param resetTime (ms)
    */
    static void enableWatchdog( WORD resetTime );

    //---------------------------------------------------------------
    /*! Disable system's watchdog
    */
    static void disableWatchdog( void );

    //---------------------------------------------------------------
    /*! Feed the watchdog
    */
    static void feedWatchdog( void );

    //---------------------------------------------------------------
    /*! Controller reset
    */
    static void reset( void );

    //---------------------------------------------------------------
    /*! Delays programm execution
        \param delay time in µs (micro seconds)
        \warning No exact timing!
    */
    static void delayMicroSec( unsigned short delay );

    //---------------------------------------------------------------
    /*! Delays programm execution
        \param delay time in ms (milli seconds)
        \warning No exact timing!
    */
    static void delayMilliSec( unsigned short delay );

  private:
    //---------------------------------------------------------------
    static unsigned char cntInterrupt;

}; // System

}  } //namespace

//*******************************************************************
/*!
\brief Low level system initialization
*/
extern "C"
{
  void SystemInit(void);
}

#endif
