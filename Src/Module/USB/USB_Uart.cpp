//*******************************************************************
/*!
\file   USB_Uart.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   21.02.2022

License: See file "LICENSE"
*/

//*******************************************************************
#include "USB_Uart.h"
#include "Std/Report.h"
#include "Std/DataPointer.h"
#include "Module/ReportID_Mod.h"

//*******************************************************************
namespace EmbSysLib {
namespace Mod {

using namespace Std;

//*******************************************************************
//
// USB_Uart::CtrlInterface
//
//*******************************************************************
//-------------------------------------------------------------------
USB_Uart::CtrlInterface::CtrlInterface( Hw::USBdevice   &usb, 
                                        BYTE             iid, 
                                        Std::Flag<connectionState_enum> &connection )
: USBdeviceInterface(usb, 1, iid)
, connection( connection )
{
}

//-------------------------------------------------------------------
void USB_Uart::CtrlInterface::onStart( void )
{
  connection = USB_READY; // USB just started, but Terminal not connected
}

//-------------------------------------------------------------------
void USB_Uart::CtrlInterface::onStop( void )
{
  connection = USB_DISCONNECTED;
}

//---------------------------------------------------------------
bool USB_Uart::CtrlInterface::onRequestCtrl_IN( DataPointer &dataPtr, 
                                                BYTE         request,
                                                WORD         value, 
                                                WORD         length )
{
  // Ignore incomming request 0x21: "get line coding" 
  dataPtr = DataPointer();
  return( true );
}

//---------------------------------------------------------------
bool USB_Uart::CtrlInterface::onTransmit( BYTE         epId,
                                          DataPointer &epdata )
{
  connection = CONNECTED;
  epdata = DataPointer();
  return( true );
}

//---------------------------------------------------------------
bool USB_Uart::CtrlInterface::onRequestCtrl_OUT( DataPointer &dataPtr, 
                                                 BYTE         request,
                                                 WORD         value, 
                                                 WORD         length )
{
  // Ignore incomming requests:
  //   0x20: "set line coding"
  //   0x22: "set control line state"
  if( request == 0x22 )
  {
    if( value & 0x01 )
    {
       connection = CONNECTED;
    }
    else
    {
       connection = DISCONNECTED;
    }
  }
  
  dataPtr = DataPointer();
  return( true );
}

//*******************************************************************
//
// USB_Uart
//
//*******************************************************************
//-------------------------------------------------------------------
USB_Uart::USB_Uart( Hw::USBdevice &usb, 
                    BYTE           configId, 
                    BYTE           interfId,
                    WORD           bufferSize )
    : USBdeviceInterface( usb, configId, interfId+1 )
    , Hw::Uart          ( ReportID_Mod::Module::USB_Uart, bufferSize, bufferSize )
    , ctrlInterface     ( usb, interfId, connection )
    {
    }

//-------------------------------------------------------------------
void USB_Uart::onConfigEndpoint( BYTE         epId, 
                                 DataPointer &data, 
                                 WORD         maxPacketSize )
{
  if( epId & 0x80 ) // IN endpoint ?
  {
    epIN =  epId;
    data = DataPointer( NULL, 0 );  // empty transmit buffer
  }
  else              // OUT endpoint
  {
    data = DataPointer( bufOUT, packetSize );
  }
}

//-------------------------------------------------------------------
bool USB_Uart::onTransmit( BYTE epId, DataPointer &data )
{
  BYTE i;
  for( i=0; i<packetSize - 1 && !dataTxBuf.isEmpty(); i++ )
  {
    dataTxBuf >> bufIN[i];
  }
  data = DataPointer( bufIN, i );
  return( i?true:false );
}

//-------------------------------------------------------------------
bool USB_Uart::onReceive( BYTE         epId, 
                          WORD         len, 
                          DataPointer &epdata )
{
  for( BYTE i=0; i<len; i++ )
  {
    dataRxBuf << bufOUT[i];
  }
  epdata = DataPointer( bufOUT, packetSize );
  return( true );
}

//-------------------------------------------------------------------
inline BYTE USB_Uart::getByte( void ) 
{
  return( 0 );
}

//-------------------------------------------------------------------
inline void USB_Uart::setByte( BYTE data ) 
{
}

//-------------------------------------------------------------------
inline void USB_Uart::transmitByte( BYTE data ) 
{
  dataTxBuf << data;
  startTransmission( epIN );
}

}  } //namespace

//EOF
