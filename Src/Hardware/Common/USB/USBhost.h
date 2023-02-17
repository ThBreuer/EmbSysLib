//*******************************************************************
/*!
\file   USBhost.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   14.12.2021

License: See file "LICENSE"

*/

//*******************************************************************
#ifndef _HW_USB_HOST_H
#define _HW_USB_HOST_H

//*******************************************************************
#include "Mcu_Types.h"
#include "Std/Report.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class USBhost

\brief Abstract class supporting USB (Host)

Interface to USB as host. Supports interrupt and control transfers.

\example HwUSBhost.cpp
*/
class USBhost
{
  protected:
    //---------------------------------------------------------------
    // Standard constructor
    // moduleId: Id of this module, used for report
    //
    USBhost( WORD moduleId )
	  : report( moduleId )
    {
    }

  public:
    //---------------------------------------------------------------
    /*! Opens a connection to a device
        \param vid vendor id
        \param pid product id
        \param iid interface id
    */
    virtual void open( WORD vid, WORD pid, WORD  iid ) = 0;

    //---------------------------------------------------------------
    /*! Closes a connection
    */
    virtual void close( void ) = 0;

    //---------------------------------------------------------------
    /*! Checks, if a device is connected
        \return true, if device is connected
    */
    virtual bool isConnected( void ) = 0;

    //---------------------------------------------------------------
    /*! Reads an object from the control endpoint 
        \param data Reference to the data object
        \return true, if data has been read successful
    */
    template <class T> bool readCtrl( T &data )
    {
      return( readCtrl_private( (BYTE *)&data, sizeof(T) ) );
    }

    //---------------------------------------------------------------
    /*! Reads a data array from the control endpoint
        \param data Pointer to the data array
        \param size Array size
        \return true, if data has been read successful
    */
    bool readCtrl( BYTE *data, WORD size )
    {
      return( readCtrl_private( data, size ) );
    }

    //---------------------------------------------------------------
    /*! Writes an object to the control endpoint
        \param data Reference to the data object
        \return true, if data has been written successful
    */
    template <class T> bool writeCtrl( T &data )
    {
      return( writeCtrl_private( (BYTE *)&data, sizeof(T) ) );
    }

    //---------------------------------------------------------------
    /*! Writes a data array to the control endpoint
        \param data Pointer to the data array
        \param size Array size
        \return true, if data has been written successful
    */
    bool writeCtrl(BYTE *data, WORD size )
    {
      return( writeCtrl_private( data, size ) );
    }

    //---------------------------------------------------------------
    /*! Reads an object from the interrupt endpoint 
        \param data Reference to the data object
        \return true, if data has been read successful
    */
    template <class T> bool readInterrupt( T &data )
    {
      return( readInterrupt_private( (BYTE *)&data, sizeof(T) ) );
    }

    //---------------------------------------------------------------
    /*! Reads a data array from the interrupt endpoint
        \param data Pointer to the data array
        \param size Array size
        \return true, if data has been read successful
    */
    bool readInterrupt( BYTE *data, WORD size )
    {
      return( readInterrupt_private( data, size) );
    }

    //---------------------------------------------------------------
    /*! Writes an object to the interrupt endpoint
        \param data Reference to the data object
        \return true, if data has been written successful
    */
    template <class T> bool writeInterrupt( T &data )
    {
      return( writeInterrupt_private( (BYTE *)&data, sizeof(T) ) );
    }

    //---------------------------------------------------------------
    /*! Writes a data array to the interrupt endpoint
        \param data Pointer to the data array
        \param size Array size
        \return true, if data has been written successful
    */
    bool writeInterrupt( BYTE *data, WORD size )
    {
      return( writeInterrupt_private( data, size ) );
    }

  private:
    //---------------------------------------------------------------
    virtual bool readCtrl_private( BYTE *data, WORD size ) = 0;

    //---------------------------------------------------------------
    virtual bool writeCtrl_private( BYTE *data, WORD size ) = 0;

    //---------------------------------------------------------------
    virtual bool readInterrupt_private( BYTE *data, WORD size ) = 0;

    //---------------------------------------------------------------
    virtual bool writeInterrupt_private( BYTE *data, WORD size ) = 0;

  public:
    //---------------------------------------------------------------
    Std::Report report;

}; //class USBhost

}  } //namespace

#endif
