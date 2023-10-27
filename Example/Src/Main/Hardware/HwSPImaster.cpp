//*******************************************************************
/*!
\file   HwSPImaster.cpp
\author Thomas Breuer
\date   31.01.2013
\brief  Sample of using hardware related classes
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
BYTE dataOut = 0;
BYTE dataOutPrev = 0;
BYTE dataIn  = 0;
WORD err     = 0;
char str[50];

//*******************************************************************
int main( void )
{
  uart.set( "\r\n\nHwSPImaster," __DATE__  "," __TIME__ "\r\n\n" );

  while( 1 )
  {
    dataIn = spiMaster.transceive( dataOut );
    
    dataIn ^= 0x55;

    if(dataIn != dataOutPrev)
    {
      err++;
    }

    sprintf( str, "out: 0x%02x,  in: 0x%02x, err: %u\r",
                  dataOutPrev,
                  dataIn,
                  err );
    uart.set( str );

    dataOutPrev = dataOut;

    dataOut = rand();
  }
}
