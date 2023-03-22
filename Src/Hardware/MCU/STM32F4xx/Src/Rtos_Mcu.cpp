//*******************************************************************
/*!
\file   Rtos_Mcu.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   07.02.2023

License: See file "LICENSE"
*/

//*******************************************************************
#include "Rtos_Mcu.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// Rtos_Mcu
//
//*******************************************************************
//-------------------------------------------------------------------
Rtos_Mcu::TaskHandle (*Rtos_Mcu::schedule)( TaskHandle sp, BYTE flag ) = 0;

//-------------------------------------------------------------------
volatile unsigned long  Rtos_Mcu::sysTic = 0;
unsigned                Rtos_Mcu::sysTicTime = 0;

//-------------------------------------------------------------------
extern "C"
{
  unsigned save_context( void         );
  void     load_context( unsigned ptr );
}

//-------------------------------------------------------------------
extern "C"
{
  void SysTick_Handler( void )
  {
    static volatile DWORD stackPointer = 0;
    
    stackPointer = save_context();
    Rtos_Mcu::sysTic++;
    stackPointer = (DWORD)Rtos_Mcu::schedule( (Rtos_Mcu::TaskHandle)stackPointer, true );
    load_context( stackPointer );
    asm("NOP");  // Workaround: armclang optimizes in a way, that load_context and POP is erroneously reordered
  }
}

//-------------------------------------------------------------------
extern "C"
{
  void PendSV_Handler( void )
  {
    static volatile DWORD stackPointer = 0;

    stackPointer = save_context();
    stackPointer = (DWORD)Rtos_Mcu::schedule( (Rtos_Mcu::TaskHandle)stackPointer, false );
    load_context( stackPointer );
    asm("NOP");  // Workaround: armclang optimizes in a way, that load_context and POP is erroneously reordered
  }
}

}  } //namespace

//EOF
