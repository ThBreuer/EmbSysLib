//*******************************************************************
/*!
\file   HwDisplayGraphic_SSD1306i2c.cpp
\author Thomas Breuer
\date   10.01.2024

\brief  Sample of using hardware related classes
*/

//*******************************************************************
/*
Usage:    Connect a SSD1306 I2C display module to I2C

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
//*******************************************************************
#include "Hardware/Peripheral/Display/DisplayGraphic_SSD1306i2c.cpp"

#include "Resource/Font/Font_8x12.h"

DisplayGraphic_SSD1306i2c disp( i2cBus, 
                                false,
                                fontFont_8x12, 
                                1 );
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
//    DisplayChar_DIP204spi disp( spiDisplay ); 
    
    sprintf(txt,"--- %6d ---------",cnt++);
    disp.gotoTextPos( disp.getNumberOfLines() - 1, 0 );
    disp.putString( txt );
    disp.refresh();
  }
}
