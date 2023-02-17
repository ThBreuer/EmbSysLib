//*******************************************************************
/*!
\file   Memory_Mcu.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   25.02.2011

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_MEMORY_MCU_H
#define _HW_MEMORY_MCU_H

//*******************************************************************
#include "Hardware/Common/Memory/Memory.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class Memory_Mcu

\brief Implementation of a non-volatile memory

\example HwMemory.cpp
*/
class Memory_Mcu : public Memory
{
  public:
    //---------------------------------------------------------------
    /*! Initialize memory

        \param filename
        \param size
        \param def
    */
    Memory_Mcu( const char *filename, DWORD size = 0, BYTE def = 0 );

    //---------------------------------------------------------------
    virtual void lock( void );

    //---------------------------------------------------------------
    virtual void unlock( void );

    //---------------------------------------------------------------
    virtual const BYTE *getPtr( void );

  private:
    //---------------------------------------------------------------
    virtual void writeByte( DWORD addr, BYTE  data );

    //---------------------------------------------------------------
    virtual BYTE readByte( DWORD addr );

  private:
    //---------------------------------------------------------------
    char   filename[256+1];
    BYTE  *cache;
    DWORD *ptr;
    int    fhRead;
    int    fhWrite;

}; //class Memory_Mcu

}  } //namespace

#endif
