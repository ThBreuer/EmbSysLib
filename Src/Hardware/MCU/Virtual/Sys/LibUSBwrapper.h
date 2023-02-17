//*******************************************************************
/*!
\file   LibUSBwrapper.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   18.11.2021

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _LIB_USB_WRAPPER_H
#define _LIB_USB_WRAPPER_H

//*******************************************************************
#include "Sys/libusb.h"

#include "Std/Report.h"
#include "Hardware/ReportID_Hw.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class LibUSBwrapper

\brief Wrapper for libusb library functions (\see http://libusb.info)
*/
//*******************************************************************
class LibUSBwrapper
{
  public:
    //---------------------------------------------------------------
    /*! Constructor
    */
    LibUSBwrapper( void )
    : report( ReportID_Hw::Module::SYSTEM_LIB_USB )
    {
    }

    //---------------------------------------------------------------
    bool init( libusb_context ** ctx )
    {
      int r = libusb_init( ctx );
      if( r < 0 )
      {
        report.alert( -r );
        return( false );
      }
      return( true );
    }

    //---------------------------------------------------------------
    WORD get_device_list( libusb_context *    ctx,
                          libusb_device ***   list )
    {
      int numOfDevice = libusb_get_device_list( ctx, list );
      if( numOfDevice < 0 )
      {
        report.alert( -numOfDevice );
        return( 0 );
      }
      return( numOfDevice );
    }

    //---------------------------------------------------------------
    bool open( libusb_device         *dev,
               libusb_device_handle **dev_handle )
    {
      int r = libusb_open( dev, dev_handle );
      if( r < 0 )
      {
        report.alert( -r );
        return( false );
      }
      return( true );
    }

    //---------------------------------------------------------------
    bool claim_interface( libusb_device_handle *dev_handle,
                          int                   interface_number )
    {
      int r = libusb_claim_interface( dev_handle, interface_number );
      if( r < 0 )
      {
        report.alert( -r );
        return( false );
      }
      return( true );
    }

    //---------------------------------------------------------------
    bool get_active_config_descriptor( libusb_device                    *dev,
                                       struct libusb_config_descriptor **config )
    {
      int r = libusb_get_active_config_descriptor( dev, config );
      if( r < 0 )
      {
        report.alert( -r );
        return( false );
      }
      return( true );
    }

    //---------------------------------------------------------------
    libusb_device *get_device( libusb_device_handle *dev_handle )
    {
      return( libusb_get_device( dev_handle ) );
    }

    //---------------------------------------------------------------
    void free_config_descriptor( libusb_config_descriptor *config )
    {
      libusb_free_config_descriptor( config );
    }

    //---------------------------------------------------------------
    void exit(   libusb_context *ctx)
    {
      libusb_exit( ctx );
    }

    //---------------------------------------------------------------
    bool release_interface( libusb_device_handle *dev_handle,
                            int                   interface_number )
    {
      int r = libusb_release_interface( dev_handle, interface_number );
      if( r < 0 )
      {
        report.alert( -r );
        return( false );
      }
      return( true );
    }

    void close ( libusb_device_handle *dev_handle )
    {
      libusb_close( dev_handle );
    }

    //---------------------------------------------------------------
    bool interrupt_transfer( libusb_device_handle *dev_handle,
                             unsigned char         endpoint,
                             unsigned char        *data,
                             int                   length,
                             int                  *transferred,
                             unsigned int          timeout )
    {
      int r = libusb_interrupt_transfer( dev_handle, endpoint, data, length, transferred, timeout );
      if( r < 0 )
      {
        report.alert( -r );
        return( false );
      }
      return( true );
    }

    //---------------------------------------------------------------
    bool control_transfer( libusb_device_handle *dev_handle,
                           uint8_t               bmRequestType,
                           uint8_t               bRequest,
                           uint16_t              wValue,
                           uint16_t              wIndex,
                           unsigned char        *data,
                           uint16_t              wLength,
                           int                  *transferred,
                           unsigned int          timeout )
    {
      int r = libusb_control_transfer( dev_handle, bmRequestType, bRequest, wValue, wIndex, data, wLength, timeout );
      if( r < 0 )
      {
        *transferred = 0;
        report.alert( -r );
        return( false );
      }
     *transferred = r;
      return( true );
    }

    //---------------------------------------------------------------
    bool get_device_descriptor( libusb_device            *dev,
                                libusb_device_descriptor *desc )
    {
      int r = libusb_get_device_descriptor( dev, desc );
      if (r < 0)
      {
        report.alert( -r );
        return( false );
      }
      return( true );
    }

    //---------------------------------------------------------------
    bool get_config_descriptor( libusb_device             *dev,
                                uint8_t                    config_index,
                                libusb_config_descriptor **config )
    {
      int r = libusb_get_config_descriptor( dev, config_index, config );
      if (r < 0)
      {
        report.alert( -r );
        return( false );
      }
      return( true );
    }

  public:
    //---------------------------------------------------------------
    Std::Report report;

}; // LibUSBwrapper

}  } //namespace

#endif
