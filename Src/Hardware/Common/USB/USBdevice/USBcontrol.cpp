//*******************************************************************
/*!
\file   USBcontrol.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"
*/

//*******************************************************************
#include "../USBdevice.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// USBdeviceControl
//
//*******************************************************************
//-------------------------------------------------------------------
USBdeviceControl::USBdeviceControl( USBdevice &usbPtr )

: usb  ( usbPtr ),
  epIN ( usbPtr, 0x80, usb.desc.getMaxPacketSize0() ),
  epOUT( usbPtr, 0x00, usb.desc.getMaxPacketSize0() )

{
  DeviceStatus = usb.desc.getConfigDeviceStatus( 1 );
}

//-------------------------------------------------------------------
void USBdeviceControl::setup( void )
{
  WORD cnt = usb.readEP( 0x00, (BYTE *)&setupData, sizeof(setupData) );
                         // ^--- this is a control endpoint, we read from 0x00

  if( cnt ==  sizeof( setupData ) )
  {
    switch( setupData.reqRecipient() )
    {
      case REQ_RECIPIENT_DEVICE:     ReqDevice   (); return;
      case REQ_RECIPIENT_INTERFACE:  ReqInterface(); return;
      case REQ_RECIPIENT_ENDPOINT:   ReqEndpoint (); return;
    }
  }
  stall();
}

//-------------------------------------------------------------------
void USBdeviceControl::in( void )
{
  if( setupData.reqDir() == REQ_DIR_DEVICE_TO_HOST )
  {
    switch(setupData.reqRecipient())
    {
      case REQ_RECIPIENT_DEVICE:    epIN.writeData(); return;
      case REQ_RECIPIENT_INTERFACE: epIN.inCtrl();    return;
      case REQ_RECIPIENT_ENDPOINT:                    return;
    }
  }
}

//-------------------------------------------------------------------
void USBdeviceControl::out( void )
{
  if( setupData.reqDir() == REQ_DIR_HOST_TO_DEVICE )
  {
    switch( setupData.reqRecipient() )
    {
      case REQ_RECIPIENT_DEVICE:                     return;
      case REQ_RECIPIENT_INTERFACE: epOUT.outCtrl(); return;
      case REQ_RECIPIENT_ENDPOINT:                   return;
    }
  }
}

//*******************************************************************
// Handle requests
//*******************************************************************
//-------------------------------------------------------------------
void USBdeviceControl::ReqDevice( void )
{
  switch( setupData.reqType() )
  {
    case REQ_TYPE_STANDARD:
      switch( setupData.request() )
      {
        case REQUEST_GET_STATUS:        ReqDeviceStd_GetStatus();        return;
        case REQUEST_CLR_FEATURE:       break;                           // not supported
        case REQUEST_RESERVED_0:        break;                           // not supported
        case REQUEST_SET_FEATURE:       break;                           // not supported
        case REQUEST_RESERVED_1:        break;                           // not supported
        case REQUEST_SET_ADDRESS:       ReqDeviceStd_SetAddress();       return;
        case REQUEST_GET_DESCRIPTOR:    ReqDeviceStd_GetDescriptor();    return;
        case REQUEST_SET_DESCRIPTOR:    break;                           // not supported
        case REQUEST_GET_CONFIGURATION: ReqDeviceStd_GetConfiguration(); return;
        case REQUEST_SET_CONFIGURATION: ReqDeviceStd_SetConfiguration(); return;
        case REQUEST_GET_INTERFACE:     break;                           // not supported
        case REQUEST_SET_INTERFACE:     break;                           // not supported
        case REQUEST_SYNC_FRAME:        break;                           // not supported
      }
      break;

    case REQ_TYPE_CLASS:
      break;

    case REQ_TYPE_VENDOR:
      ReqDeviceVendor(); return;
  }
  stall();
}

