//*******************************************************************
/*!
\file   USBinterfClassHID.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   31.01.2022

License: See file "LICENSE"
*/

//*******************************************************************
#include "USBinterfClassHID.h"

//*******************************************************************
namespace EmbSysLib {
namespace Mod {
  
//*******************************************************************
// 
// USBinterfClassHID
//
//*******************************************************************
//-------------------------------------------------------------------
USBinterfClassHID::USBinterfClassHID( Hw::USBdevice &usb, 
                                      BYTE           configId, 
                                      BYTE           interfId, 
                                      DataPointer    dataIN, 
                                      DataPointer    dataOUT )

: USBdeviceInterface( usb, configId, interfId )
, dataIN (dataIN)
, dataOUT(dataOUT)

{
}

//-------------------------------------------------------------------
DataPointer USBinterfClassHID::onGetDescriptor( WORD value )
{
  switch( value >> 8 )
  {
    case DESCRIPTOR_TYPE_HID:    return( usb.desc.getDescriptorHID      ( configId, interfId, 0 ) );
    case DESCRIPTOR_TYPE_REPORT: return( usb.desc.getDescriptorHIDreport( configId, interfId, 0 ) );
  }
  return( DataPointer() );
}

//-------------------------------------------------------------------
void USBinterfClassHID::onConfigEndpoint( BYTE         epId, 
                                          DataPointer &data, 
                                          WORD         maxPacketSize )
{
  data = DataPointer();
}

//-------------------------------------------------------------------
bool USBinterfClassHID::onTransmit(BYTE epId, DataPointer &data)
{
  isStarted = true;
  data      = dataIN;
  return( onReportTransmit() );
}

//-------------------------------------------------------------------
bool USBinterfClassHID::onReceiveCtrl( WORD len )
{
  return( onReportReceive() );
}

//-------------------------------------------------------------------
bool USBinterfClassHID::onRequestCtrl_IN( DataPointer &dataPtr, 
                                          BYTE         request, 
                                          WORD         value, 
                                          WORD         length )
{
  dataPtr = DataPointer();
  return(false);
}

//-------------------------------------------------------------------
bool USBinterfClassHID::onRequestCtrl_OUT( DataPointer &dataPtr, 
                                           BYTE         request, 
                                           WORD         value, 
                                           WORD         length )
{
  switch( request ) 
  {
    case REQUEST_SET_REPORT: 
      dataPtr = dataOUT;
      return(true);
    
    case REQUEST_SET_IDLE:
    default:
      dataPtr = DataPointer();
      return(true);
  }
}

} } //namespace Mod

//EOF
