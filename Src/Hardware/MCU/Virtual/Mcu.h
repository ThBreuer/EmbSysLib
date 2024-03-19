//*******************************************************************
/*!
\file   Mcu.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"

\brief Include controller specific header files

All controller specific header files are included here.
*/

//*******************************************************************
#ifndef _HW_MCU_H
#define _HW_MCU_H

//*******************************************************************
#include "Std/Report.h"
#include "Hardware/ReportID_Hw.h"
#include "Sys/WSA.h"
#include "Sys/LibUSBwrapper.h"
#include "Sys/UDPserver.h"

//*******************************************************************
// Delete conflicting macro definitions
#undef ABSOLUTE
#undef RELATIVE

//*******************************************************************
#include "Src/Adc_Virtual.h"
#include "Src/Dac_Virtual.h"
#include "Src/DisplayGraphic_Virtual.h"
#include "Src/DisplayChar_Virtual.h"
#include "Src/Memory_Mcu.h"
#include "Src/NetWin.h"
#include "Src/NetWinTCP.h"
#include "Src/NetWinUDP.h"
#include "Src/Port_Virtual.h"
#include "Src/Rtc_Mcu.h"
#include "Src/Rtos_Mcu.h"
#include "Src/Timer_Mcu.h"
#include "Src/Touch_Virtual.h"
#include "Src/Uart_File.h"
#include "Src/Uart_Serial.h"
#include "Src/Uart_Stdio.h"
#include "Src/USBhost_Mcu.h"

#endif //_HW_MCU_H
