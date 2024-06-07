//*******************************************************************
/*!
\file   ExtInt_Mcu.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   31.01.2024

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_EXTINT_MCU_H
#define _HW_EXTINT_MCU_H

//*******************************************************************
#include "Hardware/Common/Port/ExtInt.h"
#include "Port_Mcu.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class ExtInt_Mcu

\brief todo ...

\example HwExtInt.cpp
*/
class ExtInt_Mcu : public ExtInt
{
  public:
    //---------------------------------------------------------------
    /*! Instantiate ...
        The GPIO configuration is not changed by this constructor, 
        this must be done separatly.
        \param portId Port identification
        \param pinId Pin number
    */
    ExtInt_Mcu( Port_Mcu::PortId portId, BYTE pinId );

    //---------------------------------------------------------------
    virtual void enable();

    //---------------------------------------------------------------
    virtual void disable();

    //---------------------------------------------------------------
    virtual void setEdge( Edge edge);

  public:
    //---------------------------------------------------------------
    // Interrupt Service Routine (ISR) of timer interrupt.
    // This function must be public, because it is called
    // in extern "C"
    void isr( void );

  public:
    //---------------------------------------------------------------
    static ExtInt_Mcu *extIntPtr[16];

  private:
    //---------------------------------------------------------------
    BYTE       pinId;
    BYTE       portId;
    IRQn_Type  irqNumber;

}; //class ExtInt_Mcu

}  } //namespace

#endif
