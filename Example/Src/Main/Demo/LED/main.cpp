//*******************************************************************
/*!
\file   main.cpp
\author Thomas Breuer
\date   07.11.2022
\brief  Sample
*/

//*******************************************************************
/*
Used Configuration (see config.h):
        btn_A, led_A

Usage:
        Press and hold button A to switch on led A
*/

//*******************************************************************
#include <stdio.h>
#include "EmbSysLib.h"

//-------------------------------------------------------------------
#include "ReportHandler.h"
#include "config.h"

//*******************************************************************
int main(void)
{
  terminal.printf( "\r\n\nDemo/LED," __DATE__ "," __TIME__ "\r\n\n" );

  while( 1 )
  {
    led_A = btn_A;
  }
}
