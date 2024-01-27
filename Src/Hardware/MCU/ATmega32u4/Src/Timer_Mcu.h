//*******************************************************************
/*!
\file   Timer_Mcu.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   14.02.2011

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_TIMER_MCU_H
#define _HW_TIMER_MCU_H

//*******************************************************************
#include "Hardware/Common/Timer/Timer.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class Timer_Mcu

\brief Specialized interface class for ATmega hardware timer

\see Timer_0 etc.
\example HwTimer.cpp
*/
class Timer_Mcu : public Timer
{
  public:
    //---------------------------------------------------------------
    /*! Timer mode selection
    */
    typedef enum
    {
      INTERRUPT     = 0x00, //!< Timer in normal mode, PWM not supported
      PWM           = 0x01, //!< Timer in PWM-Mode, no interrupt
      PWM_INTERRUPT = 0x02  //!< Timer in PWM-Mode, with interrupt
    } PWM_MODE;

    //-------------------------------------------------------------------
    void isr(void)
    {
      isrHandler();
    }

  protected:
    //---------------------------------------------------------------
    // Constructor
    // mode: Timer mode
    //
    Timer_Mcu( PWM_MODE mode );

    //---------------------------------------------------------------
    // Calculate prescale settings used in
    // Timer/Counter Control Register (TCCR)
    // cycleTime:  Timer cycle time (unit: us)
    // prescale[]: Array containing prescale factors
    // size:       Number of prescale factors
    // maxTics:    Maximum timer tics
    //
    BYTE getClockSelect( const WORD cycleTime,
                         const BYTE prescale[],
                         const BYTE size,
                         const WORD maxTics );

  protected:
    //---------------------------------------------------------------
    PWM_MODE mode;  // PWM or normal mode
    WORD     tics;  // Timer tics per cycle
    BYTE     com;

}; //Timer_Mcu

//*******************************************************************
/*!
\class Timer_0

\brief Implementation of 8-bit Timer0

\example Timer.cpp
*/
class Timer_0 : public Timer_Mcu
{
  public:
    //---------------------------------------------------------------
    /*! Channel selection
    */
    typedef enum
    {
      OC0A     = 0, //!< PB7
      OC0B     = 1, //!< PD0
    } Channel;

  public:
    //---------------------------------------------------------------
    /*! Initialize Timer0\n

        In normal mode the cycle time is adapted to \n

        \code  cycle time = N * (prescale factor)/fosc  with N=1,..,256 \endcode

        In PWM mode the cycle time is adapted to \n

        \code  cycle time = 256 * (prescale factor)/fosc \endcode

        Example:
        \code
        PWM mode, fosc = 16MHz

        cycleTime   | prescale factor ||  cycle time
        (parameter) |                 ||  (real)
        ------------+-----------------++------------
           1 -   16 |        1        ||    16
          17 -  128 |        8        ||   128
         129 - 1024 |       64        ||  1024
        1025 - 4096 |      256        ||  4096
        4097 - max  |     1024        || 16384
        \endcode

        \param cycleTime Cycle Time of Timer0 (unit: us)
        \param mode PWM/normal mode selection, default: normal mode
    */
    Timer_0( WORD cycleTime, PWM_MODE mode = INTERRUPT );

    //---------------------------------------------------------------
    virtual void enablePWM( BYTE ch = 0, Mode pol = NORMAL );

    //---------------------------------------------------------------
    virtual void setPWM( BYTE ch, WORD value );

  public:
    //----------------------------------------
    static  Timer_Mcu *hwTimerPtr;

}; //class Timer_0

//*******************************************************************
/*!
\class Timer_1

\brief Implementation of 16-bit Timer1

\example Timer.cpp
*/
class Timer_1 : public Timer_Mcu
{
  public:
    //---------------------------------------------------------------
    /*! Channel selection
    */
    typedef enum
    {
      OC1A     = 0, //!< PB5
      OC1B     = 1, //!< PB6
      OC1C     = 2, //!< PB7
    } Channel;

  public:
    //---------------------------------------------------------------
    /*! Initialize Timer1

        In normal mode the cycle time is adapted to \n

        \code  cycle time = N * (prescale factor)/fosc  with N=1,..,65536 \endcode

        In PWM mode (10-bit) the cycle time is adapted to \n

        \code  cycle time = 1024 * (prescale factor)/fosc \endcode

        Example:
        \code
        PWM mode, fosc = 16MHz

        cycleTime     | prescale factor ||  cycle time
        (parameter)   |                 ||  (real)
        --------------+-----------------++------------
            1 -    64 |        1        ||    64
           65 -   512 |        8        ||   512
          513 -  4096 |       64        ||  4096
         4097 - 16383 |      256        || 16384
        16385 - max   |     1024        || 65536
        \endcode

        \param cycleTime Cycle Time of Timer1 (unit: us)
        \param mode PWM/normal mode selection, default: normal mode
    */
    Timer_1( WORD cycleTime, PWM_MODE mode = INTERRUPT );

