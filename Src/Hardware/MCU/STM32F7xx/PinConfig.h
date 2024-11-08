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
#if defined       MCU_STM32F769x
  #include "PinConfig_STM32F769.h"
#elif defined     MCU_STM32F746x
  #include "PinConfig_STM32F746.h"
#else
  #error "PinConfig.h: device type mismatch"
#endif

#endif
