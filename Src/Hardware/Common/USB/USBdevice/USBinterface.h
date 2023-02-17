//*******************************************************************
/*!
\file   USBinterface.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   21.02.2022

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_USB_DEVICE_INTERFACE_H
#define _HW_USB_DEVICE_INTERFACE_H

//*******************************************************************
#include "Mcu_Types.h"
#include "Std/DataPointer.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

using namespace Std;

//*******************************************************************
class USBdevice;

//*******************************************************************
/*!
\class USBdeviceInterface

\brief USB interface handling

This class defines the interface between USB hardware and the 
application. The application should inherit from this class and 
override the methods as necessary. 
*/
class USBdeviceInterface
{
  public:
    //---------------------------------------------------------------
    /*! Instantiate an USB interface

        \param usb      Reference to USB
        \param configId USB configuration ID
        \param interfId USB interface ID
    */
    USBdeviceInterface( USBdevice &usb,
                        BYTE       configId,
                        BYTE       interfId );

  protected:
    //---------------------------------------------------------------
    /*! Call back function

        Called, when USB is started
    */
    virtual void onStart( void )
    {
    }

    //---------------------------------------------------------------
    /*! Call back function

        Called, when USB connection is stopped/suspended
    */
    virtual void onStop( void )
    {
    }

    //---------------------------------------------------------------
    /*! Call back function

        Called, when an endpoint is configurated. The application 
        should set the DataPointer
        \param epId Endpoint ID
        \param epData Data to written in endpoint buffer
        \param maxPacketSize
    */
    virtual void onConfigEndpoint( BYTE         epId, 
                                   DataPointer &epData,
                                   WORD         maxPacketSize )
    {
      epData = DataPointer( NULL, 0 );
    }

    //---------------------------------------------------------------
    /*! Call back function

        The function is called by an interrupt on IN transfer (data
        transmited from device to host).
        The application should set \b epdata sending to host with next transfer
        \param epId Endpoint ID
        \param epData Next data send to host
        \return
          - true: OK, data ready to transmit
          - false: Not OK, no data to transmit
    */
    virtual bool onTransmit( BYTE         epId,
                             DataPointer &epData )
    {
      return( true );
    }

    //---------------------------------------------------------------
    /*! Call back function

        The function is called from interrupt on OUT transfer (data
        received from host) and data are written to the data buffer. 
        The application can handle data now.
        \param epId   Endpoint ID
        \param len    Num of received data bytes
        \param epData 
        \return
          - true: OK
          - false: Not OK
    */
    virtual bool onReceive( BYTE         epId, 
                            WORD         len, 
                            DataPointer &epData )
    {
      return( true );
    }

    //---------------------------------------------------------------
    /*! Call back function
   
        The function is called from interrupt on SETUP transfer, the
        host requests a data transfer from device via control endpoint.
        The application must fill \b epData sending to host. This
        function is called before \b transmit on control endpoint.
        \param epData  Pointer sending to data
        \param request Request type from setup
        \param value   Value from setup
        \param length  Length of requested data
        \return
          - true:  OK
          - false: Not OK
    */
    virtual bool onRequestCtrl_IN( DataPointer &epData,
                                   BYTE         request,
                                   WORD         value,
                                   WORD         length )
    {
      return( false );
    }

    //---------------------------------------------------------------
    /*! Call back function

        The function is called from interrupt on SETUP transfer, the
        host requests a data transfer to device via control endpoint.
        The application has to set \b epData to a data buffer, where
        received data will be stored. This function is called before
        \b receive on control endpoint.
        \param epData  Pointer to data receive buffer
        \param request Request type from setup
        \param value   Value from setup
        \param length  Length of requested data
        \return
          - true: Pointer is valid, data can be stored
          - false: Data can not be received
    */
    virtual bool onRequestCtrl_OUT( DataPointer &epData,
                                    BYTE         request,
                                    WORD         value,
                                    WORD         length )
    {
      return( false );
    }

    //---------------------------------------------------------------
    /*! Call back function

        The function is called from interrupt on IN control transfer
        (data transmited from device to host).
    */
    virtual void onTransmitCtrl( void )
    {
    }
    
    //---------------------------------------------------------------
    /*! Call back function

        The function is called from interrupt on OUT transfer (data
        received from host) and data are written to the data buffer,
        determined in \b onRequestCtrl_OUT(). 
        The application can handle data now.
        \param len  Num of received data bytes
        \return
          - true: OK
          - false: Not OK
    */
    virtual bool onReceiveCtrl( WORD len )
    {
      return( true );
    }
    
    //---------------------------------------------------------------
    /*! Call back function
    
        Handle application specific interface descriptor
        \param value value from setup
        \return DataPointer to descriptor
    */
    virtual DataPointer onGetDescriptor( WORD value )
    {
      return( DataPointer() );
    }

  protected:
    //---------------------------------------------------------------
    /*! Sets endpoint data valid
        \param epId Endpoint ID
    */
    virtual void startTransmission( BYTE epId );

  protected:
    //---------------------------------------------------------------
    USBdevice  &usb;
    BYTE        configId;
    BYTE        interfId;

  private:
    //---------------------------------------------------------------
    bool isReady;

  //-----------------------------------------------------------------
  friend class USBdevice;
  friend class USBdeviceEndpoint;
  friend class USBdeviceControl;

}; //class USBdeviceInterface

} } //namespace

#endif
