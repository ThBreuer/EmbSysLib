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
#include "Hardware/Common/Port/Port.h"
#include "Hardware/Common/Port/ExtInt.h"

#include "Port_Mcu.h"


//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class ExtInt_Mcu

\brief todo ...

\example ...
*/
class ExtInt_Mcu : public ExtInt
{
  public:
    //---------------------------------------------------------------
    /*! \enum PortId
        \brief GPIO port identification
    */
/*    typedef enum
    {
      PA = 0, //!< GPIO Port A
      PB,     //!< GPIO Port B
      PC,     //!< GPIO Port C
      PD,     //!< GPIO Port D
      PE,     //!< GPIO Port E
      PF,     //!< GPIO Port F
      PG,     //!< GPIO Port G
      PH,     //!< GPIO Port H
      PI,     //!< GPIO Port I
    } PortId;
*/
  public:
    //---------------------------------------------------------------
    /*! Instantiate ...
        \param portId Port identification
    */
    ExtInt_Mcu( Port_Mcu::PortId portId, BYTE pinId );

    virtual void enable();
    virtual void disable();
    virtual void setEdge( Edge edge);

    //---------------------------------------------------------------
    // Interrupt Service Routine (ISR) of timer interrupt.
    // This function must be public, because it is called
    // in extern "C"
    void isr_N( void );

static ExtInt_Mcu *extIntPtr[16];

BYTE   pinId;
BYTE   portId;
IRQn_Type  irqNumber;

}; //class ExtInt_Mcu
}  } //namespace

#endif
