//*******************************************************************
/*!
\file   DigitalButton.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   26.01.2023

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _CTRL_DIGITAL_BUTTON_H
#define _CTRL_DIGITAL_BUTTON_H

//*******************************************************************
#include "Device/Digital/Digital.h"
#include "Device/Task/TaskManager.h"

//*******************************************************************
namespace EmbSysLib {
namespace Ctrl {

//*******************************************************************
/*!
\class   DigitalButton

\brief   Debounces a digital input button and detects short 
         and long keystrokes

\example CtrlDigitalButton.cpp
*/
class DigitalButton : public TaskManager::Task
{
  public:
    //---------------------------------------------------------------
    /*! \enum Action
        \brief Action event
    */
    typedef enum
    {
      NONE  = 0,  //!< No event
      ACTIVATED,  //!< Button pressed
      SHORT,      //!< Button pressed for a short time
      LONG        //!< button pressed for a long time
    } Action;

  public:
    //---------------------------------------------------------------
    /*! Initialize a DigitalButton
        \param Digital       Digital input
        \param taskManager   TaskManager for time measurement
        \param timeDebounce  Time interval for debouncing
        \param timeLong      Minimum time for long buttom press
    */
    DigitalButton( Digital     &devDigital,
                   TaskManager &taskManager,
                   WORD         timeBebounce,
                   WORD         timeLong );

  public:
    //---------------------------------------------------------------
    /*! Returns button event
        \return Action
    */
    Action getAction();

  private:
    //---------------------------------------------------------------
    virtual void update( void );

  private:
    //---------------------------------------------------------------
    Digital     &digital;
    const WORD   timeDebounce;
    const WORD   timeLong;
    WORD         timeOut;
    WORD         time;
    bool         value;
    bool         valuePre;
    Flag<Action> action;

}; //class DigitalButton

} } //namespace

#endif
