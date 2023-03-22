//*******************************************************************
/*!
\file   CtrlDigitalEncoder.cpp
\author Thomas Breuer
\date   26.01.2023
\brief  Sample of using class DigitalEncoder
*/

//*******************************************************************
/*
Usage:    Connect board with a terminal.

          Operate encoder to increment, decrement or reset a counter.
          The counter is display on the terminal
*/

//*******************************************************************
#include <stdio.h>
#include "EmbSysLib.h"

//-------------------------------------------------------------------
#include "ReportHandler.h"
#include "config.h"

//*******************************************************************
int main( void )
{
  terminal.printf( "\r\n\nCtrlDigitalEncoder," __DATE__ "," __TIME__ "\r\n\n" );

  int cnt = 0;
  int btn = 0;

  while( 1 )
  {
    switch( encoder.getEvent() )
    {
      case DigitalEncoder::LEFT:     cnt -= 1;             break;
      case DigitalEncoder::RIGHT:    cnt += 1;             break;
      case DigitalEncoder::CTRL_DWN: cnt  = 0;  btn  = 1;  break;
      case DigitalEncoder::CTRL_UP:             btn  = 0;  break;
      default:                                             break;
    }
    terminal.printf( "\rcnt=%3d  btn=%3s\r", cnt, (btn)?"DWN":"UP " );
  }
}
