//*******************************************************************
/*!
\file   Uart_Serial.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   14.02.2011

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_UART_SERIAL_H
#define _HW_UART_SERIAL_H

//*******************************************************************
#include "Hardware/Common/Uart/Uart.h"
#include <windows.h>

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class Uart_Serial

\brief Implementation of a serial communication interface ("COM-Port")

\example HwUart.cpp
*/
class Uart_Serial : public Uart
{
  public:
    //---------------------------------------------------------------
    /*! \enum Baudrate
        \brief Baud rate
    */
    typedef enum
    {
      BR_2400   = 2400,  //!<   2400 baud
      BR_4800   = 4800,  //!<   4800 baud
      BR_9600   = 9600,  //!<   9600 baud
      BR_19200  = 19200, //!<  19200 baud
      BR_38400  = 38400, //!<  38400 baud
      BR_57600  = 57600, //!<  57600 baud
      BR_115200 = 115200 //!< 115200 baud
    } Baudrate;

  public:
    //---------------------------------------------------------------
    /*! Initialize 
        \param baud Baudrate
        \param portName Name of the COM-Port (e.g. "COM1")
    */
    Uart_Serial( Baudrate baud, const char *portName = 0 );

    //---------------------------------------------------------------
    /*! Open a COM-Port
        \param portName Name of the COM-Port (e.g. "COM1")
    */
    virtual void open( const char *portName = 0 );

    //---------------------------------------------------------------
    /*! Close the connection
    */
    void close( void );

    //---------------------------------------------------------------
    /*! Checks, if connection is open
        \return true, if connection is open
    */
    bool isOpen( void );

    //---------------------------------------------------------------
    /*! Set a new port name
        \param portName  Name of the COM-Port (e.g. "COM1")
    */
    void setPortName( const char *portName );

    //-------------------------------------------------------------------
    // We have to overwrite these functions, because there is no Interrupt!
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    virtual bool get( BYTE &data );

    //-------------------------------------------------------------------
    virtual BYTE get( void );

    //-------------------------------------------------------------------
    virtual void transmitByte( BYTE c );

    //-------------------------------------------------------------------
    virtual void set( BYTE c );

    //-------------------------------------------------------------------
    virtual void set( const char *c );

    //-------------------------------------------------------------------
    virtual void set( const BYTE *c, BYTE size );

  private:
    //-------------------------------------------------------------------
    virtual BYTE getByte( void );

    //-------------------------------------------------------------------
    virtual void setByte( BYTE c );

  private:
    //-------------------------------------------------------------------
    DWORD  baud;
    HANDLE hSerial;
    bool   isOpenFlag;
    char   portName[33];

}; //class Uart_Serial

}  } //namespace

#endif
