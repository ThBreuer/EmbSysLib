//*******************************************************************
/*!
\file   main.cpp
\author Thomas Breuer
\date   07.11.2022
\brief  Samples

This file is used to simply select the example without
modifying the IDE project file

Attention:
  - Select exactly one example at a time!
  - Not all examples are suitable for all targets
*/

//*******************************************************************
// Uncomment the desired example file.
//*******************************************************************

//===================================================================
// Demo
//===================================================================
                                                                      // Virtual  STM32  ATmega
///
#include "Main/Demo/Blinky/main.cpp"                               //  X        X      X
///#include "Main/Demo/LED/main.cpp"                                  //  X        X      X

//===================================================================
// Device
//===================================================================
                                                                      // Virtual  STM32  ATmega
///#include "Main/Device/DevAnalog.cpp"                               //  X        X     (1)
///#include "Main/Device/DevDigital.cpp"                              //  X        X      X
///#include "Main/Device/DevPointer.cpp"                              //  X        -      -
///#include "Main/Device/DevTaskManager.cpp"                          //  X        X      X
///#include "Main/Device/DevTerminal.cpp"                             //  X        X      X
///#include "Main/Device/DevScreenChar.cpp"                           //  X        X      -
///#include "Main/Device/DevScreenGraphic.cpp"                        //  X        -      -
/*
  (1) Todo: add implementation in config.h
*/

//===================================================================
// Control
//===================================================================
                                                                      // Virtual  STM32  ATmega
///#include "Main/Control/CtrlDigitalIndicator.cpp"                   //  X        X      X
///#include "Main/Control/CtrlDigitalButton.cpp"                      //  X        X      X
///#include "Main/Control/CtrlDigitalEncoder.cpp"                     //  X        X(1)   -

/*
  (1) STM32F407-Discovery only
*/

//===================================================================
// Hardware
//===================================================================
                                                                      // Virtual  STM32  ATmega
///#include "Main/Hardware/HwAdc.cpp"                                 //  X        X      X
///#include "Main/Hardware/HwDac.cpp"                                 //  X        X      -
///#include "Main/Hardware/HwDisplayChar.cpp"                         //  X        X(1)   -
///#include "Main/Hardware/HwDisplayGraphic.cpp"                      //  X        X(1)   -
///#include "Main/Hardware/HwEncoder.cpp"                             //  -        X      -
///#include "Main/Hardware/HwExtInt.cpp"                              //  -        X(3)   -
///#include "Main/Hardware/HwI2Cmaster.cpp"                           //  -        X      X(2)
///#include "Main/Hardware/HwI2Cslave.cpp"                            //  -        -      X
///#include "Main/Hardware/HwMemory.cpp"                              //  X        X      X
///#include "Main/Hardware/HwNet_TCP.cpp"                             //  X        X      -
///#include "Main/Hardware/HwNet_UDP.cpp"                             //  X        X      -
///#include "Main/Hardware/HwPort.cpp"                                //  X        X      X
///#include "Main/Hardware/HwPortPin.cpp"                             //  X        X      X
///#include "Main/Hardware/HwRtc.cpp"                                 //  X        X      -
///#include "Main/Hardware/HwRtos.cpp"                                //  X        X      X
///#include "Main/Hardware/HwSPImaster.cpp"                           //  -        X      X(2)
///#include "Main/Hardware/HwSPIslave.cpp"                            //  -        -      X
///#include "Main/Hardware/HwTimer.cpp"                               //  X        X      X
///#include "Main/Hardware/HwTouch.cpp"                               //  X        -      -
///#include "Main/Hardware/HwUart.cpp"                                //  X        X      X
///#include "Main/Hardware/HwUSBdevice.cpp"                           //  -        X      X
///#include "Main/Hardware/HwUSBhost.cpp"                             //  X        -      -

/*
  (1) STM32F407-Discovery only
  (2) not tested
  (3) STM32-Nucleo32-L432 and STM32F4-Discovery
*/

//===================================================================
// Hardware/Peripheral
//===================================================================
                                                                      // Virtual  STM32  ATmega
///#include "Main/Hardware/Peripheral/HwDisplayChar_DIP204spi.cpp"    //  X        X(1)   X
///#include "Main/Hardware/Peripheral/HwDisplayChar_Terminal.cpp"     //  X        X      X
///#include "Main/Hardware/Peripheral/HwDisplayGraphic_SSD1306i2c.cpp" //           X(2)
///#include "Main/Hardware/Peripheral/HwMemory_PCF8583.cpp"           //  -        X      -
///#include "Main/Hardware/Peripheral/HwPort_PCF8574.cpp"             //  -        X      X
///#include "Main/Hardware/Peripheral/HwRtc_PCF8583.cpp"              //  -        X      -

/*
  (1) STM32F407-Discovery only
  (2) STM32-Nucleo32-L432 only
*/

//===================================================================
// Module
//===================================================================
//-------------------------------------------------------------------
// Module/Isc
//-------------------------------------------------------------------
                                                                      // Virtual  STM32  ATmega
///#include "Main/Module/Isc/ModIsc.cpp"                             //  X        X      X

//-------------------------------------------------------------------
// Module/Rtos
//-------------------------------------------------------------------
                                                                      // Virtual  STM32  ATmega
///#include "Main/Module/Rtos/ModRtos.cpp"                            //  X        X      X

//-------------------------------------------------------------------
// Module/USB
//-------------------------------------------------------------------
                                                                      // Virtual  STM32  ATmega
///#include "Main/Module/USB/ModUSB_Uart.cpp"                         // -         X(1)   X
///#include "Main/Module/USB/ModUSBdeviceSimpleIO.cpp"                // -         X      X
///#include "Main/Module/USB/ModUSBkeyboard.cpp"                      // -         X      X
///#include "Main/Module/USB/ModUSBmouse.cpp"                         // -         X      X
///#include "Main/Module/USB/ModUSBvolume.cpp"                        // -         X      X

/*
  (1) STM32F100-Discovery only
*/

//===================================================================
// Std
//===================================================================
                                                                      // Virtual  STM32  ATmega
///#include "Main/Std/StdClock.cpp"                                   //  X        X      X
///#include "Main/Std/StdCrc.cpp"                                     //  X        X      X
///#include "Main/Std/StdDataPointer.cpp"                             //  X        X      X
///#include "Main/Std/StdFifo.cpp"                                    //  X        X      X
///#include "Main/Std/StdFlag.cpp"                                    //  X        X      X
///#include "Main/Std/StdReport.cpp"                                  //  X        X      X
///#include "Main/Std/StdSequence.cpp"                                //  X        X      X
///#include "Main/Std/StdSharedMem.cpp"                               //  X        X      X

//##################################################################

//EOF