//-------------------------------------------------------------------
void USBdeviceControl::ReqInterface( void )
{
  switch( setupData.reqType() )
  {
    case REQ_TYPE_STANDARD:
      switch( setupData.request() )
      {
        case REQUEST_GET_STATUS:        ReqInterfaceStd_GetStatus();     return;
        case REQUEST_CLR_FEATURE:       break;                           // not supported
        case REQUEST_RESERVED_0:        break;                           // not supported
        case REQUEST_SET_FEATURE:       break;                           // not supported
        case REQUEST_RESERVED_1:        break;                           // not supported
        case REQUEST_SET_ADDRESS:       break;                           // not supported
        case REQUEST_GET_DESCRIPTOR:    ReqInterfaceStd_GetDescriptor(); return;
        case REQUEST_SET_DESCRIPTOR:    break;                           // not supported
        case REQUEST_GET_CONFIGURATION: break;                           // not supported
        case REQUEST_SET_CONFIGURATION: break;                           // not supported
        case REQUEST_GET_INTERFACE:     ReqInterfaceStd_GetInterface();  return;
        case REQUEST_SET_INTERFACE:     ReqInterfaceStd_SetInterface();  return;
        case REQUEST_SYNC_FRAME:        break;                           // not supported
      }
      break;

    case REQ_TYPE_CLASS:
      ReqInterfaceClass(); return;

    case REQ_TYPE_VENDOR:
      break;
  }
  stall();
}

//-------------------------------------------------------------------
void USBdeviceControl::ReqEndpoint()
{
  switch( setupData.reqType() )
  {
    case REQ_TYPE_STANDARD:
      switch( setupData.request() )
      {
        case REQUEST_GET_STATUS:        break;                        // not supported
        case REQUEST_CLR_FEATURE:       ReqEndpointStd_ClrFeature();     return;
        case REQUEST_RESERVED_0:        break;                        // not supported
        case REQUEST_SET_FEATURE:       ReqEndpointStd_SetFeature();     return;
        case REQUEST_RESERVED_1:        break;                        // not supported
        case REQUEST_SET_ADDRESS:       break;                        // not supported
        case REQUEST_GET_DESCRIPTOR:    break;                        // not supported
        case REQUEST_SET_DESCRIPTOR:    break;                        // not supported
        case REQUEST_GET_CONFIGURATION: break;                        // not supported
        case REQUEST_SET_CONFIGURATION: break;                        // not supported
        case REQUEST_GET_INTERFACE:     break;                        // not supported
        case REQUEST_SET_INTERFACE:     break;                        // not supported
        case REQUEST_SYNC_FRAME:        break;                        // not supported
      }
      break;

    case REQ_TYPE_CLASS:
      break;

    case REQ_TYPE_VENDOR:
      break;
  }
  stall();
}

//*******************************************************************
// Methods, called in ReqDevice
//*******************************************************************
//-------------------------------------------------------------------
void USBdeviceControl::ReqDeviceStd_GetStatus( void )
{
  epIN.writeData( DataPointer( (BYTE*)&DeviceStatus, setupData.length()) );
}

//-------------------------------------------------------------------
void USBdeviceControl::ReqDeviceStd_SetAddress( void )
{
  usb.setAddress( setupData.valueLow() );
}

//-------------------------------------------------------------------
void USBdeviceControl::ReqDeviceStd_GetDescriptor( void )
{
  switch( setupData.valueHigh() )
  {
    case USBdeviceDescriptor::DEVICE_DESCRIPTOR:
      epIN.writeDataLen( usb.desc.getDescriptorDevice(),
                         setupData.length() );
      return;

    case USBdeviceDescriptor::CONFIGURATION_DESCRIPTOR:
      epIN.writeDataLen( usb.desc.getDescriptorConfig( setupData.valueLow() + 1 ),
                         setupData.length() );
      return;

    case USBdeviceDescriptor::STRING_DESCRIPTOR:
      epIN.writeDataLen( usb.desc.getDescriptorString( setupData.valueLow() ),
                         setupData.length() );
      return;

    case USBdeviceDescriptor::INTERFACE_POWER_DESCRIPTOR:
      return;

    case USBdeviceDescriptor::DEVICE_QUALIFIER_DESCRIPTOR:
      return;
  }
  stall();
}

//-------------------------------------------------------------------
void USBdeviceControl::ReqDeviceStd_GetConfiguration( void )
{
  epIN.writeData( DataPointer( (BYTE*)&Configuration,
                               setupData.length()) );
}

