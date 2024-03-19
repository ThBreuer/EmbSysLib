//*******************************************************************
/*!
\file   Mcu_Types.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   16.10.2022

License: See file "LICENSE"

\brief Include controller specific header files

All controller specific header files are included here.
*/

//*******************************************************************
#ifndef _MCU_TYPES_H
#define _MCU_TYPES_H

//*******************************************************************
#include <stdlib.h>
#include <stdint.h>

//*******************************************************************
typedef uint8_t    BYTE;
typedef uint16_t   WORD;
typedef uint32_t   DWORD;
typedef uint64_t   LWORD;
typedef uint32_t   MTYPE;

//*******************************************************************
// todo
WORD  hton( const WORD  in );
DWORD hton( const DWORD in );

WORD  ntoh( const WORD  in );
DWORD ntoh( const DWORD in );

//*******************************************************************
#define PROGMEM // nothing to do
#define GET_BYTE_PROGMEM(varName)       (varName)
#define GET_BYTE_PROGMEM_PTR(ptrName)   (*(ptrName))

#endif //_MCU_TYPES_H
