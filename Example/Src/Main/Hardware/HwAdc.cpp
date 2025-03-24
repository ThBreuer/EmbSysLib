//*******************************************************************
/*!
\file   HwAdc.cpp
\author Thomas Breuer
\date   10.08.2022
\brief  Sample of using hardware related classes
*/

//*******************************************************************
/*
Usage: Scan ADC-Channels
       Send a command via terminal to the µC:
         'n' - Select next channel
         'p' - Select previous channel
       The number of the active channel is displayed together with
       the measured values
*/

//*******************************************************************
#include <stdio.h>
#include "EmbSysLib.h"

//-------------------------------------------------------------------
#include "ReportHandler.h"
#include "configHw.h"

//*******************************************************************
int main(void)
{
  uart.set( "\r\n\nHwAdc," __DATE__ "," __TIME__ "\r\n\n" );

  const BYTE ch_max = (BYTE)sizeof( AdcChannelList );

  WORD value;
  BYTE ch = 0;
  char str[30];

  for( BYTE i = 0; i < ch_max; i++ )
  {
    adc.enable( AdcChannelList[i] );
  }

  while( 1 )
  {
    value = adc.get( AdcChannelList[ch] );

    sprintf( str, "\rADC(%2d): %5u  \r", AdcChannelList[ch], value );
    uart.set( str );

    // select channel
    switch( uart.get() )
    {
      case 'n': if( ch+1 < ch_max ) ch++; break;
      case 'p': if( ch   > 0      ) ch--; break;
    }
  }
}
