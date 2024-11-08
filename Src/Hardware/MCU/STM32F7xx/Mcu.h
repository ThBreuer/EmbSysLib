//*******************************************************************
/*!
\file   Mcu.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   16.10.2022

License: See file "LICENSE"

\brief Include controller specific header files

All controller specific header files are included here.
*/

//*******************************************************************
#ifndef _HW_MCU_H
#define _HW_MCU_H

//*******************************************************************
#include <stdlib.h>
#include <stdint.h>

//*******************************************************************
#include "Sys/stm32f7xx.h"

//*******************************************************************
#include "Std/Report.h"
#include "Hardware/ReportID_Hw.h"
#include "PinConfig.h"

//*******************************************************************
#include "Src/Adc_Mcu.h"
#include "Src/Dac_Mcu.h"
#include "Src/Ethernet_Mcu.h"
#include "Src/I2Cmaster_Mcu.h"
#include "Src/Memory_BKRAM.h"
#include "Src/Memory_Flash.h"
#include "Src/Port_Mcu.h"
#include "Src/Rtc_Mcu.h"
#include "Src/Rtos_Mcu.h"
#include "Src/Timer_Mcu.h"
#include "Src/Uart_Mcu.h"
#include "Src/USBdevice_Mcu.h"

#if defined MCU_STM32F769x
#include "Src/Dsi_Mcu.h"
#endif

#include "Src/Fmc_Mcu.h"
#include "Src/Ltdc_Mcu.h"

#endif //_HW_MCU_H
