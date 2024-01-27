//*******************************************************************
/*!
\file   Uart_Stdio.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   16.09.2020

License: See file "LICENSE"
*/

//*******************************************************************
#include "Uart_Stdio.h"
#include "Hardware/ReportID_Hw.h"

#include <stdio.h>
#include <conio.h>

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// Uart_Stdio
//
//*******************************************************************
//-------------------------------------------------------------------
Uart_Stdio::Uart_Stdio( bool echoIn )

: Uart( ReportID_Hw::Module::UART_STDIO, 4096 , 4096 ) // FIFO not used
{
  echo      = echoIn;
  ctrlState = 0;
}

//-------------------------------------------------------------------
bool Uart_Stdio::get( BYTE &data )
{
  if( ctrlState == 1)
  {
    ctrlState = 2;
    data = '[';
    return( true );
  }
  if( _kbhit() )
  {
    data = _getch();

    if( data == 224 )
    {
      ctrlState  = 1;
      data = 27;
      return( true );
    }
    else if( ctrlState == 2)
    {
      ctrlState = 0;

      // Remap some key codes for compatibility
      switch( data )
      {
        case 'H': data = 'A'; return( true );
        case 'P': data = 'B'; return( true );
        case 'M': data = 'C'; return( true );
        case 'K': data = 'D'; return( true );
        default:              return( true );
      }
    }
    else
    {
      if( echo == EchoOn )
      {
        putch( data );
        if( data == '\r' )
        {
          putch( '\n' );
        }
      }
      return( true );
    }
  }
  else
  {
    return( false );
  }
}

//-------------------------------------------------------------------
BYTE Uart_Stdio::get( void )
{
  return( Uart::get());
}

//-------------------------------------------------------------------
void Uart_Stdio::transmitByte( BYTE c )
{
  putch( c );
}

//-------------------------------------------------------------------
BYTE Uart_Stdio::getByte( void )
{
  return( false );
}

//-------------------------------------------------------------------
void Uart_Stdio::setByte( BYTE c )
{
}

}  } //namespace

//EOF
