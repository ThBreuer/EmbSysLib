//*******************************************************************
/*!
\file   HwDisplayChar_Terminal.cpp
\author Thomas Breuer
\date   20.02.2023
\brief  Sample of using hardware related classes
*/

//*******************************************************************
/*
Usage:    Connect board with terminal.
          Board sends characters to terminal, a consecutive number
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
#include "Hardware/Peripheral/Display/DisplayChar_Terminal.cpp"

DisplayChar_Terminal dispTerminal( uart ); 
//==============================================================

//*******************************************************************
int main(void)
{
  terminal.printf( "\r\n\nHwDisplayChar," __DATE__ "," __TIME__ "\r\n\n" );

  BYTE c   = 32; // skip control character
  WORD cnt = 0;
  char txt[21];

  dispTerminal.clear();

  for( BYTE line = 0; line < dispTerminal.getNumberOfLines() - 1; line++ )
  {
    for( BYTE column = 0; column < dispTerminal.getNumberOfColumns(); column++ )
    {
      dispTerminal.gotoTextPos( line, column );
      dispTerminal.putChar( c );
      c++;
    }
  }

  while(1)
  {
    sprintf(txt,"--- %6d ---------",cnt++);
    dispTerminal.gotoTextPos( dispTerminal.getNumberOfLines() - 1, 0 );
    dispTerminal.putString( txt );
    dispTerminal.refresh();
  }
}
