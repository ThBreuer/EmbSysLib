//*******************************************************************
/*!
\file   MemoryImage.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   08.01.2023

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_MEMORY_IMAGE_H
#define _HW_MEMORY_IMAGE_H

//*******************************************************************
#include "Mcu_Types.h"
#include "Std/Report.h"
#include "Memory.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class MemoryImage

\brief Divides a Memory area (typically a flash memory) in named sections.

Large data structures like bitmaps or fonts can be stored in a flash 
memory. But a typical flash memory of a mikrocontroller has large blocks, 
which size doesn't match the desired size of data structures.
The MemoryImage class allows a read-only access to named data areas stored 
in a flas memory. The flash memory must have the following format:

  \code
  Length (Byte)    Content
  4                Signature = 0x20496D67

  32               Name of the first data structure/content
  4                Length N0 of the first data structure/content
  N0               Data structue/content

  ...              ... i-th name, length and content ...

  32               Name of the last data structure/content
  4                Length Nx of the next data structure/content
  Nx               Data structue/content

  32               Zero filled 
  4                Length = 0
  \endcode

\remark The Tool "Image" can be used to generate an image file from 
a set of binary files. The image can then be downloaded to 
mikrocontroller flash memory using designated download tools.

*/
class MemoryImage : public Memory
{
  private:
    //---------------------------------------------------------------
    class contentDescriptor
    {
      public:
        char  name[32];
        DWORD size;
    } cd;

  public:
    //---------------------------------------------------------------
    /*! Initialize memory access
        \param mem Reference to a memory object containing the image
        \param name Name of the content 
    */
    MemoryImage( Memory &mem, const char *name )

    : Memory( ReportID_Hw::Module::MEMORY_IMAGE,  // moduleId 
              0,                                  // size, will be set later 
              0xFF,                               // default
              mem.isFlash() )

    {
      DWORD addr      = 0;
      DWORD signature = 0;

      ptr  = 0;

      // read signature
      for( DWORD i = 0; i < sizeof(signature); i++ )
      {
        ((BYTE*)&signature)[i] = mem.read( addr+i );
      }
      addr += sizeof(signature);
      
      if( signature == 0x20496D67 ) // signature correct?
      {
        while( addr < mem.getSize() )  // read up to end of memory
        {
          for( DWORD i = 0; i < sizeof(cd); i++ ) // read content descriptor
          {
            ((BYTE*)&cd)[i] = mem.read( addr+i );
          }
          if( cd.size == 0 || cd.name[0] == 0 )  // check content descriptor
          {
            break;                               // no more contents
          }
          addr += sizeof(contentDescriptor);  // set addr to begin of content
          if( strcmp( cd.name, name ) == 0 )  // check name: content found?
          {
            size = cd.size;                   // get size and address of the memory
            ptr  = (BYTE*)mem.getPtr() + addr;
            break;                            // ready
          }
          addr += cd.size; // set addr to next content
        }
      }
      if( ptr == 0 )
      {
        report.error( ReportID_Hw::Event::WRONG_ID );
      }
    }

  public:
    //---------------------------------------------------------------
    virtual void unlock( void )
    {
    }

    //---------------------------------------------------------------
    virtual void lock( void )
    {
    }

    //---------------------------------------------------------------
    virtual const BYTE *getPtr( void )
    {
      return( ptr );
    }

  protected:
    //---------------------------------------------------------------
    virtual void writeByte( DWORD addr, BYTE  data )
    {
      // writing impossible
      report.error( ReportID_Hw::Event::FUNCTION_NOT_SUPPORTED );
    }

    //---------------------------------------------------------------
    // Reads a data byte from the memory hardware
    // addr:   Memory address
    // return: Data byte
    //
    virtual BYTE readByte( DWORD addr )
    {
      return( ptr[addr] );
    }

  private:
    //---------------------------------------------------------------
    BYTE *ptr;

}; //class MemoryImage

}  } //namespace

#endif
