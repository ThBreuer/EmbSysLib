//*******************************************************************
/*!
\file   Uart_Mcu.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   14.02.2011

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_UART_MCU_H
#define _HW_UART_MCU_H

//*******************************************************************
#include "Hardware/Common/Uart/Uart.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class Uart_1

\brief Implementation of UART1

\example HwUart.cpp
*/
class Uart_1 : public Uart
{
  public:
    //---------------------------------------------------------------
    /*! Instantiate UART Interface\n
        Configurates hardware (baud rate and mode) and allocates
        receive and transmit buffers.
        \param mode Baud rate
        \param sizeRxBuf Size of receive buffer
        \param sizeTxBuf Size of transmit buffer
    */
    Uart_1( DWORD baudRate,
            WORD  sizeRxBuf,
            WORD  sizeTxBuf );

  private:
    //---------------------------------------------------------------
    virtual BYTE getByte( void );

    //---------------------------------------------------------------
    virtual void setByte( BYTE c );

  public:
    //---------------------------------------------------------------
    static Uart *uartPtr;

};  //class Uart_1

}  } //namespace

#endif
