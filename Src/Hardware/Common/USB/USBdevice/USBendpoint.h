//*******************************************************************
/*!
\file   USBendpoint.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_USB_DEVICE_ENDPOINT_H
#define _HW_USB_DEVICE_ENDPOINT_H

//*******************************************************************
#include "Mcu_Types.h"
#include "Std/DataPointer.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
class USBdevice;

//*******************************************************************
/*!
\class USBdeviceEndpoint

\brief USB Endpoint handling
*/
class USBdeviceEndpoint
{
  private:
    //---------------------------------------------------------------
    // non-control endpoint:
    USBdeviceEndpoint( USBdevice          &usb,
                       USBdeviceInterface *interfIn,
                       BYTE                id,
                       WORD                maxPacketSize,
                       BYTE                attributes  );

    //---------------------------------------------------------------
    // control endpoint:
    USBdeviceEndpoint( USBdevice &usb,
                       BYTE       id,
                       WORD       maxPacketSize );

    //---------------------------------------------------------------
    void in( void );

    //---------------------------------------------------------------
    void inCtrl( void );

    //---------------------------------------------------------------
    void out( void );

    //---------------------------------------------------------------
    void outCtrl( void );

    //---------------------------------------------------------------
    WORD readData( void );

    //---------------------------------------------------------------
    void writeData( void );

    //---------------------------------------------------------------
    void writeData( const Std::DataPointer &epDataIn );

    //---------------------------------------------------------------
    void writeDataLen( const Std::DataPointer &epDataIn,
                       WORD                    maxLen );

    //---------------------------------------------------------------
    void writeAcknowledge( void );

    //---------------------------------------------------------------
    void readAcknowledge( void );

    //-------------------------------------------------------------------
    void stall( void );

    //-------------------------------------------------------------------
    void clrStall( void );

    //-------------------------------------------------------------------
    void setNAK( void );

    //-------------------------------------------------------------------
    void clrNAK( void );

    //-------------------------------------------------------------------
    void clrFeature( void );

    //---------------------------------------------------------------
    void setConfiguration( void );

  private:
    //-------------------------------------------------------------------
    USBdevice           &usb;
    USBdeviceInterface  *interf;
    Std::DataPointer     epData;
    BYTE                 id;
    WORD                 maxPacketSize;
    BYTE                 attributes;
    bool                 addZLP;
    WORD                 totalLen;

  //---------------------------------------------------------------------
  friend class USBdevice;
  friend class USBdeviceControl;
  friend class USBdeviceInterface;

}; //class USBdeviceEndpoint

}  } //namespace

#endif
