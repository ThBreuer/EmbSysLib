//*******************************************************************
/*!
\file   Mcu.h
\author Thomas Breuer
\date   14.03.2011

\brief Include controller specific header files

All controller specific header files are included here. 
*/

//*******************************************************************
#ifndef _HW_ATMEGA_H
#define _HW_ATMEGA_H

//*******************************************************************
#ifndef ENABLE_BIT_DEFINITIONS
#define ENABLE_BIT_DEFINITIONS
// Enable the bit definitions in the io*.h file
#endif

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

// ### Workaround 
// ### warning: only initialized variables can be placed into program memory area
// see also: Std.h
#undef PROGMEM
#define PROGMEM __attribute__(( section(".progmem.data") ))

#undef PSTR
#define PSTR(s) (__extension__({static prog_char __c[] PROGMEM = (s);&__c[0];}))

//*******************************************************************
extern "C"
{
  /* This function is called in the event that a non-overidden
   * pure virtual function is called.  The compiler should never
   * let that happen.  We get to choose what to do - we will abort
   */
  void __cxa_pure_virtual ();
}

//*******************************************************************
inline void* operator new[]    ( unsigned int  x ) { return malloc(x); }
inline void* operator new      ( unsigned int  x ) { return malloc(x); }
inline void  operator delete[] ( void         *x ) { if(x) free(x);    }
inline void  operator delete   ( void         *x ) { if(x) free(x);    }

//*******************************************************************
#include "Hardware/ReportID_Hw.h"

//*******************************************************************
#include "Src/Adc_Mcu.h"
#include "Src/I2Cmaster_Mcu.h"
#include "Src/I2Cslave_Mcu.h"
#include "Src/Memory_EEPROM.h"
#include "Src/Port_Mcu.h"
#include "Src/Rtos_Mcu.h"
#include "Src/SPImaster_Mcu.h"
#include "Src/SPIslave_Mcu.h"
#include "Src/Timer_Mcu.h"
#include "Src/Uart_Mcu.h"
#include "Src/USBdevice_Mcu.h"

#endif //_HW_ATMEGA_H
