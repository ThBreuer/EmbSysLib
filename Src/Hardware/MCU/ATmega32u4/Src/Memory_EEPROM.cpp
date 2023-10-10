//*******************************************************************
/*!
\file   Memory_EEPROM.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   25.02.2011

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
//-------------------------------------------------------------------
Memory_EEPROM::Memory_EEPROM( void )

: Memory( ReportID_Hw::Module::MEMORY_EEPROM,
          0x400,
          0x00,   // default value
          false ) // no flash

{
  // EEPROM Control Register
  EECR &= ~(1<<EERIE); // Ready Interrupt Enable: no interrupt
}

__attribute__((optimize("Os"))) // see EECR

//-------------------------------------------------------------------
void Memory_EEPROM::unlock( void )
{
}

//-------------------------------------------------------------------
void Memory_EEPROM::lock( void )
{
}

//-------------------------------------------------------------------
inline const BYTE *Memory_EEPROM::getPtr( void )
{
  return( (const BYTE*)ptr );
}

//-------------------------------------------------------------------
void Memory_EEPROM::writeByte( DWORD addr, BYTE data )
{
  while(EECR & (1<<EEPE)); // wait until "EEPROM Write Enable"

  EEARL = (BYTE)(addr & 0xFF); // Set address
  EEARH = (BYTE)(addr >> 8);   //
  EEDR  = data;                // Set data

  // The next two instructions are time critical:
  //   - use optimize("Os")
  //   - disable all interrupts
  
  //!< todo surround with cSystem::disable/enableInterrupt();

  EECR = (1<<EEMPE); // EEPROM Master Write Enable
  EECR = (1<<EEPE);  // EEPROM Write Enable
}

//-------------------------------------------------------------------
BYTE Memory_EEPROM::readByte( DWORD addr )
{
  while(EECR & (1<<EEPE)); // wait until "EEPROM Write Enable"

  EEARL = (BYTE)(addr & 0xFF); // Set address
  EEARH = (BYTE)(addr >> 8);   //
  EECR  = (1<<EERE);           // Set strobe to EEPROM
  return(EEDR);                // Read data
}

//-------------------------------------------------------------------
ISR( EE_READY_vect )
{
  // nothing to do
}

}  } //namespace

//EOF
