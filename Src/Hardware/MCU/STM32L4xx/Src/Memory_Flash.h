//*******************************************************************
/*!
\file   Memory_Flash.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   16.10.2022

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_MEMORY_FLASH_H
#define _HW_MEMORY_FLASH_H

//*******************************************************************
#include "Hardware/Common/Memory/Memory.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class Memory_Flash

\brief Implementation of the Flash-RAM as a Memory class

\example HwMemory.cpp
*/
class Memory_Flash : public Memory
{
  public:
    //---------------------------------------------------------------
    /*! Initialize hardware
        \param offset       Physical start address-offset of the memory area
        \param size         Used memory size
    */
    Memory_Flash( DWORD offset, DWORD size );

    //---------------------------------------------------------------
    virtual void unlock( void );

    //---------------------------------------------------------------
    virtual void lock( void );

    //---------------------------------------------------------------
    virtual const BYTE *getPtr( void );

    //---------------------------------------------------------------
    virtual void erase( void );

  private:
    //---------------------------------------------------------------
    virtual void writeByte( DWORD addr, BYTE data );

    //---------------------------------------------------------------
    virtual BYTE readByte( DWORD addr );

    //---------------------------------------------------------------
    void erasePage( DWORD addr );
  
  private:
    //---------------------------------------------------------------
    DWORD  *ptr;
    DWORD   dataDoubleWord[2];
    DWORD   pendingAddr;

}; //class Memory_Flash

}  } //namespace


#endif
