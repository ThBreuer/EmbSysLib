//*******************************************************************
/*!
\file   Port_PCF8574.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   29.12.2022

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_PORT_PCF8574_H
#define _HW_PORT_PCF8574_H

//*******************************************************************
#include "Hardware/Common/Port/Port.h"
#include "Hardware/Common/I2C/I2Cmaster.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*! 
\class Port_PCF8574

\brief Implementation of the digital I/O expander PCF8574 (I2C-device)

\example HwPort.cpp
*/
class Port_PCF8574 : public Port
{
  public:
    //---------------------------------------------------------------
    /*! Initialize PCF8574
        \param i2c    Reference to I2C interface
        \param hwAddr Sub address of PCF8574 (address pins A0,A1,A2)
    */
    Port_PCF8574( I2Cmaster &i2c, 
                  BYTE       hwAddr );

    //---------------------------------------------------------------
    virtual void setMode( const MTYPE mask, Mode mode );

    //---------------------------------------------------------------
    virtual void setPinMode( const BYTE pinId, Mode mode );

    //---------------------------------------------------------------
    virtual void set( const MTYPE mask );

    //---------------------------------------------------------------
    virtual void set( const MTYPE mask, MTYPE value );

    //---------------------------------------------------------------
    virtual void clr( const MTYPE mask );

    //---------------------------------------------------------------
    virtual MTYPE get( void );

    //---------------------------------------------------------------
    virtual bool isError( void )
    {
      return( i2c.isError() );
    }

  private:
    //---------------------------------------------------------------
    I2Cmaster::Device  i2c;

    MTYPE  valueIn;
    MTYPE  valueOut;
    MTYPE  dir;

    //---------------------------------------------------------------
    static const BYTE baseAdr = 0x40;
    static const BYTE maskAdr = 0x07;

}; //class Port_PCF8574

}  } //namespace

#endif
