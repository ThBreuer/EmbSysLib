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
#if defined MCU_STM32F769x || defined MCU_STM32F746x

#else
  #error "device type mismatch"
#endif

//*******************************************************************
#include <stdlib.h>
#include <stdint.h>

//-------------------------------------------------------------------
#include "Sys/stm32f7xx.h"

//*******************************************************************
//
// Summary of clock configuration in system_STM32L1xx.cpp"
//
// These constants doesn't affect the PLCK settings.
// If other clock settings are used, all constants and their usage in
// "Hardware/Src" must be checked.
//
#define system_PCLK          (200000000)
#define system_PCLK_SYSTIC   (system_PCLK)
#define system_PCLK_TIM      (system_PCLK/4)
#define system_PCLK_I2C      (system_PCLK/4)
#define system_PCLK_SPI      (system_PCLK)
#define system_PCLK_UART     (system_PCLK/4)

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
