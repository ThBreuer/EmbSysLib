//*******************************************************************
/*!
\file   USBinterface.cpp
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
// USBdeviceInterface
//
//*******************************************************************
//-------------------------------------------------------------------
USBdeviceInterface::USBdeviceInterface( USBdevice &usbIn,
                                        BYTE       configIdIn,
                                        BYTE       interfIdIn )

:usb(usbIn)

{
  configId = configIdIn;
  interfId = interfIdIn;

  USBdeviceDescriptor::Endpoint  *epDesc = NULL;

  usb.desc.registerInterface( configId, interfId, this );

  for( WORD e = 0;
       (epDesc = (USBdeviceDescriptor::Endpoint  *)usb.desc.getDescriptorEndpoint( configId, interfId, e ).getPtr()) != NULL;
       e++ )
  {
    USBdeviceEndpoint *ptr = new USBdeviceEndpoint( usb,
                                                    this,
                                                    epDesc->bEndpointAddress,
                                                    epDesc->wMaxPacketSize,
                                                    epDesc->bmAttributes );

    usb.desc.registerEndpoint( epDesc->bEndpointAddress, ptr );
  }
  isReady=true;
}

//-------------------------------------------------------------------
void USBdeviceInterface::startTransmission( BYTE epId )
{
  if( isReady == false )
  {
    isReady = true;
    USBdeviceEndpoint *ptr = usb.desc.getEndpoint( epId );
    if( ptr )
      ptr->clrNAK();
  }
}

}  } //namespace

//EOF
