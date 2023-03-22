//*******************************************************************
/*!
\file   Memory_BKRAM.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"
*/

//*******************************************************************
#include "Memory_BKRAM.h"
#include "Hardware/ReportID_Hw.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// Memory_BKRAM
//
//*******************************************************************
/*!
\todo
size depends on category, see reference manual chapter 20.6.20:
"Address offset: 0x50 to 0x60 (value line devices)
 Address offset: 0x50 to 0x9C (Cat.1 and Cat.2 devices)
 Address offset: 0x50 to 0xCC (Cat.3, Cat.4 and Cat.5 devices)"
Here: STM32L100RC = cat. 3
*/
//-------------------------------------------------------------------
Memory_BKRAM::Memory_BKRAM( void )

: Memory( ReportID_Hw::Module::MEMORY_BKRAM,
          80,     // size
          0xFF,   // default value
          false ) // no flash

{
  ptr = (DWORD*)(&RTC->BKP0R);
  PWR->CR |= PWR_CR_DBP;
}

//-------------------------------------------------------------------
void Memory_BKRAM::unlock( void )
{
  PWR->CR |= PWR_CR_DBP;
}

//-------------------------------------------------------------------
void Memory_BKRAM::lock( void )
{
  PWR->CR &= ~PWR_CR_DBP;
}

//-------------------------------------------------------------------
inline const BYTE *Memory_BKRAM::getPtr( void )
{
  return( (const BYTE*)ptr );
}

//-------------------------------------------------------------------
inline void Memory_BKRAM::writeByte( DWORD addr, BYTE data )
{
  // write a single byte within a DWORD-Array
  //
  DWORD *tmp  = ptr + (addr>>2);
  BYTE   offs = (addr&0x03)<<3;

  *tmp &= ~(0xFF<<offs);
  *tmp |=  (data<<offs);
}

//-------------------------------------------------------------------
inline BYTE Memory_BKRAM::readByte( DWORD addr )
{
  // read a single byte within a DWORD-Array
  //
  DWORD *tmp  = ptr + (addr>>2);
  BYTE   offs = (addr&0x03)<<3;

  return( ((*tmp)>>offs) & 0xFF );
}

}  } //namespace

//EOF
