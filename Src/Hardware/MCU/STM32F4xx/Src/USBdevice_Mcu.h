//*******************************************************************
/*!
\file   USBdevice_Mcu.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   16.10.2022

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_USB_DEVICE_MCU_H
#define _HW_USB_DEVICE_MCU_H

//*******************************************************************
#include "Hardware/Common/USB/USBdevice.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class USBdevice_Mcu

\brief Interface to USB hardware

\todo USB Bulk IN transfer not working!
\warning USB Bulk IN transfer not working!

\example HwUSBdevice.cpp
*/
class USBdevice_Mcu : public USBdevice
{
  private:  
    //*****************************************************************
    class Buffer
    {
      public:
        //-----------------------------------------------------------
        BYTE data[128];
        BYTE size;
        BYTE flag;
        BYTE maxpacket;
      
        //-----------------------------------------------------------
        Buffer( void )
        {
          size      = 0;
          flag      = false;
          maxpacket = 0;
        }
    };
    
  private:
    //***************************************************************
    class EndpointIN
    {
      public:
        //-----------------------------------------------------------
        EndpointIN( void );
        
        //-----------------------------------------------------------
        void  init    ( BYTE numIn );
        void  config  ( WORD epMPS, BYTE epType = 0 ); // default: control EP
        void  flush   ( void );
        void  reset   ( void );
        void  start   ( void );
        void  setStall( void );
        void  clrStall( void );
        void  setNAK  ( void );
        void  clrNAK  ( void );
        
        //-----------------------------------------------------------
        WORD  write      ( BYTE *dataPtr, WORD len );
        void  writeToFifo( void );
        void  transmit   ( bool flag );
        void  transmitZLP( void );
        
        //-----------------------------------------------------------
        DWORD getInterrupt( void );
        void  clrInterrupt( DWORD interruptID );

      private:
      public:
        //-----------------------------------------------------------
        BYTE                        num;
        Buffer                      txBuf;
        USB_OTG_INEndpointTypeDef  *inep;
        __IO DWORD                 *fifo;
        
    }; // EndpointIN
    //***************************************************************
  
  private:
    //***************************************************************
    class EndpointOUT
    {
      public:
        EndpointOUT( void );
        
        //-----------------------------------------------------------
        void init    ( BYTE numIn );
        void config  ( WORD epMPS, BYTE epType = 0 ); // default: control EP
        void flush   ( void );
        void reset   ( void );
        void start   ( void );
        void setStall( void );
        void clrStall( void );
        void setNAK  ( void );
        void clrNAK  ( void );
      
        //-----------------------------------------------------------
        WORD read        ( BYTE *dataPtr, WORD len );
        void readFromFifo( WORD  len);
        void receive     ( void );
        void receiveZLP  ( void );

        //-----------------------------------------------------------
        DWORD getInterrupt( void );
        void  clrInterrupt( DWORD interruptID );
        
      private:
        //-----------------------------------------------------------
        BYTE                        num;
        Buffer                      rxBuf;
        USB_OTG_OUTEndpointTypeDef *outep;
        __IO DWORD                 *fifo;
        
    }; // EndpointOUT
    //***************************************************************

  public:
    //---------------------------------------------------------------
    /*! Instantiate an USB object
        \param desc Reference to a descriptor
    */
    USBdevice_Mcu( USBdeviceDescriptor &desc );

    //---------------------------------------------------------------
    /*! Start USB device
    */
    virtual void start( void );

    //---------------------------------------------------------------
    //
    void isr( void );

  private:
    //---------------------------------------------------------------
    //
    virtual void reset( void );

    //---------------------------------------------------------------
    virtual void configure( BYTE flag );

    //---------------------------------------------------------------
    virtual void setAddress( BYTE adr = 0 );

    //---------------------------------------------------------------
    virtual void configEP( BYTE epAddr,
                           WORD epMaxPacketSize,
                           BYTE epType );

    //---------------------------------------------------------------
    virtual void setStallEP( BYTE epAddr );

    //---------------------------------------------------------------
    virtual void clrStallEP( BYTE epAddr );

    //---------------------------------------------------------------
    virtual void setNAK_EP( BYTE epAddr );

    //---------------------------------------------------------------
    virtual void clrNAK_EP( BYTE epAddr );

    //---------------------------------------------------------------
    virtual WORD readEP( BYTE  epAddr,
                         BYTE *dataPtr,
                         WORD  len );

    //---------------------------------------------------------------
    virtual WORD writeEP( BYTE  epAddr,
                          BYTE *dataPtr,
                          WORD  len );

  private:
    //---------------------------------------------------------------
    BYTE  CoreReset         ( void );
    DWORD getInInterrupt    ( void );
    DWORD getOutInterrupt   ( void );
    BYTE  isInterruptPending( DWORD interuptID );
    void  clrInterrupt      ( DWORD interuptID );

  public:
    //---------------------------------------------------------------
    static USBdevice_Mcu *usbPtr;

  private:
    //---------------------------------------------------------------
    static const WORD NUM_OF_ENDPOINTS = 4;
    static const WORD MAX_EP0_SIZE     = 64;

    USBdevice_Mcu::EndpointIN  epIN [ NUM_OF_ENDPOINTS ];
    USBdevice_Mcu::EndpointOUT epOUT[ NUM_OF_ENDPOINTS ];

}; //class USBdevice_Mcu

}  } //namespace

#endif
