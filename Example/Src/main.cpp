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

//===================================================================
// Device
//===================================================================
                                                                      // Virtual  STM32  ATmega
///#include "Main/Device/DevDigital.cpp"                              //  X        X      X
///#include "Main/Device/DevPointer.cpp"                              //  X        -      -
///#include "Main/Device/DevTaskManager.cpp"                          //  X        X      X
///#include "Main/Device/DevTerminal.cpp"                             //  X        X      X
///#include "Main/Device/DevScreenChar.cpp"                           //  X        X      -
///#include "Main/Device/DevScreenGraphic.cpp"                        //  X        -      -


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
///#include "Main/Hardware/HwI2Cmaster.cpp"                           //  -        X      X(2)
///#include "Main/Hardware/HwI2Cslave.cpp"                            //  -        -      X
///#include "Main/Hardware/HwMemory.cpp"                              //  X        X      X
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
*/

//===================================================================
// Hardware/Peripheral
//===================================================================
                                                                      // Virtual  STM32  ATmega
///#include "Main/Hardware/Peripheral/HwDisplayChar_DIP204spi.cpp"    //  X        X(1)   X
///#include "Main/Hardware/Peripheral/HwDisplayChar_Terminal.cpp"     //  X        X      X
///#include "Main/Hardware/Peripheral/HwMemory_PCF8583.cpp"           //  -        X      -
///#include "Main/Hardware/Peripheral/HwRtc_PCF8583.cpp"              //  -        X      -
///#include "Main/Hardware/Peripheral/HwPort_PCF8574.cpp"             //  -        X      X

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
///#include "Main/Std/StdDataPointer.cpp"                             //  X        X      X
///#include "Main/Std/StdFifo.cpp"                                    //  X        X      X
///#include "Main/Std/StdFlag.cpp"                                    //  X        X      X
///#include "Main/Std/StdReport.cpp"                                  //  X        X      X
///#include "Main/Std/StdSequence.cpp"                                //  X        X      X

//##################################################################

//EOF
