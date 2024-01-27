//*******************************************************************
/*!
\file   HwMemory_PCF8583.cpp
\author Thomas Breuer
\date   20.02.2023
\brief  Sample of using hardware related classes
*/

//*******************************************************************
/*
Usage:    I2C Memory Test Class

          Connect to termianl
          Press 'r' to read memory
          Press 'w' to increment memory
          Press 'e' to erase memory
*/

//*******************************************************************
#include <stdio.h>
#include "EmbSysLib.h"

//-------------------------------------------------------------------
#include "ReportHandler.h"
#include "configHw.h"

using namespace EmbSysLib::Dev;

Terminal   terminal( uart, 255,255, "erw" );

//======================================================
#include "Hardware/Peripheral/Memory/Memory_PCF8583.cpp"

Memory_PCF8583  memory( i2cBus, 0 /*sub address*/ );
//======================================================

//*******************************************************************
int main( void )
{
  terminal.printf( "\r\n\nHwMemory_PCF8583," __DATE__ "," __TIME__ "\r\n\n" );

  BYTE data = 0;
  
  while( 1 )
  {
    switch( terminal.get() )
    {
      case 'r':  // Read memory
        terminal.printf( "\r\nread: ");
        for(int i=0;i<16;i++)
        {
          terminal.printf( "%02x ", memory.read( i ) );
        }
        terminal.printf( "ready\r\n");
        break;
        
      case 'e': // Erase memory
        terminal.printf( "\r\nerase: ");
        memory.erase();
        terminal.printf( "ready\r\n");
        break;

      case 'w': // Write new data
        data = memory.read( 0 ) + 1; // change data
        terminal.printf( "\r\nwrite: ");
        memory.unlock();
        for( int addr = 0; addr < 16; addr++, data++ )
        {
          memory.write( addr, data );
        }
        memory.lock();
        terminal.printf( "ready\r\n");
        break;
    }

    if( memory.isError() )
    {
      terminal.printf( "\r\nMemory access error\r\n" );
    }
  }
}
