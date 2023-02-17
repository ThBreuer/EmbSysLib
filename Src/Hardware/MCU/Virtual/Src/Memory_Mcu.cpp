//*******************************************************************
/*!
\file   Memory_Mcu.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   25.02.2011

License: See file "LICENSE"
*/

//*******************************************************************
#include "Memory_Mcu.h"
#include "Hardware/ReportID_Hw.h"

#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// Memory_Mcu
//
//*******************************************************************
//-------------------------------------------------------------------
Memory_Mcu::Memory_Mcu( const char *filename, DWORD sizeIn, BYTE def )

: Memory(  ReportID_Hw::Module::MEMORY_MCU, sizeIn,    // size
            def,      // default value
            false ) // no flash
{
  cache = NULL;

  strncpy( this->filename, filename, 256 );

  if( (fhRead = open( filename, O_RDONLY|O_CREAT|O_BINARY, S_IREAD|S_IWRITE )) <= 0 )
  {
    report.error( ReportID_Hw::Event::OPEN_ERROR );
  }

  if( (sizeIn = lseek( fhRead, 0, SEEK_END )) == 0) // empty file
  {
    erase();
  }
  else
  {
    size = sizeIn;
  }

  // init cache
  //! \todo check size
  cache = new BYTE[size];

  if( cache == NULL )
  {
    report.error( ReportID_Hw::Event::MEMORY_ALLOCTION_ERROR );
  }

  lseek( fhRead, 0, SEEK_SET );
  ::read( (int)fhRead, (void *)cache, (unsigned int)sizeIn );
  //! \todo check read

  if( size == 0 )
  {
    size = lseek( fhRead, 0, SEEK_END );
    if( size < 0 )
    {
      size = 0;
    }
  }
}

//---------------------------------------------------------------
inline void Memory_Mcu::unlock( void )
{
  if( (fhWrite = open( filename, O_RDWR|O_BINARY )) <= 0 )
  {
    report.error( ReportID_Hw::Event::OPEN_ERROR );
  }
}

//---------------------------------------------------------------
inline void Memory_Mcu::lock( void )
{
  close( fhWrite );
  fhWrite = 0;
}

//---------------------------------------------------------------
inline const BYTE *Memory_Mcu::getPtr( void )
{
  return( cache );
}

//-------------------------------------------------------------------
inline void Memory_Mcu::writeByte( DWORD addr, BYTE data )
{
  if( addr < size && cache )
  {
    cache[addr] = data;
  }
  if( fhWrite )
  {
    lseek( fhWrite, addr, SEEK_SET );
    if( ::write( fhWrite, &data, 1 ) == 1 )
    {
      return;
    }
  }
  report.error( ReportID_Hw::Event::WRITE_ERROR );
}

//-------------------------------------------------------------------
inline BYTE Memory_Mcu::readByte( DWORD addr )
{
  BYTE data = 0;
  long l=-1,r=-1;
  if( fhRead
      && (l=lseek( fhRead, addr, SEEK_SET ))==(long)addr
      && (r=::read( fhRead, &data, 1 )) >=0    ) // Bug when reading 0x1A: read returns 0 instead of 1
  {
    return( data );
  }
  report.error( ReportID_Hw::Event::READ_ERROR );
  return( def );
}

}  } //namespace

//EOF
