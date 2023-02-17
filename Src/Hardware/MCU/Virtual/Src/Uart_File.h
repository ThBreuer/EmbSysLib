//*******************************************************************
/*!
\file   Uart_File.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   16.09.2020

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_UART_FILE_H
#define _HW_UART_FILE_H

//*******************************************************************
#include "Hardware/Common/Uart/Uart.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class Uart_File

\brief Emulate an UART interface by file read/write

\example HwUart.cpp
*/
class Uart_File : public Uart
{
  public:
    //---------------------------------------------------------------
    /*! Initialize UART interface (file I/O)
        If an empty file name is specified, stdin/stdout will be used
        \param fileNameRx Name of file with receiving data (optional)
        \param fileNameTx Name of file with transmitting data (optional)
    */
    Uart_File( const char *fileNameRx,
               const char *fileNameTx );

    //-------------------------------------------------------------------
    // We have to overwrite these functions, because there is no Interrupt!
    //-------------------------------------------------------------------
    //-------------------------------------------------------------------
    virtual bool get( BYTE &data );

    //-------------------------------------------------------------------
    virtual BYTE get( void );

  protected:
    //-------------------------------------------------------------------
    virtual void transmitByte( BYTE c );

  private:
    //-------------------------------------------------------------------
    virtual BYTE getByte( void );

    //-------------------------------------------------------------------
    virtual void setByte( BYTE c );

  private:
    //-------------------------------------------------------------------
    int fhTxD;
    int fhRxD;

  public:
    //---------------------------------------------------------------
    Std::Report report;

}; //class Uart_File

}  } //namespace

#endif
