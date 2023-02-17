//*******************************************************************
/*!
\file   USBdescriptor.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"
*/

//*******************************************************************
#include "USBdescriptor.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

using namespace Std;

//*******************************************************************
//
// USBdeviceDescriptor
//
//*******************************************************************
//-------------------------------------------------------------------
DataPointer USBdeviceDescriptor::getDescriptorDevice( void )
{
  return( getDescriptor( DEVICE, 0, 0, 0 ) );
}

//-------------------------------------------------------------------
DataPointer USBdeviceDescriptor::getDescriptorConfig( BYTE confId )
{
  return( getDescriptor( CONFIG, confId, 0, 0 ) );
}

//-------------------------------------------------------------------
DataPointer USBdeviceDescriptor::getDescriptorInterface( BYTE confId,
                                                         BYTE interfId )
{
  return( getDescriptor( INTERF, confId, interfId, 0 ) );
}

//-------------------------------------------------------------------
DataPointer USBdeviceDescriptor::getDescriptorEndpoint( BYTE confId,
                                                        BYTE interfId,
                                                        BYTE epId )
{
  return( getDescriptor( ENDPOINT, confId, interfId, epId ) );
}

//-------------------------------------------------------------------
DataPointer USBdeviceDescriptor::getDescriptorHID( BYTE confId,
                                                   BYTE interfId,
                                                   BYTE hidId )
{
  return( getDescriptor( CLASS, confId, interfId, hidId ) );
}

//-------------------------------------------------------------------
DataPointer USBdeviceDescriptor::getDescriptorHIDreport( BYTE confId,
                                                         BYTE interfId,
                                                         BYTE hidId )
{
  return( getDescriptor( REPORT, confId, interfId, hidId ) );
}

//-------------------------------------------------------------------
DataPointer USBdeviceDescriptor::getDescriptorString( BYTE strId )
{
  return( getDescriptor( STRING, 0, 0, strId ) );
}

//-------------------------------------------------------------------
DataPointer USBdeviceDescriptor::getDescriptorDeviceVendor( void )
{
  return( getDescriptor( DEVICE_VENDOR, 0, 0, 0 ) );
}

//-------------------------------------------------------------------
BYTE USBdeviceDescriptor::getMaxPacketSize0( void )
{
  USBdeviceDescriptor::Device *ptr
    = (USBdeviceDescriptor::Device *)USBdeviceDescriptor::getDescriptorDevice().getPtr();

  if( ptr )
    return( ptr->bMaxPacketSize0 );
  return( 0 );
}

//-------------------------------------------------------------------
BYTE USBdeviceDescriptor::getConfigDeviceStatus( BYTE confId )
{
  USBdeviceDescriptor::Configuration *ptr
    = (USBdeviceDescriptor::Configuration *)USBdeviceDescriptor::getDescriptorConfig( confId ).getPtr();

  BYTE ret = 0;

  if( ptr )
  {
    if( ptr->bmAttributes & Configuration::SELF_POWERED )
    {
      ret |= Device::SELF_POWERED_MASK;
    }
    if( ptr->bmAttributes & Configuration::REMOTE_WAKEUP )
    {
      ret |= Device::REMOTE_WAKEUP_MASK;
    }
  }
  return( ret );
}

}  } //namespace

//EOF
