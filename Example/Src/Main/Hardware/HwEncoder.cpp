//*******************************************************************
/*!
\file   HwEncoder.cpp
\author Thomas Breuer
\date   26.01.2024
\brief  Sample of using hardware related class "Encoder"

*/

//*******************************************************************
/*
Usage:    ...
          // todo add usage
*/

//*******************************************************************
#include <stdio.h>
#include "EmbSysLib.h"

//-------------------------------------------------------------------
#include "ReportHandler.h"
#include "configHw.h"

//*******************************************************************
char str[40];

int speed = 0;
int pos   = 0;

//*******************************************************************
int main(void)
{
  uart.set( "\r\n\nHwEncoder," __DATE__ "," __TIME__ "\r\n\n" );

  while( 1 )
  {
    char str[40];

    speed  = enc.get();
    pos   += speed;

    sprintf( str, "speed: %5d,  pos: %5d\r", speed, pos );
    uart.set( str );
  }
}
