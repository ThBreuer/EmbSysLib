//*******************************************************************
/*!
\file   Memory_PCF8583.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   29.12.2022

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_MEMORY_PCF8583_H
#define _HW_MEMORY_PCF8583_H

//*******************************************************************
#include "Hardware/Common/Memory/Memory.h"
#include "Hardware/Common/I2C/I2Cmaster.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class Memory_PCF8583

\brief Implementation of the battery buffered RAM of RTC-PCF8583

\example HwMemory.cpp
*/
class Memory_PCF8583 : public Memory
{
  public:
    //---------------------------------------------------------------
    /*! Initialize PCF8574, part RAM
        \param i2c    Reference to I2C interface
        \param hwAddr Sub address of PCF8574 (address input pin A0)
    */
    Memory_PCF8583( I2Cmaster &i2c, BYTE hwAddr );

    //---------------------------------------------------------------
    virtual void unlock( void );

    //---------------------------------------------------------------
    virtual void lock( void );

    //---------------------------------------------------------------
    virtual const BYTE *getPtr( void );

    //---------------------------------------------------------------
    bool isError( void )
    {
      return( i2c.isError() );
    }

  private:
    //---------------------------------------------------------------
    virtual void writeByte( DWORD addr, BYTE  data );

    //---------------------------------------------------------------
    virtual BYTE readByte( DWORD addr );

  private:
  //----------------------------------------
    I2Cmaster::Device i2c;

    static const BYTE baseAdr = 0xA0;
    static const BYTE maskAdr = 0x01;

}; //class Memory_PCF8583

}  } //namespace

#endif
