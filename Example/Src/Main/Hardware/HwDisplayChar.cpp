//*******************************************************************
/*!
\file   HwDisplayChar.cpp
\author Thomas Breuer
\date   10.08.2022
\brief  Sample of using hardware related classes
*/

//*******************************************************************
/*
Usage:    A test output is displayed
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
  uart.set( "\r\n\nHwDisplayChar," __DATE__ "," __TIME__ "\r\n\n" );

  BYTE c   = 32; // skip control character
  WORD cnt = 0;
  char txt[21];
	
  #ifdef HAS_LCD_ENABLE_PIN
  lcdDisp.set(1);
  #endif

  disp.clear();

  for( BYTE line = 0; line < disp.getNumberOfLines() - 1; line++ )
  {
    for( BYTE column = 0; column < disp.getNumberOfColumns(); column++ )
    {
      disp.gotoTextPos( line, column );
      disp.putChar( c );
      c++;
    }
  }

  while(1)
  {
    sprintf(txt,"--- %6d ---------",cnt++);
    disp.gotoTextPos( disp.getNumberOfLines() - 1, 0 );
    disp.putString( txt );
    disp.refresh();
  }
}
