//*******************************************************************
/*!
\file   Uart_Mcu.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   16.10.2022

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
\class Uart_Mcu

\brief Implementation of UART hardware

\example HwUart.cpp
*/
class Uart_Mcu : public Uart
{
  public:
    //---------------------------------------------------------------
    /*! \enum USART_Id
        \brief USART identification
    */
    typedef enum
    {
      USART_1 = 1, //!< USART 1
      USART_2,     //!< USART 2
    } USART_Id;

  public:
    //---------------------------------------------------------------
    /*! Initialize the UART hardware
        Default Configuration: 8 bit, no parity, 1 stop-bit
        \param id   USART identification
        \param baud Baud rate
        \param sizeRxBuf Size of receive buffer
        \param sizeTxBuf Size of transmit buffer
    */
    Uart_Mcu( USART_Id id,
              DWORD    baud,
              WORD     sizeRxBuf,
              WORD     sizeTxBuf );

    //---------------------------------------------------------------
    // Interrupt Service Routine (ISR) of USART interrupt.
    // This function must be public, because it is called
    // in extern "C"
    void isr( void );

  private:
    //---------------------------------------------------------------
    virtual BYTE getByte( void );

    //---------------------------------------------------------------
    virtual void setByte( BYTE c );

    //---------------------------------------------------------------
    virtual void setBaudrate( DWORD baud );

  public:
    //---------------------------------------------------------------
    static Uart_Mcu *uartPtr_1;
    static Uart_Mcu *uartPtr_2;

  private:
    //---------------------------------------------------------------
    USART_TypeDef *ptr;

}; //class Uart_Mcu

}  } //namespace

#endif
