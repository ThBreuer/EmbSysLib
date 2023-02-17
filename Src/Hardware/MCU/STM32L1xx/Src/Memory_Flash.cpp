//*******************************************************************
/*!
\file   Memory_Flash.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"
*/

//*******************************************************************
#include "Memory_Flash.h"
#include "Hardware/ReportID_Hw.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// Memory_Flash
//
//*******************************************************************
//-------------------------------------------------------------------
Memory_Flash::Memory_Flash( DWORD offset, DWORD size  )

: Memory( ReportID_Hw::Module::MEMORY_FLASH,
          size,
          0xFF,  // default value
          true ) // flash
{
  ptr            = (DWORD*)(FLASH_BASE + offset);
  dataDoubleWord = 0;
}

//-------------------------------------------------------------------
void Memory_Flash::unlock( void )
{
  // unlock FLASH_PECR
  if( FLASH->PECR & FLASH_PECR_PELOCK )
  {
    FLASH->PEKEYR = 0x89ABCDEF;
    FLASH->PEKEYR = 0x02030405;
  }

  // unlock program memory
  if( FLASH->PECR & FLASH_PECR_PRGLOCK )
  {
    FLASH->PRGKEYR = 0x8C9DAEBF;
    FLASH->PRGKEYR = 0x13141516;
  }
  dataDoubleWord = 0;
}

//-------------------------------------------------------------------
void Memory_Flash::lock( void )
{
  // write last DWORD if pending
  if( !(FLASH->PECR & FLASH_PECR_PRGLOCK) && dataDoubleWord != 0 )
  {
    while( FLASH->SR & FLASH_SR_BSY );

    ptr[pendingAddr>>2] = dataDoubleWord ;
    while( FLASH->SR & FLASH_SR_BSY );
  }

  // lock PECR and program memory
  FLASH->PECR |= FLASH_PECR_PRGLOCK;
  FLASH->PECR |= FLASH_PECR_PELOCK;
}

//-------------------------------------------------------------------
inline const BYTE *Memory_Flash::getPtr( void )
{
  return( (const BYTE*)ptr );
}

//-------------------------------------------------------------------
void Memory_Flash::erase( void )
{
  unlock();
  for( DWORD addr = 0; addr < size; addr += 0xFF )
  {
    erasePage( addr );
  }
  lock();
}

//-------------------------------------------------------------------
void Memory_Flash::writeByte( DWORD addr, BYTE data )
{
  BYTE shift = (addr&0x3)<<3;

  if( (addr&0x03) == 0 )
  {
    dataDoubleWord = data;
  }
  else
  {
    dataDoubleWord |= ((DWORD)data<<shift);
  }

  if( (addr &0xFF) == 0 )
  {
    erasePage( addr );
  }

  if( (addr&0x03) == 0x03 )
  {
    while( FLASH->SR & FLASH_SR_BSY );
    ptr[addr>>2] = dataDoubleWord ;
    while( FLASH->SR & FLASH_SR_BSY );
    dataDoubleWord = 0;
  }
  else
  {
    pendingAddr = addr;
  }
}

//-------------------------------------------------------------------
BYTE Memory_Flash::readByte( DWORD addr )
{
  BYTE shift = (addr&0x3)<<3;
  return( ptr[addr>>2] >> shift );
}

//-------------------------------------------------------------------
void Memory_Flash::erasePage( DWORD addr )
{
  FLASH->PECR |= FLASH_PECR_ERASE;
  FLASH->PECR |= FLASH_PECR_PROG;

  while( FLASH->SR & FLASH_SR_BSY );

  ptr[(addr & ~(0xFF))>>2] = 0x00;  // round to page start address

  FLASH->PECR &= ~FLASH_PECR_ERASE;
  FLASH->PECR &= ~FLASH_PECR_PROG;

  while( FLASH->SR & FLASH_SR_BSY );
}

}  } //namespace

//EOF
