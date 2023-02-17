//*******************************************************************
/*!
\file   PinConfig.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   10.09.2020

License: See file "LICENSE"

\brief Include controller specific header files

All controller specific header files are included here.
*/


//*******************************************************************
#ifndef _HW_PIN_CONFIG_H
#define _HW_PIN_CONFIG_H

//*******************************************************************
#if defined         MCU_STM32L100xC
    #include "PinConfig_STM32L100xC.h"
#else
  #error "PinConfig.h: device type mismatch"
#endif

#endif
