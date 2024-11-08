//*******************************************************************
/*!
\file   StdCrc.cpp
\author Thomas Breuer
\date   18.12.2023
\brief  Sample
*/

//*******************************************************************
/*
Configuration:
          Uart

Usage:    \todo
*/

//*******************************************************************
#include <stdio.h>
#include "EmbSysLib.h"

//-------------------------------------------------------------------
#include "ReportHandler.h"
#include "config.h"

//*******************************************************************
int main(void)
{
  char str[80];
  uart.set( "\r\n\nStdCRC," __DATE__ "," __TIME__ "\r\n\n" );

  BYTE data[] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB };

  // Variante 1
  Crc crc1( Crc::SMALL );
  crc1( data, sizeof(data) );
  sprintf( str, "crc:%04X \r\n", crc1.get() );
  uart.set( str );

  // Variante 2
  Crc crc2( Crc::FAST );
  crc2( data, sizeof(data) );
  sprintf( str, "crc:%04X \r\n", crc2.get() );
  uart.set( str );

  // Variante 3
  crc2.reset();
  for( BYTE i=0;i<sizeof(data);i++)
  {
    crc2( data[i] );
  }
  sprintf( str, "crc:%04X \r\n", crc2.get() );
  uart.set( str );

  while( 1 )
  {
    asm("NOP");
  }
}
