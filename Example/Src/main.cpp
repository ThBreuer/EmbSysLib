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
#include "Main/Demo/Blinky/main.cpp"                               //  x        x      x

//===================================================================
// Device
//===================================================================
                                                                      // Virtual  STM32  ATmega
///#include "Main/Device/DevDigital.cpp"                              //  x        x      x
///#include "Main/Device/DevPointer.cpp"                              //  x        -      -
///#include "Main/Device/DevTaskManager.cpp"                          //  x        x      x
///#include "Main/Device/DevTerminal.cpp"                             //  x        x      x
///#include "Main/Device/DevScreenChar.cpp"                           //  x        x      -
///#include "Main/Device/DevScreenGraphic.cpp"                        //  x        -      -


//===================================================================
// Control
//===================================================================
                                                                      // Virtual  STM32  ATmega
///#include "Main/Control/CtrlDigitalIndicator.cpp"                   //  x        x      x
///#include "Main/Control/CtrlDigitalButton.cpp"                      //  x        x      x
///#include "Main/Control/CtrlDigitalEncoder.cpp"                     //  x        x(1)   -

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
///#include "Main/Hardware/HwI2Cmaster.cpp"                           //  -        X      x
///#include "Main/Hardware/HwI2Cslave.cpp"                            //  -        -      x
///#include "Main/Hardware/HwMemory.cpp"                              //  X        X      X
///#include "Main/Hardware/HwPort.cpp"                                //  X        X      X
///#include "Main/Hardware/HwPortPin.cpp"                             //  X        X      X
///#include "Main/Hardware/HwRtc.cpp"                                 //  X        X      -
///#include "Main/Hardware/HwRtos.cpp"                                //  X        X      X
///#include "Main/Hardware/HwSPImaster.cpp"                           //  todo     todo   todo
///#include "Main/Hardware/HwSPIslave.cpp"                            //  -        -      todo
///#include "Main/Hardware/HwTimer.cpp"                               //  X        X      X
///#include "Main/Hardware/HwTouch.cpp"                               //  X        -      -
///#include "Main/Hardware/HwUart.cpp"                                //  X        X      X
///#include "Main/Hardware/HwUSBdevice.cpp"                           //  -        X      X
///#include "Main/Hardware/HwUSBhost.cpp"                             //  X        -      -

/*
  (1) STM32F407-Discovery only
*/

//===================================================================
// Hardware/Peripheral
//===================================================================
                                                                      // Virtual  STM32  ATmega
///#include "Main/Hardware/Peripheral/HwDisplayChar_DIP204spi.cpp"    //  x        x(1)   -
///#include "Main/Hardware/Peripheral/HwDisplayChar_Terminal.cpp"     //  x        x      x
///#include "Main/Hardware/Peripheral/HwMemory_PCF8583.cpp"           //  -        x      -
///#include "Main/Hardware/Peripheral/HwRtc_PCF8583.cpp"              //  -        x      -
///#include "Main/Hardware/Peripheral/HwPort_PCF8574.cpp"             //  -        x      x

/*
  (1) STM32F407-Discovery only
*/

//===================================================================
// Module
//===================================================================
//-------------------------------------------------------------------
// Module/Rtos
//-------------------------------------------------------------------
                                                                      // Virtual  STM32  ATmega
///#include "Main/Module/Rtos/ModRtos.cpp"                            //  x        x      x

//-------------------------------------------------------------------
// Module/USB
//-------------------------------------------------------------------
                                                                      // Virtual  STM32  ATmega
///#include "Main/Module/USB/ModUSB_Uart.cpp"                         // -         x(1)   x
///#include "Main/Module/USB/ModUSBdeviceSimpleIO.cpp"                // -         x      x
///#include "Main/Module/USB/ModUSBkeyboard.cpp"                      // -         x      x
///#include "Main/Module/USB/ModUSBmouse.cpp"                         // -         x      x
///#include "Main/Module/USB/ModUSBvolume.cpp"                        // -         x      x

/*
  (1) STM32F100-Discovery only
*/

//===================================================================
// Std
//===================================================================
                                                                      // Virtual  STM32  ATmega
///#include "Main/Std/StdDataPointer.cpp"                             //  x        x      x
///#include "Main/Std/StdFifo.cpp"                                    //  x        x      x
///#include "Main/Std/StdFlag.cpp"                                    //  x        x      x
///#include "Main/Std/StdReport.cpp"                                  //  x        x      x
///#include "Main/Std/StdSequence.cpp"                                //  x        x      x

//##################################################################
//##################################################################
//
// Test Bench
//
//##################################################################
//##################################################################

                                                                      // Virtual  STM32  ATmega
///#include "TestBench/HwI2Cslave/HwI2CslaveTest.cpp"                 //  -        x      x

//EOF
