//*******************************************************************
/*!
\file   Clock.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   27.01.2024

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _STD_CLOCK_H
#define _STD_CLOCK_H

//*******************************************************************
namespace EmbSysLib {
namespace Std {

//*******************************************************************
/*!
\class Clock

\brief Clock features within a task

The clock uses the timer tics, which are provided by a task object.

Pay attention:
Waiting for timeout like
'while( !timeout() );'
doesn't work within a Timer:Task!
*/
class Clock
{
  public:
    //---------------------------------------------------------------
    /*! Instantiate a Clock

        Zero run time
    */
    Clock( void );

    //---------------------------------------------------------------
    /*! Starts the time, regardless if the clock is running or not
        \param timeToWait_msec Runtime in miliseconds (ms)
        \param timeToWait_usec Runtime in microseconds (us)
    */
    void start( DWORD timeToWait_msec,
                WORD  timeToWait_usec = 0 );

    //---------------------------------------------------------------
    /*! \see \a start(), but reuse the stored runtime
    */
    void start( void );

    //---------------------------------------------------------------
    /*! Check timeout\n

        The behaviour depends on the run state of the timer:
        - Timer is running: If an timeout occures, the timer will be
                            stopped. All parameters are ignored
        - Timer is NOT running: The timer will be restarted with given
                                parameters
        \param timeToWait_msec Timer-Laufzeit (ms)
        \param timeToWait_usec Timer-Laufzeit (us)
        \return timer state:
                 - true: timeout
                 - false: timer is still running
    */
//    BYTE timeout( WORD timeToWait_msec,
//                  WORD timeToWait_usec = 0 );

    //--------------------------------------------------------------
    // todo review comment
    /*! \see \a timeout(), but reuse the stored runtime
        \return timer state:
                 - true: timeout
                 - false: timer is still running
    */
    bool trigger( void );

    //---------------------------------------------------------------
    // todo add comment
    /*!
    */
    bool timeout( void );

    //---------------------------------------------------------------
    /*! Stop the timer
        \remark The next call to \a timeout() does not indicate a timeout
    */
    void stop( void );

    // todo comment
    LWORD get( void );

  protected:
    //---------------------------------------------------------------
    // Set runtime, but don't start the timer
    // timeToWait_msec: Runtime in miliseconds (ms)
    // timeToWait_usec: Runtime in microseconds (us)
    LWORD toTics( DWORD timeToWait_msec,
                  WORD  timeToWait_usec = 0 );

  protected:
    //---------------------------------------------------------------
    virtual LWORD getTics() = 0;

    //---------------------------------------------------------------
    virtual DWORD getTimeOfTic() = 0;

  protected:
    //---------------------------------------------------------------
    LWORD startTime;
    LWORD timeToGo;
    BYTE  running;

}; //class Clock

}  } //namespace

#endif
