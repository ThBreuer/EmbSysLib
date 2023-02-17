//*******************************************************************
/*!
\file   Memory_PCF8583.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   29.12.2022

License: See file "LICENSE"
*/

//*******************************************************************
#include "Memory_PCF8583.h"
#include "Hardware/ReportID_Hw.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// Memory_PCF8583
//
//*******************************************************************
//-------------------------------------------------------------------
Memory_PCF8583::Memory_PCF8583( I2Cmaster &i2cIn, BYTE hwAddrIn )

: Memory( ReportID_Hw::Module::MEMORY_PERIPHERAL,
          240,    // size
          0x00,   // default value
          false   )

, i2c( i2cIn, baseAdr | ((hwAddrIn&maskAdr)<<1) )

{
}

//-------------------------------------------------------------------
void Memory_PCF8583::unlock( void )
{
}

//-------------------------------------------------------------------
void Memory_PCF8583::lock( void )
{
}

//-------------------------------------------------------------------
inline const BYTE *Memory_PCF8583::getPtr( void )
{
  report.error( ReportID_Hw::Event::POINTER_NOT_AVAILABLE );
  return( NULL );
}

//-------------------------------------------------------------------
inline void Memory_PCF8583::writeByte( DWORD addr, BYTE data )
{
  i2c.write( (BYTE)(addr+0x10), data );
}

//-------------------------------------------------------------------
inline BYTE Memory_PCF8583::readByte( DWORD addr )
{
  return( i2c.read( (BYTE)(addr+0x10) ));
}

}  } //namespace

//EOF
