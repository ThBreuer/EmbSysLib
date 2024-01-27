//*******************************************************************
/*!
\file   HwI2Cmaster.cpp
\author Thomas Breuer
\date   10.08.2022
\brief  Sample of using hardware related class 'I2Cmaster'
*/

//*******************************************************************
/*
Usage:    \todo
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
  uart.set( "\r\n\nHwI2Cmaster," __DATE__ "," __TIME__ "\r\n\n" );

  I2Cmaster::Device i2cDevice( i2cBus, 0xB0 /* I2Sslave device address */ );

  DWORD w = 0;
  DWORD r = 0;
  DWORD err = 0;
  char str[256];

  while( 1 )
  {
    w = w ? w<<1 : 1;
    i2cDevice.write( (BYTE)0xff, w );
    i2cDevice.read( (BYTE)0xff, &r );
    if( w != r )
      err++;
    
    sprintf(str,"w:%08lx  r:%08lx (%ld)\r\n", 
                (unsigned long)w,
                (unsigned long)r, 
                (unsigned long)err );
    uart.set( str );
  }
}
