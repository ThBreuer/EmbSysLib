//*******************************************************************
/*!
\file   USBdevice_Mcu.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

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

\example HwUSBdevice.cpp
*/
class USBdevice_Mcu : public USBdevice
{
  public:
    //---------------------------------------------------------------
    /*! Initialize the USB device hardware
        \param desc Complete set of descriptors
    */
    USBdevice_Mcu( USBdeviceDescriptor &desc );

    //---------------------------------------------------------------
    // Interrupt Service Routine (ISR) of USB interrupt.
    // This function must be public, because it is called
    // in extern "C"
    void isr( void );

    //---------------------------------------------------------------
    virtual void start( void );

  private:
    //***************************************************************
    class EPconfig
    {
      public:
        //-----------------------------------------------------------
        typedef enum
        {
          DISABLED = 0,
          STALL    = 1,
          NAK      = 2,
          VALID    = 3
        } State;
        
      public:
        //-----------------------------------------------------------
        EPconfig()
        {
          regPtr = NULL;
          config = 0;
          rxPtr  = NULL;
          txPtr  = NULL;
        }

        //-----------------------------------------------------------
        void init( DWORD addrIn, DWORD type, DWORD kind, WORD sizeIn );

        //-----------------------------------------------------------
        WORD *bdt_init(BYTE num, BYTE isRX, WORD size );

        //-----------------------------------------------------------
        void clrCTR_RX( void );

        //-----------------------------------------------------------
        void clrCTR_TX( void );

        //-----------------------------------------------------------
        void setSTAT_RX( DWORD stat );

        //-----------------------------------------------------------
        void setSTAT_TX(DWORD stat );

        //-----------------------------------------------------------
        volatile DWORD  config;
        volatile DWORD *regPtr;
        volatile DWORD  addr;
        volatile WORD   rxSize;
        volatile WORD   txSize;
        volatile WORD  *txPtr;
        volatile WORD  *rxPtr;

        //------------------------------------------------------------
        volatile static WORD offset;

    };  //class EPconfig
    //***************************************************************

  private:
    //---------------------------------------------------------------
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
                          BYTE *pData,
                          WORD  cnt );

  private:
    //---------------------------------------------------------------
    BYTE epAddr2Index( BYTE epAddr )
    {
      return( ((epAddr & 0x0F) << 1) | ((epAddr & 0x80)?0x01:0x00) );
    }

  public:
    //---------------------------------------------------------------
    static USBdevice_Mcu *usbPtr;

  private:
    //---------------------------------------------------------------
    static const WORD USB_EP_NUM = 32;

    EPconfig EP[8];

}; //class USBdevice_Mcu

}  } //namespace

#endif
