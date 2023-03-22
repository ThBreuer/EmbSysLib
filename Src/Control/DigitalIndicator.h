//*******************************************************************
/*!
\file   DigitalIndicator.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   26.01.2023

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _CTRL_DIGITAL_INDICATOR_H
#define _CTRL_DIGITAL_INDICATOR_H

//*******************************************************************
#include "Device/Digital/Digital.h"
#include "Device/Task/TaskManager.h"

//*******************************************************************
namespace EmbSysLib {
namespace Ctrl {

//*******************************************************************
/*!
\class   DigitalIndicator

\brief   Generates transient and periodic outputs, e.g. as a led signal

\example CtrlDigitalIndicator.cpp
*/
class DigitalIndicator : public TaskManager::Task
{
  public:
    //---------------------------------------------------------------
    /*! Initialize a DigitalIndicator
        \param Digital Digital output
        \param taskManager   TaskManager for time measurement
    */
    DigitalIndicator( Digital     &devDigital,
                      TaskManager &taskManager );

  public:
    //---------------------------------------------------------------
    /*! Clears the output
    */
    virtual void clr( void );

    //---------------------------------------------------------------
    /*! Triggers a transient output
        \param time Duration of switch-on state
    */
    virtual void trigger( WORD time );

    //---------------------------------------------------------------
    /*! Starts a periodic flashing
        \param time Duration of the period
        \param duty Duty cycle [%]
    */
    void blink( WORD time, WORD duty );

  private:
    //---------------------------------------------------------------
    virtual void update( void );

  private:
    //---------------------------------------------------------------
    Digital  &digital;
    DWORD     cycleTime;
    WORD      limit;
    WORD      maximum;
    WORD      t;
    bool      repeat;
    bool      runUpdate;
}; //class DigitalIndicator

} } //namespace

#endif
