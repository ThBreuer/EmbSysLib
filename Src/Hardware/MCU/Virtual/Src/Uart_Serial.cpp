//*******************************************************************
/*!
\file   Uart_Serial.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   14.02.2011

License: See file "LICENSE"

\todo Fehlermeldung / Report: unterscheiden zw. Modul und Win-Fehlermeldungen mit Get error()
*/

//*******************************************************************
#include "Uart_Serial.h"
#include "Hardware/ReportID_Hw.h"

#include <stdio.h>

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// Uart_Serial
//
//*******************************************************************
//-------------------------------------------------------------------
Uart_Serial::Uart_Serial( Baudrate baud, const char *portName )
: Uart( ReportID_Hw::Module::UART_SERIAL, 4096, 4096 ) // FIFO not used
{
  this->baud = baud;
  isOpenFlag = false;
  hSerial    = INVALID_HANDLE_VALUE;

  open( portName );
}

//-------------------------------------------------------------------
void Uart_Serial::open( const char *portName )
{
  char portNameLoc[33];

  if( portName == 0 || strlen(portName) == 0 )
  {
    return;
  }

  snprintf( portNameLoc, 32, "\\\\.\\%s", portName );
  if( isOpenFlag )
  {
    close();
  }

  DCB          dcbSerialParams = {0};
  COMMTIMEOUTS timeouts        = {0};

  // Open serial port
  hSerial = CreateFile( portNameLoc,
                        GENERIC_READ|GENERIC_WRITE,
                        0,
                        NULL,
                        OPEN_EXISTING,
                        0,
                        NULL);
  if (hSerial == INVALID_HANDLE_VALUE)
  {
    report.alert( GetLastError() );
    return;
  }

  // Set device parameters
  dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
  if( GetCommState(hSerial, &dcbSerialParams) == 0 )
  {
    CloseHandle( hSerial );
    report.error( GetLastError() );
    return;
  }

  dcbSerialParams.BaudRate = baud;
  dcbSerialParams.ByteSize = 8;
  dcbSerialParams.StopBits = ONESTOPBIT;
  dcbSerialParams.Parity   = NOPARITY;
  if( !SetCommState(hSerial, &dcbSerialParams) )
  {
      CloseHandle( hSerial );
      report.alert( GetLastError() );
      return;
  }

  // Set COM port timeout settings
  timeouts.ReadIntervalTimeout         = MAXDWORD;
  timeouts.ReadTotalTimeoutMultiplier  = 0;
  timeouts.ReadTotalTimeoutConstant    = 0;
  timeouts.WriteTotalTimeoutMultiplier = 10;
  timeouts.WriteTotalTimeoutConstant   = 50;
  if( !SetCommTimeouts( hSerial, &timeouts ) )
  {
    CloseHandle( hSerial );
    report.alert( GetLastError() );
    return;
  }
  isOpenFlag = true;
}

//-------------------------------------------------------------------
bool Uart_Serial::isOpen()
{
    return( isOpenFlag );
}

//-------------------------------------------------------------------
void Uart_Serial::close()
{
  if( hSerial != INVALID_HANDLE_VALUE )
  {
     Sleep( 100 );
     CloseHandle( hSerial );
     hSerial = INVALID_HANDLE_VALUE;
  }
  isOpenFlag = false;
}

//-------------------------------------------------------------------
bool Uart_Serial::get( BYTE &data )
{
  if( !isOpenFlag || hSerial == INVALID_HANDLE_VALUE )
  {
    return( false );
  }

  DWORD dwBytesRead = 0;
  if(    !ReadFile( hSerial, &data, 1, &dwBytesRead, NULL )
      || dwBytesRead == 0 )
  {
    return( false );
  }
  return( true );
}

//-------------------------------------------------------------------
BYTE Uart_Serial::get( void )
{
  return( Uart::get());
}

//-------------------------------------------------------------------
void Uart_Serial::transmitByte( BYTE c )
{
  DWORD dwBytesWritten = 0;

  if( !isOpenFlag || hSerial == INVALID_HANDLE_VALUE )
  {
    return;
  }

  if( !WriteFile( hSerial, &c, 1, &dwBytesWritten, NULL ) )
  {
    close();
    report.alert( GetLastError() );
  }
}

//-------------------------------------------------------------------
void Uart_Serial::set( BYTE c )
{
  transmitByte( c );
}

//-------------------------------------------------------------------
void Uart_Serial::set( const char *c )
{
  set( (BYTE*)c, strlen(c) );
}

//-------------------------------------------------------------------
void Uart_Serial::set( const BYTE *c, BYTE size )
{
  DWORD dwBytesWritten = 0;

  if( !isOpenFlag || hSerial == INVALID_HANDLE_VALUE )
  {
    return;
  }

  if( !WriteFile( hSerial, c, size, &dwBytesWritten, NULL ) || size != dwBytesWritten)
  {
    close();
    report.error( GetLastError() );
  }
}

//-------------------------------------------------------------------
BYTE Uart_Serial::getByte( void )
{
  // nothing to do ...
  return( false );
}

//-------------------------------------------------------------------
void Uart_Serial::setByte( BYTE c )
{
  // nothing to do ...
}

}  } //namespace

//EOF