    //---------------------------------------------------------------
    virtual void enablePWM( BYTE ch = 0, Mode pol = NORMAL );

    //---------------------------------------------------------------
    virtual void setPWM( BYTE ch, WORD value );

  public:
    //----------------------------------------
    static       Timer_Mcu *hwTimerPtr;

}; //class Timer_1

//*******************************************************************
/*!
\class Timer_3

\brief Implementation of 16-bit Timer3

\example Timer.cpp
*/
#if defined _USE_RTOS && _USE_RTOS==true
//  #pragma warning "Timer 3 not available"
#else
class Timer_3 : public Timer_Mcu
{
  public:
    //---------------------------------------------------------------
    /*! Channel selection
    */
    typedef enum
    {
      OC3A     = 0, //!< PC6
    } Channel;

  public:
    //---------------------------------------------------------------
    /*! Initialize Timer3

        In normal mode the cycle time is adapted to \n

        \code  cycle time = N * (prescale factor)/fosc  with N=1,..,65536 \endcode

        In PWM mode the cycle time is adapted to \n

        \code  cycle time = 1024 * (prescale factor)/fosc \endcode

        Example:
        \code
        PWM mode, fosc = 16MHz

        cycleTime     | prescale factor ||  cycle time
        (parameter)   |                 ||  (real)
        --------------+-----------------++------------
            1 -    64 |        1        ||    64
           65 -   512 |        8        ||   512
          513 -  4096 |       64        ||  4096
         4097 - 16383 |      256        || 16384
        16385 - max   |     1024        || 65536
        \endcode

        \param cycleTime Cycle Time of Timer2 (unit: us)
        \param mode PWM/normal mode selection, default: normal mode
    */
    Timer_3( WORD cycleTime, PWM_MODE mode = INTERRUPT );

    //---------------------------------------------------------------
    virtual void enablePWM( BYTE ch = 0, Mode pol = NORMAL );

    //---------------------------------------------------------------
    virtual void setPWM( BYTE ch, WORD value );

  public:
    //----------------------------------------
    static       Timer_Mcu *hwTimerPtr;

}; //class Timer_3
#endif

//*******************************************************************
/*!
\class Timer_4

\brief Implementation of 10-bit Timer4

For simplicity, however, the timer is implemented here only as an 8-bit timer.

\example Timer.cpp
*/
class Timer_4 : public Timer_Mcu
{
  public:
    //---------------------------------------------------------------
    /*! Channel selection
    */
    typedef enum
    {
      OC4A     = 0, //!< PC6
      OC4B     = 1, //!< PB6
      OC4D     = 3, //!< PD7
    } Channel;

  public:
    //---------------------------------------------------------------
    /*! Initialize Timer4

        In normal mode the cycle time is adapted to \n

        \code  cycle time = N * (prescale factor)/fosc  with N=1,..,256 \endcode

        In PWM mode the cycle time is adapted to \n

        \code  cycle time = 256 * (prescale factor)/fosc \endcode

        Example:
        \code
        PWM mode, fosc = 16MHz

        cycleTime     | prescale factor ||  cycle time
        (parameter)   |                 ||  (real)
        --------------+-----------------++------------
            1 -    16 |        1        ||    16
           17 -    32 |        2        ||    32
           33 -    64 |        4        ||    64
           65 -   128 |        8        ||   128
          129 -   256 |       16        ||   256
          257 -   512 |       32        ||   512
          513 -  1024 |       64        ||  1024
         1023 -  2048 |      128        ||  2048
         2049 -  4096 |      256        ||  4096
         4097 -  8192 |      512        ||  8192
         8193 - 16384 |     1024        || 16384
        16385 - 32768 |     2048        || 32768
        32769 - 65536 |     4096        || 65536
        (cycle time > 65536us not supported by software)
        \endcode

        \param cycleTime Cycle Time of Timer2 (unit: us)
        \param mode PWM/normal mode selection, default: normal mode
    */
    Timer_4( WORD cycleTime, PWM_MODE mode = INTERRUPT );

    //---------------------------------------------------------------
    virtual void enablePWM( BYTE ch = 0, Mode pol = NORMAL );

    //---------------------------------------------------------------
    virtual void setPWM( BYTE ch, WORD value );

  public:
    //----------------------------------------
    static       Timer_Mcu *hwTimerPtr;

}; //class Timer_4

}  } //namespace

#endif
