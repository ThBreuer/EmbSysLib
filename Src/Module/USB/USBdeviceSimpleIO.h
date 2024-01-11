//*******************************************************************
/*!
\file   USBdeviceSimpleIO.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   01.12.2022

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _MOD_USB_DEVICE_SIMPLE_IO_H
#define _MOD_USB_DEVICE_SIMPLE_IO_H

//*******************************************************************
namespace EmbSysLib {
namespace Mod {

//*******************************************************************
/*!
\class USBdeviceSimpleIO

\brief Supports device-side a simple data transfer via USB

This class supports devices sending (IN-transfer) and receiving 
(OUT-transfer) data structures via interrupt endpoints. 

Sending: The data structure is buffered in a fifo. With each IN-transfer,
triggered by host, a data set is taken from the fifo and transfered.

Receiving: Incoming data sets from host are also buffered in a fifo and 
can queried.

The member \b Flag<Status> status can be used to request the USB connection state.

\example ModUSBdeviceSimpleIO.cpp
*/
//-----------------------------------------------------------------
template <class TOUT = BYTE, class TIN = BYTE>
class USBdeviceSimpleIO : private Hw::USBdeviceInterface
{
  public:
    //---------------------------------------------------------------
    /*! \enum Status
        \brief Indicates USB connection status
    */
    typedef enum
    {
      STARTED = 1, //!< USB connection established
      STOPPED = 2  //!< USB disconnected
    } Status;

    //---------------------------------------------------------------
    /*! Instantiate a simple USB-IO 
        \param usb USB device (hardware)
        \param configId Configuration Id
        \param interfId Interface Id
    */
    USBdeviceSimpleIO( Hw::USBdevice &usb, 
                       BYTE           configId,
                       BYTE           interfId )

    : USBdeviceInterface( usb, configId, interfId )
    {
    }

    //---------------------------------------------------------------
    /*! Add a data copied into the IN-interrupt queue. The data will be 
        send with one of the next IN interrupts 
        \param data Reference to the data
        \return true: data is successfully entered into the queried \n
                false: data not copied, as fifo is full
    */
    bool transmit( TIN &data )
    {
      return( inFifo << data );
    }

    //---------------------------------------------------------------
    /*! Get data from OUT fifo
        \param data Reference to the data, filled by this method
        \return true: data is successfully picked from fifo\n
                false: no data, fifo is empty
    */
    bool receive( TOUT &data )
    {
      return( outFifo >> data );
    }

  private:
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
    virtual bool onReceive( BYTE epId, WORD cnt, DataPointer &data )
    {
      
      if( sizeof(TOUT) == cnt && !outFifo.isFull() )
      {
        outFifo << outDataBuffer;
      }
      data = outDataBuffer;
      return( true );
    }

    //---------------------------------------------------------------
    virtual bool onTransmit( BYTE epId, DataPointer &data )
    {
      if( !inFifo.isEmpty() )
      {
        inFifo >> inDataBuffer;
        data = inDataBuffer;
        return( true );
      }
      data = DataPointer(0,0);
      return( true );
    }

    //---------------------------------------------------------------
    virtual void onStart()
    {
      status = STARTED;
    }

    //---------------------------------------------------------------
    virtual void onStop()
    {
      status = STOPPED;
    }

  public:
    //---------------------------------------------------------------
    /*! USB connection state
    */
    Flag<Status> status;

  private:
    //---------------------------------------------------------------
    TOUT outDataBuffer;
    TIN  inDataBuffer;
  
    Fifo<TOUT> outFifo;
    Fifo<TIN>  inFifo;
    
}; //class USBdeviceSimpleIO

} } //namespace

#endif
