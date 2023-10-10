//*******************************************************************
/*!
\file   Memory_EEPROM.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   25.02.2011

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_MEMORY_EEPROM_H
#define _HW_MEMORY_EEPROM_H

//*******************************************************************
#include "Hardware/Common/Memory/Memory.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class Memory_EEPROM

\brief Implementation of the EEPROM as a Memory class

\example HwMemory.cpp
*/
class Memory_EEPROM : public Memory
{
  public:
    //---------------------------------------------------------------
    /*! Initialize hardware
    */
    Memory_EEPROM( void );

    //---------------------------------------------------------------
    virtual void unlock( void );

    //---------------------------------------------------------------
    virtual void lock( void );

    //---------------------------------------------------------------
    virtual const BYTE *getPtr( void );

  private:
    //---------------------------------------------------------------
    virtual void writeByte( DWORD addr, BYTE  data );

    //---------------------------------------------------------------
    virtual BYTE readByte( DWORD addr );

  private:
    //---------------------------------------------------------------
    BYTE *ptr;

}; //class Memory_EEPROM

}  } //namespace

#endif
