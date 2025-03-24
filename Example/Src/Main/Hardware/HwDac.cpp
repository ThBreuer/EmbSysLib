//*******************************************************************
/*!
\file   HwDac.cpp
\author Thomas Breuer
\date   10.08.2022
\brief  Sample of using hardware related classes
*/

//*******************************************************************
/*
Usage: Generate a DAC output
       Send a command via terminal to the µC:
         'n' - Select next channel
         'p' - Select previous channel
         '+' - Increment output voltage by 1/32
         '-' - Decrement output voltage by 1/32
       The number of the active channel is displayed together with
       the output value
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
  uart.set( "\r\n\nHwDac," __DATE__ "," __TIME__ "\r\n\n" );

  const BYTE ch_max = (BYTE)sizeof( DacChannelList );

  BYTE step  = 0;
  WORD value = 0;
  BYTE ch    = 0;
  char str[40];

  for( BYTE i = 0; i < ch_max; i++ )
  {
    dac.enable( DacChannelList[i] );
  }

  while(1)
  {
    dac.set( DacChannelList[ch], value );

    sprintf(str, "\rDAC(%2d): %5u (0x%04x)  \r", DacChannelList[ch], value, value );
    uart.set( str );

    switch( uart.get() )
    {
      // set amplitude
      case '+': if( step < 32 ) step++;    break;
      case '-': if( step >  0 ) step--;    break;

      // select channel
      case 'n': if( ch+1 < ch_max ) ch++; break;
      case 'p': if( ch   > 0      ) ch--; break;
    }
    value = (WORD)((DWORD)0xFFFF*step/32);
  }
}
