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

\remark Pay attention:
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
    /*! Provides runtime since last start of the timer
    
        \return runtime in microseconds (us)
    */
    LWORD get( void );

    //---------------------------------------------------------------
    /*! Stop the timer
        \remark The next call to \a timeout() does not indicate a timeout
    */
    void stop( void );

    //---------------------------------------------------------------
    /*! Check trigger\n

        This method checks if the timer has expired. If so, the timer
        will be \b restarted.
        \return trigger event:
                 - true: A timeout occurs
                 - false: Neither a timeout event nor the timer is 
                          running
    */
    bool trigger( void );

    //---------------------------------------------------------------
    /*! Check timeout\n

        This method checks if the timer has expired. If so, the timer
        will be \b stopped.
        \return timer event:
                 - true: A timeout occurs
                 - false: Neither a timeout event nor the timer is 
                          running
    */
    bool timeout( void );

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
