//*******************************************************************
/*!
\file   Uart_Stdio.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   16.09.2020

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_UART_STDIO_H
#define _HW_UART_STDIO_H

//*******************************************************************
#include "Hardware/Common/Uart/Uart.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class Uart_Stdio

\brief Emulate an UART/Terminal interface by stdin/stdout

\example HwUart.cpp
*/
class Uart_Stdio : public Uart
{
  public:
    //---------------------------------------------------------------
    /*! Initialize stdio as UART interface
        Control keys (arrow keys) are filtered out and mapped to
        an equivalent terminal ESC-sequence.
        Control key are never echoed.
        \param echo true: input is echoed on stdout\n
                    false: no echo
    */
    Uart_Stdio( bool echo = EchoOff );

    //-------------------------------------------------------------------
    // We have to overwrite these functions, because there is no Interrupt!
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    virtual bool get( BYTE &data );

    //-------------------------------------------------------------------
    virtual BYTE get( void );

    //-------------------------------------------------------------------
    virtual void transmitByte( BYTE c );

  private:
    //----------------------------------------
    virtual BYTE getByte( void );

    //----------------------------------------
    virtual void setByte( BYTE c );

  public:
    //---------------------------------------------------------------
    static const bool EchoOn  = true;  //!< Input echo on
    static const bool EchoOff = false; //!< Input echo off

  private:
    //----------------------------------------
    bool echo;
    BYTE ctrlState;

}; //class Uart_Stdio

}  } //namespace

#endif
