//*******************************************************************
/*!
\file   PinConfig.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   16.10.2022

License: See file "LICENSE"

\brief Include controller specific header files

All controller specific header files are included here.
*/


//*******************************************************************
#ifndef _HW_PIN_CONFIG_H
#define _HW_PIN_CONFIG_H

//*******************************************************************
#if defined       MCU_STM32F407
  #include "PinConfig_STM32F407.h"
#else
  #error "PinConfig.h: device type mismatch"
#endif

#endif
