//*******************************************************************
/*!
\file   USBendpoint.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"
*/

//*******************************************************************
#include "../USBdevice.h"
#include "Std/Std.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// USBdeviceEndpoint
//
//*******************************************************************
//-------------------------------------------------------------------
USBdeviceEndpoint::USBdeviceEndpoint( USBdevice          &usbIn,
                                      USBdeviceInterface *interfIn,
                                      BYTE                idIn,
                                      WORD                maxPacketSizeIn,
                                      BYTE                attributesIn )
: usb( usbIn )
{
  interf        = interfIn;
  id            = idIn;
  maxPacketSize = maxPacketSizeIn;
  attributes    = attributesIn;
  addZLP        = false;
  totalLen      = 0;
}

//-------------------------------------------------------------------
USBdeviceEndpoint::USBdeviceEndpoint( USBdevice &usbIn,
                                      BYTE       idIn,
                                      WORD       maxPacketSizeIn )
: usb( usbIn )
{
  interf        = NULL;
  id            = idIn;
  maxPacketSize = maxPacketSizeIn;
  attributes    = USBdeviceDescriptor::Endpoint::CONTROL;
}

//-------------------------------------------------------------------
void USBdeviceEndpoint::in( void )
{
  if( epData.isEmpty() )
  {
    if( addZLP )
    {
      writeData();
      addZLP = false;
    }
    else if( interf )
    {
      if( interf->onTransmit( id, epData ) )
      {
        if( epData.getSize() && epData.getSize()%maxPacketSize == 0 )
        {
          addZLP = true;
        }
        writeData();
      }
      else
      {
        writeData();
        setNAK();
        interf->isReady = false;
      }
    }
  }
  else
  {
    writeData();
  }
}

//-------------------------------------------------------------------
void USBdeviceEndpoint::inCtrl( void )
{
  if( epData.isEmpty() )
  {
    if( interf )
    {
      interf->onTransmitCtrl();
    }
  }
  else
  {
    writeData( );
  }
}

//-------------------------------------------------------------------
void USBdeviceEndpoint::out( void )
{
  WORD cnt = readData( );

  totalLen += cnt;

  if( cnt < maxPacketSize || epData.getSize() == 0 )
  {
    if( interf )
    {
      if( interf->onReceive( id, totalLen, epData ) == false )
      {
        stall();
      }
    }
    totalLen = 0;
  }
}

//-------------------------------------------------------------------
void USBdeviceEndpoint::outCtrl( void )
{
  WORD cnt = readData( );

  totalLen += cnt;

  if( cnt < maxPacketSize ||  epData.getSize() == 0 )
  {
    if( interf )
    {
      if( interf->onReceiveCtrl( totalLen ) == false )
      {
        stall();
      }
    }
    totalLen = 0;
  }
}

//-------------------------------------------------------------------
WORD USBdeviceEndpoint::readData( void )
{
  WORD cnt = usb.readEP( id,
                         epData.getPtr(),
                         epData.getSize() );

  epData.shift( cnt );

  return( cnt );
}

//-------------------------------------------------------------------
void USBdeviceEndpoint::writeData( void )
{
  WORD cnt = usb.writeEP( id,
                          epData.getPtr(),
                          MIN( epData.getSize(),
                          maxPacketSize ) );

  epData.shift( cnt );
}

//-------------------------------------------------------------------
void USBdeviceEndpoint::writeData( const DataPointer &epDataIn )
{
  epData = epDataIn;
  writeData();
}

//-------------------------------------------------------------------
void USBdeviceEndpoint::writeDataLen( const DataPointer &epDataIn,
                                      WORD               maxLen )
{
  DataPointer tmp( epDataIn );

  epData.set( tmp.getPtr(), MIN( tmp.getSize(), maxLen ));
  writeData();
}

//-------------------------------------------------------------------
void USBdeviceEndpoint::writeAcknowledge( void )
{
  usb.writeEP( id, NULL, 0 );
}

//-------------------------------------------------------------------
void USBdeviceEndpoint::readAcknowledge( void )
{
  BYTE buffer;

  usb.readEP( id, &buffer, 1 );
}

//-------------------------------------------------------------------
void USBdeviceEndpoint::setConfiguration( void )
{
  usb.configEP( id, maxPacketSize, attributes );

  if( interf )
  {
    interf->onConfigEndpoint( id, epData, maxPacketSize );
    writeData( );
  }
}

//-------------------------------------------------------------------
void USBdeviceEndpoint::stall( void )
{
  usb.setStallEP( id );
}

//-------------------------------------------------------------------
void USBdeviceEndpoint::clrStall( void )
{
  usb.clrStallEP( id );
}

//-------------------------------------------------------------------
void USBdeviceEndpoint::setNAK( void )
{
  usb.setNAK_EP( id );
}

//-------------------------------------------------------------------
void USBdeviceEndpoint::clrNAK( void )
{
  usb.clrNAK_EP( id );
}

}  } //namespace

//EOF
