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
  private:
    //---------------------------------------------------------------
    // see: RM0410,Reference manual STM32F76xxx/77xxx, ...
    // Table 3. 2 Mbytes Flash memory single bank organization (256 bits read width)
    class SectorInfo
    {
      public:
      BYTE *ptr;
      DWORD size;

      SectorInfo( BYTE sector )
      {
        switch( sector )
        {
          case  0: ptr = (BYTE*)0x08000000; size = 0x08000; break; //  32k
          case  1: ptr = (BYTE*)0x08008000; size = 0x08000; break; //  32k
          case  2: ptr = (BYTE*)0x08010000; size = 0x08000; break; //  32k
          case  3: ptr = (BYTE*)0x08018000; size = 0x08000; break; //  32k
          case  4: ptr = (BYTE*)0x08020000; size = 0x20000; break; // 128k
          case  5: ptr = (BYTE*)0x08040000; size = 0x40000; break; // 256k
          case  6: ptr = (BYTE*)0x08080000; size = 0x40000; break; // 256k
          case  7: ptr = (BYTE*)0x080C0000; size = 0x40000; break; // 256k
          case  8: ptr = (BYTE*)0x08100000; size = 0x40000; break; // 256k
          case  9: ptr = (BYTE*)0x08140000; size = 0x40000; break; // 256k
          case 10: ptr = (BYTE*)0x08180000; size = 0x40000; break; // 256k
          case 11: ptr = (BYTE*)0x081C0000; size = 0x40000; break; // 256k
          default: ptr = (BYTE*)NULL;       size = 0;       break;
        }
      }
    };

  public:
    //---------------------------------------------------------------
    /*! Initialize hardware
        \param sector Flash memory sector
    */
    Memory_Flash( BYTE sectorFirst, BYTE sectorLast = 0 );

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
    void erasePage( BYTE sector );
  
  private:
    //---------------------------------------------------------------
    BYTE  *ptr;
    DWORD  dataDoubleWord;
    BYTE   sectorID;
    BYTE   sectorFirst;
    BYTE   sectorLast;

}; //class Memory_Flash

}  } //namespace

#endif
