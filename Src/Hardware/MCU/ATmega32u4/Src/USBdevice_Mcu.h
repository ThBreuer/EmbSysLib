//*******************************************************************
/*!
\file   USBdevice_Mcu.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   01.06.02012

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_USB_DEVICE_MCU_H
#define _HW_USB_DEVICE_MCU_H

//*******************************************************************
#include "Hardware/Common/USB/USBdevice.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*! 
\class USBdevice_Mcu

\brief Interface to USB hardware

\example HwUSBdevice.cpp
*/
class USBdevice_Mcu : public USBdevice
{
  public:
    //---------------------------------------------------------------
    /*! Initialize the USB device hardware
        \param desc Complete set of descriptors
    */
    USBdevice_Mcu( USBdeviceDescriptor &desc );

    //---------------------------------------------------------------
    // 
    virtual void start( void );

  private:
    //---------------------------------------------------------------
    //
    virtual void reset( void );
  
    //---------------------------------------------------------------
    virtual void configure( BYTE flag );

    //---------------------------------------------------------------
    virtual void setAddress( BYTE adr = 0 );

    //---------------------------------------------------------------
    virtual void configEP( BYTE epAddr, 
                           WORD epMaxPacketSize, 
                           BYTE epType );

    //---------------------------------------------------------------
    virtual void setStallEP( BYTE epAddr );

    //---------------------------------------------------------------
    virtual void clrStallEP( BYTE epAddr );

    //---------------------------------------------------------------
    virtual void setNAK_EP( BYTE epAddr );

    //---------------------------------------------------------------
    virtual void clrNAK_EP( BYTE epAddr );

    //---------------------------------------------------------------
    virtual WORD readEP( BYTE  epAddr, 
                         BYTE *dataPtr, 
                         WORD  len );

    //---------------------------------------------------------------
    virtual WORD writeEP( BYTE  epAddr, 
                          BYTE *pData, 
                          WORD  cnt );

  public:
    //---------------------------------------------------------------
    // 
    void isrGEN( void );
  
    //---------------------------------------------------------------
    // 
    void isrCOM( void );

  public:
    //---------------------------------------------------------------
    static USBdevice_Mcu *usbPtr;
  
  private:                                   
    static const WORD  USB_EP_NUM = 7;

    bool nak[USB_EP_NUM];

}; //class USBdevice_Mcu

}  } //namespace

#endif
