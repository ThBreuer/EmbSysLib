//*******************************************************************
/*!
\file   Uart.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_UART_H
#define _HW_UART_H

//*******************************************************************
#include "Mcu_Types.h"
#include "Std/Fifo.h"
#include "Std/Report.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class Uart

\brief Abstract class supporting the UART hardware

UART is a serial communication interface. This base class uses a
receive and transmit buffer (FIFO).

Derived classes should at least:
 - initialize the UART hardware
 - implement \a setByte() to write a data byte to the hardware
 - implement \a getByte() to read a data byte from the hardware
 - call \a isrRx() and \a isrTx() in the appropriate ISR

\example HwUart.cpp
*/
class Uart
{
  protected:
    //---------------------------------------------------------------
    // Initializes the receive and transmit buffer
    // moduleId:  Id of this module, used for report
    // sizeRxBuf: Size of receive buffer
    // sizeTxBuf: Size of transmit buffer
    //
    Uart( WORD moduleId, WORD sizeRxBuf, WORD sizeTxBuf );

  public:
    //---------------------------------------------------------------
    /*! Writes a data byte to the transmit buffer and starts the data
        transfer by hardware
        \remark This method blocks until FIFO is not full.
                Use isTxBufferFull() to check FIFO
        \param data Data byte to transmit
    */
    virtual void set( BYTE data );

    //---------------------------------------------------------------
    /*! Writes a byte array to the transmit buffer and starts the data
        transfer by hardware
        \remark This method blocks until FIFO is not full.
                Use isTxBufferFull() to check FIFO
        \param  data   Pointer to byte array
        \param  length Length of byte array
    */
    virtual void set( const BYTE *data, WORD length );

    //---------------------------------------------------------------
    /*! Writes a string to the transmit buffer and starts the data
        transfer by hardware
        \remark This method blocks until FIFO is not full.
                Use isTxBufferFull() to check FIFO
        \param str Null terminated string to transmit
    */
    virtual void set( const char *str );

    //---------------------------------------------------------------
    /*! Checks, if transmit buffer is full
        \return \a true:  Transmit buffer is full, next call to set()
                          will block \n
                \a false: Transmit  buffer is not full, next call to
                          set() will return immediatly
    */
    virtual bool isTxBufferFull( void );

    //---------------------------------------------------------------
    /*! todo ...
    */
    virtual WORD getFifoRemainingSize( void );

    //---------------------------------------------------------------
    /*! Checks, if a data byte has been received. In this case, the
        data byte is read from hardware and written to \a *data
        \param data Reference, returns the data byte
        \return \a true:  A new data byte has been received \n
                \a false: No received data
    */
    virtual bool get( BYTE &data );

    //---------------------------------------------------------------
    /*! Checks, if a data byte has been received. In this case, the
        data byte is read from hardware and returned
        \return \a 0:  No received data or 0 is received \n
                \a other: Received data
    */
    virtual BYTE get( void );

  protected:
  public:
    //---------------------------------------------------------------
    // Interrupt Service Routine of receive interrupt
    // Reads the received data by calling getByte() and writes the
    // data to the receive buffer
    //
    virtual void isrRx( void );

    //---------------------------------------------------------------
    // Interrupt Service Routine of transmit interrupt
    // Checks, if further data has to been sent. If the transmit FIFO
    // is not empty, the next data byte is written to hardware by
    // calling setByte().
    //
    virtual void isrTx( void );

  private:
    //---------------------------------------------------------------
    // Writes a data byte to the hardware interface (register)
    // data: Transmitted data byte
    //
    virtual void transmitByte( BYTE data );

    //---------------------------------------------------------------
    // Writes a data byte to the hardware interface (register)
    // data: Transmitted data byte
    //
    virtual void setByte( BYTE data ) = 0;

    //---------------------------------------------------------------
    // Reads a data byte from the hardware interface (register)
    // and returns it
    // return: Received data byte
    //
    virtual BYTE getByte( void ) = 0;

  protected:
    //---------------------------------------------------------------
    Std::Fifo<BYTE>  dataRxBuf; // Receive buffer
    Std::Fifo<BYTE>  dataTxBuf; // Transmit buffer

  protected:
    //---------------------------------------------------------------
    volatile bool txReady;   // Transmit ready flag,
                             // true after transmitting last data byte

  public:
    //---------------------------------------------------------------
    Std::Report report;

}; //class Uart

}  } //namespace

#endif
