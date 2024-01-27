//*******************************************************************
/*!
\file   Isc_USBdevice.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   03.01.2024

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _MOD_ISC_USB_DEVICE_H
#define _MOD_ISC_USB_DEVICE_H

//*******************************************************************
#include "Module/Isc/Isc.h"

//*******************************************************************
namespace EmbSysLib {
namespace Mod {

//*******************************************************************
/*!
\class Isc_USBdevice

\brief Isc transport class with USB hardware

\remark The length of the payload is limited to 60 bytes
  
  \todo use USBdeviceInterface::onStop() / onStart to check USB state

\example Isc.cpp
*/
template <WORD maxLength, BYTE fifoSize> 
class Isc_USBdevice : public Isc,
                      public USBdeviceInterface
{
  protected:
    //---------------------------------------------------------------
    #pragma pack(1)
    class DATA
    {
      public:
        WORD size;
        class Payload
        {
          public:
//            static const WORD maxLength = 80; // todo const max payload size / configurable?

            Header header;
            BYTE data[ maxLength ];
        } payload;
    };
    #pragma pack()

  public:
    //---------------------------------------------------------------
    /*! Instantiate an Isc transport object with USB interface

        \param uart Reference to USB hardware
        \param interfId USB interface ID
        \param crcMode Select CRC calculation mode (FAST or SMALL)
    */
    Isc_USBdevice( USBdevice    &usb,
                   BYTE          interfId,
                   Crc::MODE     crcMode = Crc::SMALL )

    : Isc( crcMode ),
      USBdeviceInterface( usb, 1, interfId ),
      usb(usb),
      inFifo(fifoSize),   // todo why "5" ? what is the lowest number? configurable?
      outFifo(fifoSize)

    {
    }

    //---------------------------------------------------------------
    /*! ...
    */
    virtual bool isConnected()
    {
      return( true );
    }

    //---------------------------------------------------------------
    /*! Update the Isc transport object
    */
     virtual void update( void )
       {
  DATA tmp;

  while( outFifo >> tmp ) //read data from fifo and process
  {
    process( tmp.payload.header, tmp.payload.data, tmp.size-sizeof(Header) );
  }
}


   private:
    //---------------------------------------------------------------
    virtual bool writeStream( Header &header, BYTE *data, WORD size )
      {
  DATA tmp;

  tmp.payload.header = header;
  tmp.size = size+sizeof(Header);
  memcpy( tmp.payload.data, data, size );

  return( inFifo << tmp );
}

   //---------------------------------------------------------------
    virtual void onConfigEndpoint( BYTE epId, DataPointer &data, WORD maxPacketSize )
    {
      if( epId & 0x80 )
      {
        data = inDataBuffer;
      }
      else
      {
        data = outDataBuffer;
      }
    }

    //---------------------------------------------------------------
    virtual bool onTransmit( BYTE epId, DataPointer &epData )
      {
  if( !inFifo.isEmpty() )
  {
    inFifo >> inDataBuffer;
    //data = inDataBuffer.payload;
    epData.set( (BYTE*)&inDataBuffer.payload, inDataBuffer.size );
    return( true );
  }
  epData = DataPointer(0,0);

  return( true );
}

    //---------------------------------------------------------------
    virtual bool onReceive( BYTE epId, WORD cnt, DataPointer &data )
      {
  outDataBuffer.size = cnt;
  outFifo << outDataBuffer;

  data = outDataBuffer.payload;

  return( true );
}

  private:
    //---------------------------------------------------------------
    USBdevice &usb;

    //---------------------------------------------------------------
    DATA   inDataBuffer;
    DATA  outDataBuffer;

    Fifo<DATA>  inFifo;
    Fifo<DATA> outFifo;

}; //Isc_USBdevice

} } //namespace

#endif
