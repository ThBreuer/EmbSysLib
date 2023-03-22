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
Memory_Flash::Memory_Flash( BYTE sectorFirst, BYTE sectorLast )

: Memory( ReportID_Hw::Module::MEMORY_FLASH,
          0,     // recalulate later
          0xFF,  // default value
          true ) // flash

{
  if( sectorFirst >= sectorLast )
  {
    sectorLast = sectorFirst; // set last sector if not given 
  }
  if( sectorLast > 11 ) // number of sectors matched?
  {
    report.alert( ReportID_Hw::Event::WRONG_ID );
  }

  this->sectorFirst = sectorFirst;
  this->sectorLast  = sectorLast;
  
  ptr = SectorInfo( sectorFirst ).ptr;
  
  if( ptr == NULL )
  {
    report.error( ReportID_Hw::Event::WRONG_ID );
  }

  // calculate total memory size
  size = 0;
  for( BYTE s = sectorFirst; s <= sectorLast; s++ )
  {
    size += SectorInfo( s ).size;
  }
  dataDoubleWord = 0;
}

//-------------------------------------------------------------------
void Memory_Flash::unlock( void )
{
  while( FLASH->SR & FLASH_SR_BSY );

  FLASH->KEYR = 0x45670123;
  FLASH->KEYR = 0xCDEF89AB;

  while( FLASH->SR & FLASH_SR_BSY );
}

//-------------------------------------------------------------------
void Memory_Flash::lock( void )
{
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
  for( BYTE s = sectorFirst; s <= sectorLast; s++ )
  {
    erasePage( s );
  }
  lock();
}

//-------------------------------------------------------------------
void Memory_Flash::writeByte( DWORD addr, BYTE data )
{
  if( addr < size )
  {
    if( addr == 0 )
    {
      for( BYTE s = sectorFirst; s <= sectorLast; s++ )
      {
        erasePage( s );
      }
    }

    FLASH->CR |= FLASH_CR_PG;
    
    ptr[addr] = data;
    
    while( FLASH->SR & FLASH_SR_BSY );
  }
  else
  {
    report.alert( ReportID_Hw::Event::SIZE_ERROR );
  }
}

//-------------------------------------------------------------------
BYTE Memory_Flash::readByte( DWORD addr )
{
  return( ptr[addr] );
}

//-------------------------------------------------------------------
void Memory_Flash::erasePage( BYTE sector )
{
  FLASH->CR &= ~(FLASH_CR_STRT | FLASH_CR_SER | (0xf<<3)); // set SER-bit
  FLASH->CR |=   FLASH_CR_SER | (sector<<3); 
  
  FLASH->CR |= FLASH_CR_STRT;// set STRT-bit
  
  while( FLASH->SR & FLASH_SR_BSY );
  
  FLASH->CR &= ~(FLASH_CR_STRT | FLASH_CR_SER | (0xf<<3)); // clr SER-bit
}

}  } //namespace

//EOF
