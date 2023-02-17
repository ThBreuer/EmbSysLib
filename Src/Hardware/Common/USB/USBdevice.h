//*******************************************************************
/*!
\file   USBdevice.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_USB_DEVICE_H
#define _HW_USB_DEVICE_H

//*******************************************************************
#include "Mcu_Types.h"
#include "Std/Report.h"
#include "USBdevice/USBdescriptor.h"
#include "USBdevice/USBendpoint.h"
#include "USBdevice/USBcontrol.h"
#include "USBdevice/USBinterface.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class USBdevice

\brief Abstract interface to USB hardware

\example HwUSBdevice.cpp
*/
class USBdevice
{
  protected:
    //---------------------------------------------------------------
    // Base class constructor
    // moduleId: Id of this module, used for report
    // desc:     Reference to the descriptor object
    //
    USBdevice( WORD moduleId, USBdeviceDescriptor &desc );

    //---------------------------------------------------------------
    // Handle an interrupt (IN, OUT or SETUP) on endpoint EPnum
    // EPnum:   endpoint identifier
    // isSetup: true, if setup message received 
    //
    virtual void eventHandler( BYTE EPnum,
                               BYTE isSetup );

    //---------------------------------------------------------------
    // Handle a suspend interrupt
    // This method calls the onStop() method of all USBdeviceInterface
    // objects
    //    
    virtual void suspend( void );

  public:
    //---------------------------------------------------------------
    /*! Start USB
        Restart hardware and enable interrupts
    */
    virtual void start( void ) = 0;

  protected:
    //---------------------------------------------------------------
    virtual void reset( void ) = 0;

    //---------------------------------------------------------------
    virtual void configure( BYTE flag ) = 0;

    //-------------------------------------------------------------------
    virtual void setAddress( BYTE addr ) = 0;

    //---------------------------------------------------------------
    virtual void configEP( BYTE epAddr,
                           WORD epMaxPacketSize,
                           BYTE epType          ) = 0;

    //---------------------------------------------------------------
    virtual void setStallEP( BYTE epAddr ) = 0;

    //---------------------------------------------------------------
    virtual void clrStallEP( BYTE epAddr ) = 0;

    //---------------------------------------------------------------
    virtual void setNAK_EP( BYTE epAddr ) = 0;

    //---------------------------------------------------------------
    virtual void clrNAK_EP( BYTE epAddr ) = 0;

    //---------------------------------------------------------------
    virtual WORD readEP( BYTE  epAddr,
                         BYTE *dataPtr,
                         WORD  len     ) = 0;

    //---------------------------------------------------------------
    virtual WORD writeEP( BYTE  epAddr,
                          BYTE *dataPtr,
                          WORD  len     ) = 0;

  public:
    //---------------------------------------------------------------
    USBdeviceDescriptor  &desc;   // must be the first one

  protected:
    //---------------------------------------------------------------
    USBdeviceControl      epCtrl;

  //-----------------------------------------------------------------
  friend class USBdeviceControl;
  friend class USBdeviceEndpoint;
  friend class USBdeviceInterface;

  public:
    //---------------------------------------------------------------
    Std::Report report;

}; //class USBdevice

}  } //namespace

#endif
