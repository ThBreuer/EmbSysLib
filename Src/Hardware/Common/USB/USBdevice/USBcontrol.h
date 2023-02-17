//*******************************************************************
/*!
\file   USBcontrol.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_USB_DEVICE_CONTROL_H
#define _HW_USB_DEVICE_CONTROL_H

//*******************************************************************
#include "Mcu_Types.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
class USBdevice;
class USBdeviceEndpoint;

//*******************************************************************
/*!
\class USBdeviceControl

\brief Control Endpoint (IN and OUT)
This class handles any SETUP, IN or OUTR transfer at control enpoint 0
*/
class USBdeviceControl
{
  private:
    //---------------------------------------------------------------
    // \class Setup
    // Data structure of a setup package
    //
    #pragma pack(1)
    class Setup
    {
      private:
        //---------------------------------------------------------------
        BYTE bmRequestType;
        BYTE bRequest;
        WORD wValue;
        WORD wIndex;
        WORD wLength;

      private:
        //---------------------------------------------------------------
        BYTE reqRecipient() { return( (bmRequestType     ) & 0x1F ); }
        BYTE reqType()      { return( (bmRequestType >> 5) & 0x03 ); }
        BYTE reqDir()       { return( (bmRequestType >> 7) & 0x01 ); }
        BYTE request()      { return(  bRequest                   ); }
        WORD value()        { return(  wValue                     ); }
        BYTE valueLow()     { return(  wValue & 0xFF              ); }
        BYTE valueHigh()    { return(  wValue >> 8                ); }
        BYTE index()        { return(  wIndex                     ); }
        BYTE indexLow()     { return(  wIndex & 0xFF              ); }
        BYTE indexHigh()    { return(  wIndex >> 8                ); }
        WORD length()       { return(  wLength                    ); }

      //-----------------------------------------------------------------
      friend class USBdeviceControl;

    }; //Setup
    #pragma pack()
    //*******************************************************************

  private:
    //---------------------------------------------------------------
    enum
    {
      REQ_RECIPIENT_DEVICE    = 0,
      REQ_RECIPIENT_INTERFACE = 1,
      REQ_RECIPIENT_ENDPOINT  = 2,
      REQ_RECIPIENT_OTHER     = 3
    };

    //---------------------------------------------------------------
    enum
    {
      REQ_TYPE_STANDARD  = 0,
      REQ_TYPE_CLASS     = 1,
      REQ_TYPE_VENDOR    = 2,
      REQ_TYPE_RESERVED  = 3
    };

    //---------------------------------------------------------------
    enum
    {
      REQUEST_GET_STATUS        =  0,
      REQUEST_CLR_FEATURE       =  1,
      REQUEST_RESERVED_0        =  2,
      REQUEST_SET_FEATURE       =  3,
      REQUEST_RESERVED_1        =  4,
      REQUEST_SET_ADDRESS       =  5,
      REQUEST_GET_DESCRIPTOR    =  6,
      REQUEST_SET_DESCRIPTOR    =  7,
      REQUEST_GET_CONFIGURATION =  8,
      REQUEST_SET_CONFIGURATION =  9,
      REQUEST_GET_INTERFACE     = 10,
      REQUEST_SET_INTERFACE     = 11,
      REQUEST_SYNC_FRAME        = 12
    };

    //---------------------------------------------------------------
    enum
    {
      REQ_DIR_HOST_TO_DEVICE = 0,
      REQ_DIR_DEVICE_TO_HOST = 1
    };

    //---------------------------------------------------------------
    enum
    {
      FEATURE_BIT_ENDPOINT_STALL = 0x00,
      FEATURE_BIT_REMOTE_WAKEUP  = 0x01
    };

  private:
    //---------------------------------------------------------------
    USBdeviceControl( USBdevice &usbPtr );

    //---------------------------------------------------------------
    virtual void setup( void );

    //---------------------------------------------------------------
    virtual void out( void );

    //---------------------------------------------------------------
    virtual void in( void );

  private:
    //---------------------------------------------------------------
    virtual void stall( void );

  private:
    //---------------------------------------------------------------
    // Top level methods, called in setup()
    void ReqDevice   ( void );
    void ReqInterface( void );
    void ReqEndpoint ( void );

    //---------------------------------------------------------------
    // Methods, called in ReqDevice
    void ReqDeviceStd_GetStatus        ( void );
    void ReqDeviceStd_ClrFeature       ( void );
    void ReqDeviceStd_SetFeature       ( void );
    void ReqDeviceStd_SetAddress       ( void );
    void ReqDeviceStd_GetDescriptor    ( void );
    void ReqDeviceStd_GetConfiguration ( void );
    void ReqDeviceStd_SetConfiguration ( void );

    void ReqDeviceVendor( void );

    //---------------------------------------------------------------
    // Methods, called in ReqInterface
    void ReqInterfaceStd_GetStatus     ( void );
    void ReqInterfaceStd_GetDescriptor ( void );
    void ReqInterfaceStd_GetInterface  ( void );
    void ReqInterfaceStd_SetInterface  ( void );

    void ReqInterfaceClass( void );

    //---------------------------------------------------------------
    // Methods, called in ReqEndpoint
    void ReqEndpointStd_ClrFeature ( void );
    void ReqEndpointStd_SetFeature ( void );

   private:
     //---------------------------------------------------------------
     USBdevice          &usb;
     USBdeviceEndpoint   epIN;
     USBdeviceEndpoint   epOUT;
     Setup               setupData;
     WORD                DeviceStatus;
     BYTE                Configuration;

   //-----------------------------------------------------------------
   friend class USBdevice;

}; //class USBdeviceControl

}  } //namespace

#endif
