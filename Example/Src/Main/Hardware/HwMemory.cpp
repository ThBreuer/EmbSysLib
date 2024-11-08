//*******************************************************************
/*!
\file   HwMemory.cpp
\author Thomas Breuer
\date   10.08.2022
\brief  Sample of using hardware related class 'Memory'
*/

//*******************************************************************
/*
Usage:    Non-volatile memory
          The memory is read after a reset and the reading is displayed,
          then the stored value is modified.
          Reset the uC to check new readings by pressing the "RESET" button.
          Be patient: "erase memory..." could take a while.

Code:     Select the memory hardware used by assigning the
          corresponding object to reference "mem" in configHw.h
*/

//*******************************************************************
#include <stdio.h>
#include "EmbSysLib.h"

//-------------------------------------------------------------------
#include "ReportHandler.h"
#include "configHw.h"

//*******************************************************************
int main( void )
{
  uart.set( "\r\n\nHwMemory," __DATE__ "," __TIME__ "\r\n\n" );

  BYTE data = 0;
  char str[40];

  if( mem.isFlash() )
  {
    uart.set( "Flash-Memory with block erase\r\n" );
  }
  else
  {
    uart.set( "Non-Flash-Memory with random access\r\n" );
  }

  data = mem.read( 0 );

  // Print memory content
  sprintf( str, "read: %u ( ", data);
  uart.set( str );
  for(int i=0;i<8;i++)
  {
    sprintf( str, "%u ", mem.read( i ));
    uart.set( str );
  }
  uart.set( ")\r\n" );

  // change data
  data++;

  if( data > 4 )
  {
    uart.set( "erase memory...\r\n" );
    mem.erase();
  }
  else
  {
    sprintf( str, "write: %u\r\n", data );
    uart.set( str );

    mem.unlock();
    for( int addr = 0; addr < 8; addr++, data++ )
    {
      mem.write( addr, data );
    }
    mem.lock();
  }

  uart.set( "done\r\n" );
  while(1)
  {
    asm("NOP");
  }
}
