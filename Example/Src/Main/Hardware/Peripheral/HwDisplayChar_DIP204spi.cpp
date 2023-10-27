//*******************************************************************
/*!
\file   HwDisplayChar_DIP204spi.cpp
\author Thomas Breuer
\date   20.03.2023

\brief  Sample of using hardware related classes
*/

//*******************************************************************
/*
Usage:    Connect a EA DIP204 display module to SPI

          Board sends characters to the display, a consecutive number
          will be printed at the bottom.
*/

//*******************************************************************
#include <stdio.h>
#include "EmbSysLib.h"

//-------------------------------------------------------------------
#include "ReportHandler.h"
#include "configHw.h"

using namespace EmbSysLib::Dev;

Terminal   terminal( uart, 255,255, "erw" );

//=============================================================
#include "Hardware/Peripheral/Display/DisplayChar_DIP204spi.cpp"

DisplayChar_DIP204spi disp( spiDisplay ); 
//==============================================================

//*******************************************************************
int main(void)
{
  terminal.printf( "\r\n\nHwDisplayChar," __DATE__ "," __TIME__ "\r\n\n" );

  BYTE c   = 32; // skip control character
  WORD cnt = 0;
  char txt[21];

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
