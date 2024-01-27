//*******************************************************************
/*!
\file   USBhost_Mcu.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   10.11.2016

License: See file "LICENSE"
*/

//*******************************************************************
#include <stdio.h>
#include "Sys/libusb.h"
#include "Hardware/ReportID_Hw.h"
#include "USBhost_Mcu.h"

#include <iostream>
using namespace std;

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// USBhost_Mcu
//
//*******************************************************************
//-------------------------------------------------------------------
USBhost_Mcu::USBhost_Mcu( void )

: USBhost( ReportID_Hw::Module::USB_HOST_MCU )

{
  iID       = 0;
  ep_IN     = 0x80;
  ep_OUT    = 0;
  devHandle = NULL;
}

//-------------------------------------------------------------------
bool USBhost_Mcu::checkDevice(libusb_device *dev, WORD vid, WORD pid, BYTE iid )
{
  bool ret = false;

	libusb_device_descriptor desc;
	if( libusb.get_device_descriptor(dev, &desc) == false )
	{
		return( false );
	}

  libusb_config_descriptor *config;

  if( libusb.get_config_descriptor(dev, 0, &config) == true )
  {
    if( desc.idVendor == vid && desc.idProduct == pid )
    {
      if( config->bNumInterfaces >= iid )
      {
        ret = true;
      }
    }
  }
  libusb.free_config_descriptor(config);
  return( ret );
}

//-------------------------------------------------------------------
void USBhost_Mcu::open( WORD vid, WORD pid, WORD iid)
{
  close();

  libusb.init( NULL );

  libusb_device **deviceList;
  int numOfDevice = libusb.get_device_list( NULL, &deviceList );

  devHandle = NULL;
  //int err = 0;
  for( int d=0;d<numOfDevice;d++)
  {
    libusb_device *device = deviceList[d];

    if( checkDevice( device, vid, pid, iid ) )
    {
      if( libusb.open(device, &devHandle) == false )
      {
        report.alert( ReportID_Hw::Event::OPEN_ERROR );
        close();
      }
      else
      {
       break;
      }
    }
  }

  if( devHandle )
  {
    if( libusb.claim_interface( devHandle, iid ) == false )
    {
      report.alert( ReportID_Hw::Event::OPEN_ERROR );
      close();
    }
  }

  ///  libusb_free_device_list(deviceList,0);

  if( devHandle )
  {
    struct libusb_config_descriptor *config;

    libusb.get_active_config_descriptor( libusb.get_device(devHandle),&config );

    for( int e = 0; e < config->interface[iid].altsetting->bNumEndpoints; e++ )
    {
      int ep = config->interface[iid].altsetting->endpoint[e].bEndpointAddress;

      if( ep&0x80 )
      {
        ep_IN = ep;
      }
      else
      {
        ep_OUT = ep;
        maxPacketSizeOUT = config->interface[iid].altsetting->endpoint[e].wMaxPacketSize;
      }
    }

    this->iID = iid;

    libusb.free_config_descriptor( config );
  }
  else
  {
    report.alert( ReportID_Hw::Event::NO_DEVICE_HANDLE );
  }
}

//-------------------------------------------------------------------
void USBhost_Mcu::close( void )
{
  if( devHandle )
  {
    libusb.release_interface( devHandle, iID );
    libusb.close            ( devHandle      );
    libusb.exit             ( NULL           );
  }
  devHandle = NULL;
}

//---------------------------------------------------------------
bool USBhost_Mcu::isConnected( void )
{
  return( (devHandle != NULL) );
}

//-------------------------------------------------------------------
bool USBhost_Mcu::readInterrupt_private( BYTE *data, WORD size )
{
  int success  = false;

  if( devHandle == NULL )
  {
    report.alert( ReportID_Hw::Event::NO_DEVICE_HANDLE );
    return( false );
  }

  BYTE buffer[size+1];
  WORD pos=0;

  int transfered = 0;
  success = libusb.interrupt_transfer( devHandle, ep_IN, buffer, size+1, &transfered, 100 );
  //!< \todo check success before size
  //!< \todo allow transfered = 0 or transfered < size? return transfered?

  if( 0 <= transfered && transfered <= size )
  {
    memcpy( data, buffer, transfered);
  }
  else
  {
    report.alert( ReportID_Hw::Event::SIZE_ERROR );
    return( false );
  }


 if( success == false )
  {
    close();
    report.alert( ReportID_Hw::Event::READ_ERROR );
    return( false );
  }
  return( true );
}

//-------------------------------------------------------------------
bool USBhost_Mcu::readCtrl_private( BYTE *data, WORD size )
{
  int success = false;

   BYTE buffer[1024];

  if( devHandle == NULL )
  {
    report.alert( ReportID_Hw::Event::OPEN_ERROR );
    return( false );
  }

  int transfered = 0;
  success = libusb.control_transfer( devHandle, 0x21|0x80, 0x09, 0, iID, buffer, size, &transfered, 500 );

  if( transfered != size )
  {
    report.alert( ReportID_Hw::Event::SIZE_ERROR );
    return( false );
  }

  if( success == false )
  {
    close();
    report.alert( ReportID_Hw::Event::READ_ERROR );
    return( false );
  }
  else
  {
     memcpy( data, buffer, transfered);
  }
  return( true );
}

//-------------------------------------------------------------------
bool USBhost_Mcu::writeInterrupt_private( BYTE *data, WORD size )
{
  int success       = false;

  if ( devHandle == NULL )
  {
    report.alert( ReportID_Hw::Event::OPEN_ERROR );
    return( false );
  }


  int transfered = 0;
  success = libusb.interrupt_transfer( devHandle, ep_OUT, data, size, &transfered, 500 );
  if( transfered != size )
  {
    report.alert( ReportID_Hw::Event::SIZE_ERROR );
    return( false );
  }
  if( success  && size%maxPacketSizeOUT == 0 )  //send ZLP ?
  {
    success = libusb.interrupt_transfer( devHandle, ep_OUT, data, 0, &transfered, 500 );
  }

  if( success == false )
  {
    close();
    report.alert( ReportID_Hw::Event::WRITE_ERROR );
    return( false );
  }
  return( true );
}

//-------------------------------------------------------------------
bool USBhost_Mcu::writeCtrl_private( BYTE *data, WORD size )
{
  int success       = false;

  if ( devHandle == NULL )
  {
    report.alert( ReportID_Hw::Event::NO_DEVICE_HANDLE );
    return( false );
  }

  int transfered = 0;
  success = libusb.control_transfer( devHandle, 0x21, 0x09, 0x0200, iID, data, size, &transfered,500 );

  if( transfered != size )
  {
    report.alert( ReportID_Hw::Event::SIZE_ERROR );
    return( false );
  }

  if( success == false )
  {
    close();
    report.alert( ReportID_Hw::Event::WRITE_ERROR );
    return( false );
  }
  return( true );
}

}  } //namespace

//EOF
