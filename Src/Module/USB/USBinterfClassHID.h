//*******************************************************************
/*!
\file   USBinterfClassHID.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   31.01.2022

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _MOD_USB_INTERF_CLASS_HID_H
#define _MOD_USB_INTERF_CLASS_HID_H

//*******************************************************************
namespace EmbSysLib {
namespace Mod {

//*******************************************************************
/*!
\class USBinterfClassHID

\brief Implements an USB Human Interface Device (HID)

\example ModUSBkeyboard.cpp, ModUSBmouse.cpp, ModUSVvolume.cpp
*/
class USBinterfClassHID : public Hw::USBdeviceInterface
{
  public:
    //---------------------------------------------------------------
    /*! Constants defined in USB HID protocol
    */
    enum
    {
      REQUEST_GET_REPORT   = 0x01,
      REQUEST_GET_IDLE     = 0x02,
      REQUEST_GET_PROTOCOL = 0x03,
      REQUEST_SET_REPORT   = 0x09,
      REQUEST_SET_IDLE     = 0x0A,
      REQUEST_SET_PROTOCOL = 0x0B
    };
    
    enum
    {
      DESCRIPTOR_TYPE_HID    = 0x21,
      DESCRIPTOR_TYPE_REPORT = 0x22
    };

  public:
    //---------------------------------------------------------------
    /*! Initialize an USB HID interface
        \param usb USB device (hardware)
        \param configId Configuration Id
        \param interfId Interface Id
        \param dataIN Data buffer, send to USB host (IN transfer)
        \param dataOUT Data buffer, received from USB (OUT transfer)
    */
    USBinterfClassHID( Hw::USBdevice &usb,
                       BYTE           configId,
                       BYTE           interfId, 
                       DataPointer    dataIN, 
                       DataPointer    dataOUT );
     
    //---------------------------------------------------------------
    virtual bool onReportTransmit( void ) = 0;
   
    //---------------------------------------------------------------
    virtual bool onReportReceive( void ) = 0;
  
    //---------------------------------------------------------------
    virtual Std::DataPointer onGetDescriptor( WORD value );
  
    //---------------------------------------------------------------
    virtual void onConfigEndpoint( BYTE         epId, 
                                   DataPointer &data, 
                                   WORD         maxPacketSize );

    //---------------------------------------------------------------
    virtual bool onTransmit( BYTE epId, DataPointer &data );
    
    //---------------------------------------------------------------
    virtual bool onReceiveCtrl( WORD len );

    //---------------------------------------------------------------
    virtual bool onRequestCtrl_IN( DataPointer &dataPtr, 
                                   BYTE         request, 
                                   WORD         value, 
                                   WORD         length);

    //---------------------------------------------------------------
    virtual bool onRequestCtrl_OUT( DataPointer &dataPtr, 
                                    BYTE         request, 
                                    WORD         value, 
                                    WORD         length );

  public:
    //---------------------------------------------------------------
    DataPointer dataIN;
    DataPointer dataOUT;
    Flag<bool>  isStarted;
    
}; //class USBinterfClassHID

} } //namespace

#endif
