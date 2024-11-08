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


/**
  Device
  ------
    V   Virtual
    S1  STM32F407-Discovery
    S2  STM32F746-Discovery
    S3  STM32F769-Discovery
    S4  STM32L100-Discovery
    S5  STM32-Nucleo32-L432
    A1  ATmega32u4-Leonardo


  Test mark
  ---------
    x tested
    * not tested
    - not implemented
    ? todo

**/
//===================================================================
// Demo
//===================================================================
                                                                      //  V  S1 S2 S3 S4 S5 A1
                                                                      //  --------------------
///
#include "Main/Demo/Blinky/main.cpp"                               //  x  x  x  x  x  x  x
///#include "Main/Demo/LED/main.cpp"                                  //  x  x  x  x  x  x  x

//===================================================================
// Device
//===================================================================
                                                                      //  V  S1 S2 S3 S4 S5 A1
                                                                      //  --------------------
///#include "Main/Device/DevAnalog.cpp"                               //  x  -  -  -  -  x  -
///#include "Main/Device/DevDigital.cpp"                              //  x  -  -  -  -  x  -
///#include "Main/Device/DevPointer.cpp"                              //  x  x  x  x  -  -  -
///#include "Main/Device/DevTaskManager.cpp"                          //  x  x  x  x  x  x  x
///#include "Main/Device/DevTerminal.cpp"                             //  x  x  x  x  x  x  x
///#include "Main/Device/DevScreenChar.cpp"                           //  x  x  x  x  x  x  -
///#include "Main/Device/DevScreenGraphic.cpp"                        //  x  x  x  x  -  -  -

//===================================================================
// Control
//===================================================================
                                                                      //  V  S1 S2 S3 S4 S5 A1
                                                                      //  --------------------
///#include "Main/Control/CtrlDigitalIndicator.cpp"                   //  x  x  x  x  x  -  x
///#include "Main/Control/CtrlDigitalButton.cpp"                      //  x  x  x  x  x  -  x
///#include "Main/Control/CtrlDigitalEncoder.cpp"                     //  x  x  *  x  *  *  -

//===================================================================
// Hardware
//===================================================================
                                                                      //  V  S1 S2 S3 S4 S5 A1
                                                                      //  --------------------
///#include "Main/Hardware/HwAdc.cpp"                                 //  x  x  x  x  x  x  x
///#include "Main/Hardware/HwDac.cpp"                                 //  x  x  -  x  x  x  -
///#include "Main/Hardware/HwDisplayChar.cpp"                         //  x  x  x  x  x  *  -
///#include "Main/Hardware/HwDisplayGraphic.cpp"                      //  x  x  x  x  -  *  -
///#include "Main/Hardware/HwEncoder.cpp"                             //  -  x  -  *  x  ?  -
///#include "Main/Hardware/HwExtInt.cpp"                              //  -  x  *  *  x  ?  -
///#include "Main/Hardware/HwI2Cmaster.cpp"                           //  -  x  *  *  x  x  x
///#include "Main/Hardware/HwI2Cslave.cpp"                            //  -  -  -  -  -  -  x
///#include "Main/Hardware/HwMemory.cpp"                              //  x  x  x  x  x  x  x
///#include "Main/Hardware/HwNet_TCP.cpp"                             //  x  x  x  x  *  *  -
///#include "Main/Hardware/HwNet_UDP.cpp"                             //  x  x  x  x  *  *  -
///#include "Main/Hardware/HwPort.cpp"                                //  x  x  x  x  x  x  x
///#include "Main/Hardware/HwPortPin.cpp"                             //  x  x  x  x  x  x  x
///#include "Main/Hardware/HwRtc.cpp"                                 //  x  x  x  x  x  x  -
///#include "Main/Hardware/HwRtos.cpp"                                //  x  x  x  x  x  x  x
///#include "Main/Hardware/HwSPImaster.cpp"                           //  -  x  ?  x  x  ?  ?
///#include "Main/Hardware/HwSPIslave.cpp"                            //  -  -  -  -  -  -  x
///#include "Main/Hardware/HwTimer.cpp"                               //  1  x  x  x  x  x  x
///#include "Main/Hardware/HwTouch.cpp"                               //  x  x  x  x  -  *  -
///#include "Main/Hardware/HwUart.cpp"                                //  x  x  x  x  x  x  x
///#include "Main/Hardware/HwUSBdevice.cpp"                           //  -  x  x  x  x  x  x
///#include "Main/Hardware/HwUSBhost.cpp"                             //  x  -  -  -  -  -  -

