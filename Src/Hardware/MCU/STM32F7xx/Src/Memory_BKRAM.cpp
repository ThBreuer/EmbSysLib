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
//-------------------------------------------------------------------
Memory_BKRAM::Memory_BKRAM( void )

: Memory( ReportID_Hw::Module::MEMORY_BKRAM,
          128,    // size
          0xFF,   // default value
          false ) // no flash

{
  ptr = (DWORD*)(&RTC->BKP0R);
  RCC->APB1ENR |= RCC_APB1ENR_PWREN;
  RCC->AHB1ENR |= RCC_AHB1ENR_BKPSRAMEN;
}

//-------------------------------------------------------------------
void Memory_BKRAM::unlock( void )
{
  PWR->CR1 |= PWR_CR1_DBP;
}

//-------------------------------------------------------------------
void Memory_BKRAM::lock( void )
{
  PWR->CR1 &= ~PWR_CR1_DBP;
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