//-------------------------------------------------------------------
 void USBdeviceControl::ReqDeviceStd_SetConfiguration( void )
{
  USBdeviceDescriptor::Endpoint *epDesc   = NULL;
  USBdeviceEndpoint             *ep       = NULL;
  USBdeviceInterface            *interf   = NULL;
  BYTE                           configId = setupData.valueLow();

  if( (BYTE *)usb.desc.getDescriptorConfig( configId ) != NULL )
  {
    Configuration = configId;
    usb.configure(true);

    DeviceStatus = usb.desc.getConfigDeviceStatus( configId );

    // Iteration ueber alle in einer Configuration definierten Endpunkte
    for( WORD i = 0; (BYTE *)usb.desc.getDescriptorInterface( configId, i ) != NULL; i++ )
    {
      for( WORD e=0; (epDesc = usb.desc.getDescriptorEndpoint( configId, i, e )) != NULL; e++ )
      {
        if( (ep = usb.desc.getEndpoint( epDesc->bEndpointAddress )) != NULL )
        {
          ep->setConfiguration();
        }
      }
      interf = usb.desc.getInterface( configId, i );
      if( interf )
        interf->onStart();
    }
    epIN.writeAcknowledge();
  }
  else
  {
    Configuration = 0;
    usb.configure(false);
    stall();
  }
}

void USBdeviceControl::ReqDeviceVendor( void )
{
  switch( setupData.index() )
  {
    case 0x0004: epIN.writeDataLen( usb.desc.getDescriptorDeviceVendor(),
                                    setupData.length() );
                 break;
  }
}

//*******************************************************************
// Methods, called in ReqInterface
//*******************************************************************
//-------------------------------------------------------------------
 void USBdeviceControl::ReqInterfaceStd_GetStatus( void )
{
  // not supported
  stall();
}

//-------------------------------------------------------------------
 void USBdeviceControl::ReqInterfaceStd_GetDescriptor( void )
{

  USBdeviceInterface *interf
    = usb.desc.getInterface( setupData.valueLow()+1,  // confId
                             setupData.indexLow() );  // interfId
  if( interf )
  {
    epIN.writeDataLen( interf->onGetDescriptor( setupData.value() ),
                       setupData.length() );
    return;
  }
  stall();
}

//-------------------------------------------------------------------
 void USBdeviceControl::ReqInterfaceStd_GetInterface( void )
{
  // not supported
  stall();
}

//-------------------------------------------------------------------
 void USBdeviceControl::ReqInterfaceStd_SetInterface( void )
{
  // not supported
  stall();
}

//-------------------------------------------------------------------
 void USBdeviceControl::ReqInterfaceClass( void )
{
  DataPointer tmp;

  USBdeviceInterface *interfTmp
    = usb.desc.getInterface( 1, setupData.indexLow() );

  if( interfTmp )
  {
    switch( setupData.reqDir() )
    {
      case REQ_DIR_DEVICE_TO_HOST:
        if( interfTmp->onRequestCtrl_IN( tmp,
                                         setupData.request(),
                                         setupData.value(),
                                         setupData.length() ) )
        {
          epIN.interf = interfTmp;
          epIN.epData = tmp;
          epIN.writeDataLen( tmp, tmp.getSize());
          return;
        }
        break;
      case REQ_DIR_HOST_TO_DEVICE:
        if( interfTmp->onRequestCtrl_OUT( tmp,
                                          setupData.request(),
                                          setupData.value(),
                                          setupData.length() ) )
        {
          epOUT.interf = interfTmp;
          epOUT.epData = tmp;
          epIN.writeAcknowledge();
          return;
        }
        break;
    }
  }

  stall();
}

//*******************************************************************
// Methods, called in ReqEndpoint
//*******************************************************************
//-------------------------------------------------------------------
void USBdeviceControl::ReqEndpointStd_ClrFeature ( void )
{
  USBdeviceEndpoint *ep = usb.desc.getEndpoint( setupData.indexLow() );

  if( ep && Configuration > 0)
  {
    switch( setupData.value() )
    {
      case FEATURE_BIT_ENDPOINT_STALL:
        ep->clrStall();
        epIN.writeAcknowledge();
        return;
    }
  }
  stall();
}

//-------------------------------------------------------------------
void USBdeviceControl::ReqEndpointStd_SetFeature ( void )
{
  USBdeviceEndpoint *ep = usb.desc.getEndpoint( setupData.indexLow() );

  if( ep && Configuration > 0 )
  {
    switch( setupData.valueLow() )
    {
      case FEATURE_BIT_ENDPOINT_STALL:
        ep->stall();
        epOUT.writeAcknowledge();
        return;
    }
  }
  stall();
}

//*******************************************************************
// Other needfull methods
//*******************************************************************
//-------------------------------------------------------------------
void USBdeviceControl::stall( void )
{
  epIN.stall();
}

}  } //namespace

//EOF
