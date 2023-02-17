//*******************************************************************
/*!
\file   HwTouch.cpp
\author Thomas Breuer
\date   10.08.2022
\brief  Sample of using hardware related classes
*/

//*******************************************************************
/*
Usage:    Read touch screen or mouse pointer input
          Move the mouse pointer into the blue area of the Virtual Device Server and
          press / release the left mouse button.
          The actual mouse position and button state is displayed.
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
  uart.set( "\r\n\nHwTouch," __DATE__ "," __TIME__ "\r\n\n" );

  while( 1 )
  {
    touch.update();

    char str[40];
    sprintf( str, "x=%5d y=%5d t=%c\r",
                  touch.getPosX(),
                  touch.getPosY(),
                  touch.isTouched()?'X':'-' );

    uart.set( str );
  }
}
