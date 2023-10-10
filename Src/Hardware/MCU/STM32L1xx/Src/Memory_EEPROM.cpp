//*******************************************************************
/*!
\file   Memory_EEPROM.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"
*/

//*******************************************************************
#include "Memory_EEPROM.h"
#include "Hardware/ReportID_Hw.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// Memory_EEPROM
//
//*******************************************************************
/*!
\todo
size depends on category, see reference manual chapter 3.2.
Here: STM32L100RC = cat. 3
*/
//-------------------------------------------------------------------
Memory_EEPROM::Memory_EEPROM( void )

: Memory( ReportID_Hw::Module::MEMORY_EEPROM,
          0x2000, // size Cat.3 = 8Kbytes
          0x00,   // default value
          false ) // no flash

{
  ptr = (BYTE*)(0x08080000); // see Datasheet, Table 8 to 11
}

//-------------------------------------------------------------------
void Memory_EEPROM::unlock( void )
{
  if( FLASH->PECR & FLASH_PECR_PELOCK )
  {
    FLASH->PEKEYR = 0x89ABCDEF;
    FLASH->PEKEYR = 0x02030405;
  }
  FLASH->PECR &= ~FLASH_PECR_FTDW; // 0: conditional erase before programming
}

//-------------------------------------------------------------------
void Memory_EEPROM::lock( void )
{
  FLASH->PECR |= FLASH_PECR_PELOCK;
}

//-------------------------------------------------------------------
inline const BYTE *Memory_EEPROM::getPtr( void )
{
  return( (const BYTE*)ptr );
}

//-------------------------------------------------------------------
void Memory_EEPROM::writeByte( DWORD addr, BYTE data )
{
  while( FLASH->SR & FLASH_SR_BSY );
  ptr[addr] = data;
}

//-------------------------------------------------------------------
BYTE Memory_EEPROM::readByte( DWORD addr )
{
  return( ptr[addr] );
}

}  } //namespace

//EOF
