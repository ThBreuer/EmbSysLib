//*******************************************************************
/*!
\file   USB_Uart.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   21.02.2021

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _MOD_USB_Uart_H
#define _MOD_USB_Uart_H

//*******************************************************************
#include "Mcu_Types.h"
#include "Hardware/Common/USB/USBdevice.h"
#include "Hardware/Common/Uart/Uart.h"
#include "Std/Flag.h"
#include "Std/DataPointer.h"
#include "Module/ReportID_Mod.h"

//*******************************************************************
namespace EmbSysLib {
namespace Mod {

using namespace Std;

//*******************************************************************
/*!
\class USB_Uart

\brief Implements an Uart-Interface using USB CDC

Device:        Class = COMMUNICATIONS_CLASS, SubClass = 0, Protocol = 0
Interface n:   Class = CDC_COMMUNICATION, SubClass = 2, Protocol = 0, 1 interrupt endpoint
Interface n+1: Class = CDC_DATA, SubClass = 0, Protocol = 0, 2 bulk endpoints (IN/OUT) with maxPacketSize = USB_Uart::bufferSize

\example ModUSB_Uart.cpp
*/
class USB_Uart : public Hw::USBdeviceInterface, public Hw::Uart
{
  public:
    //-----------------------------------------------------------------
    /*! \enum connectionState_enum
        \brief Indicates the state of the USB communication
    */
    typedef enum
    {
      DEFAULT            = 0,
      DISCONNECTED       = 1, //!< \n No USB host connected
      CONNECTED          = 2, //!< \n Interface is connected to a terminal
      USB_READY          = 3, //!< \n USB is ready to use
      USB_DISCONNECTED   = 4  //!< \n USB suspend
    } connectionState_enum;

  private:
    //-----------------------------------------------------------------
    class CtrlInterface : public USBdeviceInterface
    {
      public:
        //---------------------------------------------------------------
        CtrlInterface( Hw::USBdevice   &usb, 
                       BYTE             iid, 
                       Std::Flag<connectionState_enum> &connection );

      private:
        //---------------------------------------------------------------
        virtual void onStart( void );

        //---------------------------------------------------------------
        virtual void onStop( void );
   
        //---------------------------------------------------------------
        virtual bool onRequestCtrl_IN( DataPointer &dataPtr, 
                                       BYTE         request,
                                       WORD         value, 
                                       WORD         length );

        //---------------------------------------------------------------
        virtual bool onTransmit( BYTE         epId,
                                 DataPointer &epdata );

        //---------------------------------------------------------------
        virtual bool onRequestCtrl_OUT( DataPointer &dataPtr, 
                                        BYTE         request,
                                        WORD         value, 
                                        WORD         length );

      private:
        //---------------------------------------------------------------
        Std::Flag<connectionState_enum> &connection;

    }; //class CtrlInterface
 
  public:
    //---------------------------------------------------------------
    /*! Initialize USB CDC
        \param usb USB device (hardware)
        \param configId Configuration Id
        \param interfId Interface Id
        \param bufferSize Size of I/O data buffer
    */
    USB_Uart( Hw::USBdevice &usb, 
              BYTE           configId, 
              BYTE           interfId,
              WORD           bufferSize );

  private:
    //---------------------------------------------------------------
    virtual void onConfigEndpoint( BYTE         epId, 
                                   DataPointer &data, 
                                   WORD         maxPacketSize );

    //---------------------------------------------------------------
    virtual bool onTransmit( BYTE         epId, 
                             DataPointer &data );
    
    //---------------------------------------------------------------
    virtual bool onReceive( BYTE         epId, 
                            WORD         len, 
                            DataPointer &epdata );

    //---------------------------------------------------------------
    virtual BYTE getByte( void );
    
    //---------------------------------------------------------------
    virtual void setByte( BYTE data );

    //---------------------------------------------------------------
    virtual void transmitByte( BYTE data );

  public:
    //---------------------------------------------------------------
    static const WORD packetSize = 32; //!< \n

  public:
    //---------------------------------------------------------------
    Std::Flag<connectionState_enum> connection; //!< \n

  private:
    //---------------------------------------------------------------
    BYTE          bufIN [packetSize];
    BYTE          bufOUT[packetSize];
    BYTE          epIN;
    CtrlInterface ctrlInterface;

};

} } //namespace

#endif
