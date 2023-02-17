//*******************************************************************
/*!
\file   Memory_Flash.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

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

/*!
\todo use parameter for memory size and start address.
                Clear with paramter "start address".
                 method returning page size

\todo documentation: einschränkungen "write"
  - wenn neue page begonnen wird, wird gesamte page gelöscht
  - muss mit page startadresse beginnen (vielfaches von 0xFF)
  - adressen sequentiell aufsteigend
  - Start-Adresse (offset), Überlappung mit Prog-Mem möglich!

\todo (all HwMomory classes): ssize, addr etc. with 32 bit DWORD

\todo 16-bit write possible?
     Doku:
     - writeByte will clear the complete memory page, when address points
       to page start adress
     - writeByte writes only when address matches 0x03 (each fourth adress)
       => total number of bytes should be a  multiple of 4

      // see Datasheet, Table 8 to 11
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
    DWORD *ptr;
    DWORD  dataDoubleWord;
    DWORD  pendingAddr;

}; //class Memory_Flash

}  } //namespace

#endif
