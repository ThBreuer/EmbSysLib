//*******************************************************************
/*!
\file   Uart_File.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   16.09.2020

License: See file "LICENSE"
*/

//*******************************************************************
#include "Uart_File.h"

//*******************************************************************
#include "Hardware/ReportID_Hw.h"

#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// Uart_File
//
//*******************************************************************
//-------------------------------------------------------------------
Uart_File::Uart_File( const char *fileNameRxD, const char *fileNameTxD )

: Uart( ReportID_Hw::Module::UART_FILE, 4096, 4096 ) // FIFO not used
, report( ReportID_Hw::Module::UART_FILE )
{
  //
  // receive file
  //
  if( fileNameRxD && strlen(fileNameRxD) > 0 )
  {
    fhRxD = open( fileNameRxD, O_RDONLY|O_BINARY );
    if( fhRxD <= 0)
      report.alert( ReportID_Hw::Event::OPEN_ERROR );
    else
      lseek( fhRxD, 0, SEEK_END );
  }
  else
  {
    fhRxD = 0;
  }

  if( fhRxD <= 0 )
  {
      fhRxD = STDIN_FILENO;
  }

  //
  // transmit file
  //
  if( fileNameTxD && strlen(fileNameTxD) > 0 )
  {
    fhTxD = open( fileNameTxD, O_WRONLY|O_CREAT/*|O_TRUNC*/|O_BINARY, S_IREAD|S_IWRITE );
    if( fhTxD <= 0)
      report.alert( ReportID_Hw::Event::OPEN_ERROR );
    else
      lseek( fhTxD, 0, SEEK_END );
  }
  else
  {
    fhTxD = 0;
  }

  if( fhTxD <= 0 )
  {
      fhTxD = STDOUT_FILENO;
  }
}

//-------------------------------------------------------------------
bool Uart_File::get( BYTE &data )
{
  BYTE tmp = 0;

  if( read( fhRxD, &tmp, 1 ) == 1 )
  {
    data = tmp;
    return(true);
  }
  else
  {
    return(false);
  }
}

//-------------------------------------------------------------------
BYTE Uart_File::get( void )
{
  return( Uart::get());
}

//-------------------------------------------------------------------
void Uart_File::transmitByte( BYTE c )
{
  write( fhTxD, &c, 1 );
}

//-------------------------------------------------------------------
BYTE Uart_File::getByte( void )
{
  return( false );
}

//-------------------------------------------------------------------
void Uart_File::setByte( BYTE c )
{
}

}  } //namespace

//EOF
