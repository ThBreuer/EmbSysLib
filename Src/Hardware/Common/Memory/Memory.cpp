//*******************************************************************
/*!
\file   Memory.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"
*/

//*******************************************************************
#include "Memory.h"
#include "Hardware/ReportID_Hw.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// Memory
//
//*******************************************************************
//-------------------------------------------------------------------
Memory::Memory( WORD moduleId, DWORD size, BYTE def, bool isFlashFlag )
: def        ( def         )
, isFlashFlag( isFlashFlag )
, report     ( moduleId    )
{
  this->size        = size;
}

//-------------------------------------------------------------------
inline void Memory::write( DWORD addr, BYTE data )
{
  if( addr < size )          // valid address?
  {
    writeByte( addr, data ); // write data byte
  }
  else
  {
    report.error( ReportID_Hw::Event::WRITE_ERROR );
  }
}

//-------------------------------------------------------------------
inline BYTE Memory::read( DWORD addr )
{
  if( addr < size )           // valid address?
  {
    return( readByte(addr) ); // read data byte
  }
  else
  {
    report.error( ReportID_Hw::Event::READ_ERROR );
    return( def );
  }
}

//-------------------------------------------------------------------
inline void Memory::erase( void )
{
  unlock();
  for( DWORD i = 0; i < size; i++ )  // write default to all addresses
  {
    writeByte( i, def );
  }
  lock();
}

//-------------------------------------------------------------------
inline DWORD Memory::getSize( void ) const
{
  return( size );
}

//-------------------------------------------------------------------
inline bool Memory::isFlash( void ) const
{
  return( isFlashFlag );
}

}  } //namespace

//EOF
