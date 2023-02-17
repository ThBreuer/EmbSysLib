//*******************************************************************
/*!
\file   Memory_Flash.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   16.10.2022

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
Memory_Flash::Memory_Flash( DWORD offset, DWORD size )

: Memory( ReportID_Hw::Module::MEMORY_FLASH,
          size,
          0xFF, // default value
          true )// flash

{
  ptr            = (DWORD*)(0x8000000 + offset);
  dataDoubleWord[0] = 0;
  dataDoubleWord[1] = 0;
}

//-------------------------------------------------------------------
void Memory_Flash::unlock( void )
{
  // unlock FLASH
  while( FLASH->SR & FLASH_SR_BSY );

  FLASH->KEYR = 0x45670123;
  FLASH->KEYR = 0xCDEF89AB;

  while( FLASH->SR & FLASH_SR_BSY );

  FLASH->CR |= FLASH_CR_PG;

  while( FLASH->SR & FLASH_SR_BSY );
  dataDoubleWord[0] = 0;
  dataDoubleWord[1] = 0;
}

//-------------------------------------------------------------------
void Memory_Flash::lock( void )
{
  //<! /todo Write pending bytes ?

  while( FLASH->SR & FLASH_SR_BSY );

  FLASH->CR &= ~FLASH_CR_PG;

  FLASH->CR |= FLASH_CR_LOCK;

  while( FLASH->SR & FLASH_SR_BSY );
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
  for( DWORD addr = 0; addr < size; addr += 0x7FF )
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
    dataDoubleWord[(addr&0x04)?1:0] = data;
  }
  else
  {
    dataDoubleWord[(addr&0x04)?1:0] |= ((DWORD)data<<shift);
  }

  if( (addr &0x7FF) == 0 ) // 2K-Limit ?
  {
    erasePage( addr );
  }

  if( (addr&0x07) == 0x07 )
  {
    addr = (addr >> 2)&~0x01;
    while( FLASH->SR & FLASH_SR_BSY );
    ptr[addr] = dataDoubleWord[0];
    while( FLASH->SR & FLASH_SR_BSY );
    ptr[addr+1] = dataDoubleWord[1];
    while( FLASH->SR & FLASH_SR_BSY );
    dataDoubleWord[0] = 0;
    dataDoubleWord[1] = 0;
  }
  else
  {
    pendingAddr = addr;
  }
}

//-------------------------------------------------------------------
BYTE Memory_Flash::readByte( DWORD addr )
{
  return( ((BYTE*)ptr)[addr] );
}
//-------------------------------------------------------------------
void Memory_Flash::erasePage( DWORD addr )
{
  DWORD page = ((DWORD)ptr-0x8000000 + addr)/0x800;
  while( FLASH->SR & FLASH_SR_BSY );
  FLASH->CR &= ~FLASH_CR_PG;
  FLASH->CR |= FLASH_CR_PER | (page&0xff)<<3;
  FLASH->CR |= FLASH_CR_STRT;

  while( FLASH->SR & FLASH_SR_BSY );
  FLASH->CR &= ~FLASH_CR_PER;
  FLASH->CR |= FLASH_CR_PG;
}

}  } //namespace


//EOF
