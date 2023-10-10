//*******************************************************************
/*!
\file   Port_Mcu.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   14.02.2011

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
      PortB = 1, //!< Use GPIO B
      PortC,     //!< Use GPIO C
      PortD,     //!< Use GPIO D
      PortE,     //!< Use GPIO E
    } PortId;

  public:
    //---------------------------------------------------------------
    /*! Instantiate a port
        \param portId Port identification
    */
    Port_Mcu( PortId portId );

   //---------------------------------------------------------------
    virtual void setMode( const MTYPE mask, Mode mode );

    //---------------------------------------------------------------
    virtual void setPinMode( const BYTE pinId, Mode modeIn );

    //---------------------------------------------------------------
    virtual void set( const MTYPE mask );

    //---------------------------------------------------------------
    virtual void set( const MTYPE mask, MTYPE value );

    //---------------------------------------------------------------
    virtual void clr( const MTYPE mask );

    //---------------------------------------------------------------
    virtual MTYPE get( void );

  private:
    //---------------------------------------------------------------
    volatile BYTE *ptr;

}; //class Port_Mcu

}  } //namespace

#endif
