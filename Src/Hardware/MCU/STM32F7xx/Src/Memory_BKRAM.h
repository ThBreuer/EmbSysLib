//*******************************************************************
/*!
\file   Memory_BKRAM.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   16.10.2022

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_MEMORY_BKRAM_H
#define _HW_MEMORY_BKRAM_H

//*******************************************************************
#include "Hardware/Common/Memory/Memory.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class Memory_BKRAM

\brief Implementation of the battery buffered RAM

\example HwMemory.cpp
*/
class Memory_BKRAM : public Memory
{
  public:
    //---------------------------------------------------------------
    /*! Initialize battery buffered RAM hardware
    */
    Memory_BKRAM( void );

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
    DWORD *ptr;

}; //class Memory_BKRAM

}  } //namespace

#endif
