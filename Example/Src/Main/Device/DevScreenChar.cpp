//*******************************************************************
/*!
\file   DevScreenChar.cpp
\author Thomas Breuer
\date   26.02.2023
\brief  Sample Device Screen (character)
*/

//*******************************************************************
/*
Usage:    A character test output is displayed
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
  BYTE cnt = 0;

  while( 1 )
  {
    if( cnt == 0 )
    {
      screenChar.clear();

      screenChar.printf( 0, 0, "DevScreenChar" );
      screenChar.printf( 1, 0, __DATE__ "," __TIME__"\r\n" );
      screenChar.printf( 2, 0, "123456789012345678901234567890" );
      screenChar.printf( 3, 0, "------------------------------" );
    }
    screenChar.printf( 3, 3,  "cnt = %d", cnt++ );
    screenChar.refresh();
  }
}
