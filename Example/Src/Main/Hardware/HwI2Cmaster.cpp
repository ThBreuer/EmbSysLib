//*******************************************************************
/*!
\file   HwI2Cmaster.cpp
\author Thomas Breuer
\date   10.08.2022
\brief  Sample of using hardware related class 'I2Cmaster'
*/

//*******************************************************************
/*
Usage:    I2C master
          Connect a I/O expander PCF8574 to I2C pins and connect up to 
          8 LEDs to the output of the I/O expander.
          A running light is shown.

Code      Alternatively any two GPIO pins can be used as I2C bus interface
*/

//*******************************************************************
// Set this flag to use I2C master emulation (bit banging):
#define USE_I2C_MASTER_EMUL false

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

  I2Cmaster::Device i2cDevice( i2cBus, 0x40 /* PCF8574 device address */ );

  while( 1 )
  {
    BYTE value = i2cDevice.read();
    i2cDevice.write( (BYTE)(value ? value<<1 : 1) );
    
    System::delayMilliSec( 100 );
  }
}