/*
  (1) ohne PWM
*/

//===================================================================
// Hardware/Peripheral
//===================================================================
                                                                      //  V  S1 S2 S3 S4 S5 A1
                                                                      //  --------------------
///#include "Main/Hardware/Peripheral/HwDisplayChar_DIP204spi.cpp"    //  -  *  *  *  x  *  x
///#include "Main/Hardware/Peripheral/HwDisplayChar_Terminal.cpp"     //  -  x  x  x  x  *  x
///#include "Main/Hardware/Peripheral/HwDisplayGraphic_SSD1306i2c.cpp"//  -  *  *  *  *  x  -
///#include "Main/Hardware/Peripheral/HwMemory_PCF8583.cpp"           //  -  *  *  *  *  *  -
///#include "Main/Hardware/Peripheral/HwPort_PCF8574.cpp"             //  -  *  *  *  *  *  x
///#include "Main/Hardware/Peripheral/HwRtc_PCF8583.cpp"              //  -  *  *  *  *  *  -
///#include "Main/Hardware/Peripheral/HwEthernet_ENC28J60.cpp"        //  -  *  *  *  *  *  -

//===================================================================
// Module
//===================================================================
//-------------------------------------------------------------------
// Module/Isc
//-------------------------------------------------------------------
                                                                      //  V  S1 S2 S3 S4 S5 A1
                                                                      //  --------------------
///#include "Main/Module/Isc/ModIsc.cpp"                              //  x  *  *  ?  ?  ?  x

//-------------------------------------------------------------------
// Module/Rtos
//-------------------------------------------------------------------
                                                                      //  V  S1 S2 S3 S4 S5 A1
                                                                      //  --------------------
///#include "Main/Module/Rtos/ModRtos.cpp"                            //  x  x  x  x  x  x  x

//-------------------------------------------------------------------
// Module/USB
//-------------------------------------------------------------------
                                                                      //  V  S1 S2 S3 S4 S5 A1
                                                                      //  --------------------
///#include "Main/Module/USB/ModUSB_Uart.cpp"                         //  -  1  1  1  1  1  x
///#include "Main/Module/USB/ModUSBdeviceSimpleIO.cpp"                //  -  x  *  x  x  x  x
///#include "Main/Module/USB/ModUSBkeyboard.cpp"                      //  -  x  x  x  x  x  x
///#include "Main/Module/USB/ModUSBmouse.cpp"                         //  -  x  x  x  x  x  x
///#include "Main/Module/USB/ModUSBvolume.cpp"                        //  -  x  x  x  x  x  x

/**
Sx   Not ok with uVision
(1)  Not ok, bulk transfer not running
**/

//-------------------------------------------------------------------
// Module/Mqtt
//-------------------------------------------------------------------
                                                                      //  V  S1 S2 S3 S4 S5 A1
                                                                      //  --------------------
///#include "Main/Module/Mqtt/ModMqtt.cpp"                            //  x  *  *  x  *  *  -
///#include "Main/Module/Mqtt/ModMqttClient.cpp"                      //  x  *  *  x  *  *  -

/*
  (1) STM32F769-Discovery only
*/

//===================================================================
// Std
//===================================================================
                                                                      //  V  S1 S2 S3 S4 S5 A1
                                                                      //  --------------------
///#include "Main/Std/StdClock.cpp"                                   //  x  *  x  *  x  *  x
///#include "Main/Std/StdCrc.cpp"                                     //  x  *  x  *  x  *  x
///#include "Main/Std/StdDataPointer.cpp"                             //  x  x  *  x  x  x  x
///#include "Main/Std/StdFifo.cpp"                                    //  x  x  *  x  x  x  x
///#include "Main/Std/StdFlag.cpp"                                    //  x  x  *  x  x  x  x
///#include "Main/Std/StdReport.cpp"                                  //  x  x  *  x  x  x  x
///#include "Main/Std/StdSequence.cpp"                                //  x  x  *  x  x  x  x
///#include "Main/Std/StdSharedMem.cpp"                               //  x  *  x  *  x  *  x

//##################################################################

//EOF
