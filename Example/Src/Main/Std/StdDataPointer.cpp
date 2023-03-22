//*******************************************************************
/*!
\file   StdDataPointer.cpp
\author Thomas Breuer
\date   24.08.2022
\brief  Sample
*/

//*******************************************************************
/*
Configuration:
          Uart

Usage:    Test the DataPointer class

          Output:
            "OK" or "ERROR"

*/

//*******************************************************************
#include <stdio.h>
#include "EmbSysLib.h"

//-------------------------------------------------------------------
#include "ReportHandler.h"
#include "config.h"

//*******************************************************************
class Test
{
  public:
    //---------------------------------------------------------------
    BYTE buffer[100];

    // Byte wise manipulation
    void fill( DataPointer dp )
    {
      BYTE *ptr = dp.getPtr();
      for( WORD idx=0; idx<dp.getSize(); idx++ )
      {
        ptr[idx] = idx+1;
      }
    }

    //
    void write( DataPointer dp )
    {
      memcpy( buffer, dp.getPtr(), MIN(dp.getSize(),(WORD)100) );
    }

    //
    void read( DataPointer dp )
    {
      memcpy( dp.getPtr(), buffer, MIN(dp.getSize(),(WORD)100) );
    }

    DataPointer get()
    {
      return( DataPointer(buffer,100) );
    }
};

//*******************************************************************
class Data
{
  public:
    //---------------------------------------------------------------
    Data()
    {
      dataA = 0;
      dataB = 0;
    }

    //---------------------------------------------------------------
    DWORD dataA;
    DWORD dataB;
};

//*******************************************************************
int main(void)
{
  uart.set( "\r\n\nStdDataPointer," __DATE__ "," __TIME__ "\r\n\n" );
  uart.set( "running ...\r\n" );

  Data X,Y,*Z;
  Test test;

  test.fill ( X );
  test.write( X );
  test.read ( Y );
  Z = test.get();

  // check
  if(   Y.dataB == 0x08070605 && Y.dataB == Z->dataB 
     && Y.dataA == 0x04030201 && Y.dataA == Z->dataA )
  {
    uart.set( "OK\r\n" );
  }
  else
  {
    uart.set( "ERROR\r\n" );
  }

  while( 1 )
  {
  }
}
