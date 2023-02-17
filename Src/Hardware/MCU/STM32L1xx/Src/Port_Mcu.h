//*******************************************************************
/*!
\file   Port_Mcu.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_Port_MCU_H
#define _HW_Port_MCU_H

//*******************************************************************
#include "Hardware/Common/Port/Port.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class Port_Mcu

\brief Implementation of the GPIO
*/
class Port_Mcu : public Port
{
  public:
    //---------------------------------------------------------------
    /*! \enum PortId
        \brief GPIO port identification
    */
    typedef enum
    {
      PA = 0, //!< GPIO Port A
      PB,     //!< GPIO Port B
      PC,     //!< GPIO Port C
      PD,     //!< GPIO Port D
      PE,     //!< GPIO Port E
      PF,     //!< GPIO Port F
      PG,     //!< GPIO Port G
      PH      //!< GPIO Port H
    } PortId;

  public:
    //---------------------------------------------------------------
    /*! Instantiate a port
        \param portId Port identification
    */
    Port_Mcu( PortId portId );

    //---------------------------------------------------------------
    virtual void setMode( MTYPE mask, Mode mode );

    //---------------------------------------------------------------
    virtual void setPinMode( BYTE pinId, Mode modeIn );

    //---------------------------------------------------------------
    virtual void set( MTYPE mask );

    //---------------------------------------------------------------
    virtual void set( MTYPE mask, MTYPE value );

    //---------------------------------------------------------------
    virtual void clr( MTYPE mask );

    //---------------------------------------------------------------
    virtual MTYPE get( void );

   private:
    //---------------------------------------------------------------
    GPIO_TypeDef *gpio;

}; //class Port_Mcu

}  } //namespace

#endif
