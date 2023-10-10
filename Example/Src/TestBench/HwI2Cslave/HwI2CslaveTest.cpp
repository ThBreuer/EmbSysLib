//*******************************************************************
/*!
\file   HwI2Cmaster.cpp
\author Thomas Breuer
\date   10.08.2022
\brief  Test bench to test a class 'I2Cslave' on other device
*/

//*******************************************************************
/*
Usage:    ...
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
  uart.set( "\r\n\nHwI2CslaveTest," __DATE__ "," __TIME__ "\r\n\n" );

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
    
    sprintf(str,"w:%08x  r:%08x (%d)\r\n", w,r, err );
    uart.set( str );
  }
}
