//*******************************************************************
/*!
\file   Memory.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_MEMORY_H
#define _HW_MEMORY_H

//*******************************************************************
#include "Mcu_Types.h"
#include "Std/Report.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class Memory

\brief Abstract class supporting non volatile memory

This base class encapsulates non volatile memories,
e.g. EEPROM, Flash-RAM  or battery buffered RAM

Derived classes should at least:
 - initialize the memory hardware access (but not the memory content)
 - implement \a unlock() to enable write access
 - implement \a lock() to disable write access
 - implement \a writeByte() to write a data byte into the memory
 - implement \a readByte() to read a data byte from the memory
 - implement \a getPtr() to deliver memory location

\example HwMemory.cpp
*/
class Memory
{
  protected:
    //---------------------------------------------------------------
    // Initialize memory access
    // moduleId:    Id of this module, used for report
    // size:        Memory space
    // def:         Default value, used in clear() and read()
    // isFlashFlag: true, if memory is a flash memory (block erase only)
    //
    Memory( WORD moduleId, DWORD size, BYTE def, bool isFlashFlag );

  public:
    //---------------------------------------------------------------
    /*! Unlock memory
    */
    virtual void unlock( void ) = 0;

    //---------------------------------------------------------------
    /*! Lock memory
    */
    virtual void lock( void ) = 0;

    //---------------------------------------------------------------
    /*! Get pointer to first memory location
        \return
    */
    virtual const BYTE *getPtr( void ) = 0;

    //---------------------------------------------------------------
    /*! Write a data byte to the specified memory address
        \param addr Memory address
        \param data Data byte
    */
    virtual void write( DWORD addr, BYTE  data );

    //---------------------------------------------------------------
    /*! Read a data byte from a specified memory address
        \param  addr Memory address
        \return data byte
    */
    virtual BYTE read( DWORD addr );

    //---------------------------------------------------------------
    /*! Erase the complete memory
    */
    virtual void erase( void );

    //---------------------------------------------------------------
    /*! Get memory size
        \return Size of memory, unit: byte
    */
    virtual DWORD getSize( void ) const;

    //---------------------------------------------------------------
    /*! Check, if memory is of type flash with block erase only
        \return true, if the memory is a flash memory
    */
    virtual bool isFlash( void ) const;

  protected:
    //---------------------------------------------------------------
    // Writes a data byte to the memory hardware
    // addr: Memory address
    // data: Stored data byte
    //
    virtual void writeByte( DWORD addr, BYTE  data ) = 0;

    //---------------------------------------------------------------
    // Reads a data byte from the memory hardware
    // addr:   Memory address
    // return: Recalled data byte
    //
    virtual BYTE readByte( DWORD addr ) = 0;

  protected:
    //---------------------------------------------------------------
    const BYTE def;
    const bool isFlashFlag;
    DWORD      size;  // size may be recalculated in the constructor of the derived class

  public:
    //---------------------------------------------------------------
    Std::Report report;

}; //class Memory

}  } //namespace

#endif
