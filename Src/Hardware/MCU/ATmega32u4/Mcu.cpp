//*******************************************************************
/*!
\file   Mcu.cpp
\author Thomas Breuer
\date   14.03.2011

\brief Include controller specific source code

All controller specific source files are included here. 
*/

//*******************************************************************
extern "C"
{
  void __cxa_pure_virtual( void )
  {
    abort();
  }
}

//-------------------------------------------------------------------
#include "PinConfig.cpp"

//-------------------------------------------------------------------
#include "Src/Adc_Mcu.cpp"
#include "Src/I2Cmaster_Mcu.cpp"
#include "Src/I2Cslave_Mcu.cpp"
#include "Src/Memory_EEPROM.cpp"
#include "Src/Port_Mcu.cpp"
#include "Src/Rtos_Mcu.cpp"
#include "Src/SPImaster_Mcu.cpp"
#include "Src/SPIslave_Mcu.cpp"
#include "Src/Timer_Mcu.cpp"
#include "Src/Uart_Mcu.cpp"
#include "Src/USBdevice_Mcu.cpp"

// EOF
