//*******************************************************************
/*!
\file   Adc.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_ADC_H
#define _HW_ADC_H

//*******************************************************************
#include "Hardware/Common/Timer/Timer.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class Adc

\brief Abstract class supporting the analog to digital converter (ADC)

This class is an interface to the ADC hardware.
All registered channels (see \a addChannel()) are scanned after
calling the update() method. The sampled analog values are then
stored in a buffer and can be obtained by get() \b after the update()
method is called next time.\n
In other words: An ongoing conversion does not affect the values given
by get(), until the conversion is ready.\n
A timer object (see: Timer) is used to trigger the conversion
automatically.\n
Multiple channels are supported (limited by hardware).\n
Independently of the resolution of the used ADC, a 16 bit value
is delivered

Derived classes should at least:
 - initialize the ADC hardware
 - implement getResult(), startCh() and configCh() to access the
   hardware
 - call \a isr() in the appropriate ISR

\example HwAdc.cpp
*/
class Adc : protected Timer::Task
{
  protected:
    //---------------------------------------------------------------
    // Initialize the ADC and allocate buffer memory
    // moduleId:     Id of this module, used for report
    // num:          Number of supported channels
    // timer:        Referenz to a timer object
    //
    Adc( WORD moduleId, BYTE num, Timer &timer );

  public:
    //---------------------------------------------------------------
    /*! Enable a channel by adding to the scan list
        \param ch  Channel ID
        \param para Additional configuration parameter, HW specific
    */
    virtual void enable( BYTE ch, BYTE para = 0 );

    //---------------------------------------------------------------
    /*! Get the last sample of a channel
        \param ch  Channel ID
        \return    Sample value
    */
    virtual WORD get( BYTE ch );

    //---------------------------------------------------------------
    /*! Start a scan. This function returns immediately and does not
        wait for completition of the scan
    */
    virtual void update( void );

  public:
    //---------------------------------------------------------------
    // Interrupt Service Routine of an ADC interrupt
    // Stores the sample in the buffer and starts sampling of next
    // channel if necessary
    //
    virtual void isr( void );

  protected:
    //---------------------------------------------------------------
    // Get sample value from ADC hardware
    // return: Sample value
    //
    virtual WORD getResult( void ) = 0;

    //---------------------------------------------------------------
    // Start sampling
    // ch:  Channels number
    //
    virtual void startCh( BYTE ch ) = 0;

    //---------------------------------------------------------------
    // Configure the channel secific adc hardware
    // ch:  Channels number
    //
    virtual void configCh( BYTE ch, BYTE para = 0 ) = 0;

  protected:
    //---------------------------------------------------------------
    WORD *result;         // sample buffer
    BYTE *chList;         // list of registered chanels
    BYTE  numOfChannels;  // number of supported channels
    BYTE  numActivatedCh; // number or registered channels
    BYTE  currentCh;      // actual scanned channel
    BYTE  currentId;      // index to chList of actual scanned channel
    bool  isRunning;      // 0: scan is not running, 1: scan is running

  public:
    //---------------------------------------------------------------
    Std::Report report;

}; //class Adc

} } //namespace

#endif
