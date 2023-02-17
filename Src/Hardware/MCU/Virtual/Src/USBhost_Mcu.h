//*******************************************************************
/*!
\file   USBhost_Mcu.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   15.12.2021

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_USB_HOST_MCU_H
#define _HW_USB_HOST_MCU_H

//*******************************************************************
#include <windows.h>

#include "Hardware/Common/USB/USBhost.h"
#include "Sys/libusb.h"
#include "Sys/LibUSBwrapper.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class USBhost_Mcu

\brief Interface to USB hardware

\example HwUSBhost.cpp
*/
class USBhost_Mcu : public USBhost
{
  public:
    //---------------------------------------------------------------
    USBhost_Mcu( void );

    //---------------------------------------------------------------
    /*! Open an USB connection
        \param vid vendor ID
        \param pid product ID
        \param iid Interface ID
    */
    virtual void open( WORD vid, WORD pid, WORD iid );

    //---------------------------------------------------------------
    virtual void close( void );

    //---------------------------------------------------------------
    virtual bool isConnected( void );

  private:
    //---------------------------------------------------------------
    virtual bool readInterrupt_private( BYTE *data, WORD size );

    //---------------------------------------------------------------
    virtual bool writeInterrupt_private( BYTE *data, WORD size );

    //---------------------------------------------------------------
    virtual bool readCtrl_private( BYTE *data, WORD size );

    //---------------------------------------------------------------
    virtual bool writeCtrl_private( BYTE *data, WORD size );

  private:
    //---------------------------------------------------------------
    bool checkDevice( libusb_device *dev, WORD vid, WORD pid, BYTE iid );

  private:
    //---------------------------------------------------------------
    int iID;
    int ep_IN;
    int ep_OUT;

    WORD maxPacketSizeOUT;

    LibUSBwrapper         libusb;
    libusb_device_handle* devHandle;

}; //class USBhost_Mcu

}  } //namespace

#endif
