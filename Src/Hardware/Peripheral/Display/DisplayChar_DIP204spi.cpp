//*******************************************************************
/*!
\file   DisplayChar_DIP204spi.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   20.03.2023

License: See file "LICENSE"
*/

//*******************************************************************
#include "DisplayChar_DIP204spi.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// DisplayChar_DIP204spi
//
//*******************************************************************
//-------------------------------------------------------------------
DisplayChar_DIP204spi::DisplayChar_DIP204spi( SPImaster::Device &spiIn )

: DisplayChar( 0 /* todo ... */, NUM_OF_LINE, NUM_OF_COLUMN ),
  spi( spiIn )

{
  init();
}

//---------------------------------------------------------------
void DisplayChar_DIP204spi::clear( void )
{
  writeCmd( 0x01 ); // Clear Display
}

//---------------------------------------------------------------
void DisplayChar_DIP204spi::gotoTextPos( BYTE lineIn, BYTE columnIn )
{
  line   = lineIn;
  column = columnIn;

  // Startadresse fuer Display-Speicher schreiben
  if( line < NUM_OF_LINE && column < NUM_OF_COLUMN )
  {
    writeCmd( 0x80 | ((line&0x07)<<5) | (column&0x1F) );
  }
}

//---------------------------------------------------------------
void DisplayChar_DIP204spi::putChar( char c )
{
  if( line < NUM_OF_LINE && column < NUM_OF_COLUMN )
  {
    writeDat( c );  // putchar at current pos
    column++;       // emulate cursor increment
  }
}

//-------------------------------------------------------------------
void DisplayChar_DIP204spi::init( void )
{
  writeCmd(  (1<<5)  // Function set (RE=0)
            |(1<<4)	 //   DL:  4-bit,
            |(0<<3)  //   N :  1-line,
            |(1<<2)  //   RE:  1
            |(0<<1)  //   DH:  dot scroll
            |(0<<0));//   REV: normal

  writeCmd(  (1<<3)  // extended Function set (RE=1)
            |(0<<2)  //  FW:  5-dot font width
            |(1<<1)  //  BW:  inverting cursor
            |(1<<0));//  NW:  4-line display

  writeCmd(  (1<<5)  // Function set (RE=1)
            |(1<<4)	 //   DL:  4-bit,
            |(0<<3)  //   N :  1-line,
            |(0<<2)  //   RE:  0
            |(0<<1)  //   BE:  CG/SEG-RAM blink disable
            |(0<<0));//   LP:  normal mode

  writeCmd(  (1<<0));// Clear Display

  writeCmd(  (1<<3)  // Display On/Off Control
            |(1<<2)  //  D:  display on
            |(0<<1)  //  C:  cursor off
            |(0<<0));//  B:  blink off

  // leaving with RE-Bit = 0
}

//-------------------------------------------------------------------
void DisplayChar_DIP204spi::waitBusy( void )
{
  BYTE data[2];
  WORD i = 0x0FFF;

  do
  {
    data[0] = (0x1F)    // START
              | (0<<6)  // RS
              | (1<<5); // RW
    data[1] = 0;
    spi.transceive( data, 2);
  } while( --i && data[1] & 0x80);

  if( i == 0 )
  {
    report.alert(0);//!< \todo i==0 is an Error -> report!
  }
}

//-------------------------------------------------------------------
void DisplayChar_DIP204spi::writeCmd( BYTE cmd )
{
  BYTE data[3];

  data[0] =   (0x1F)  // START
            | (0<<6)  // RS
            | (0<<5); // RW
  data[1] = (cmd   ) & 0x0F;
  data[2] = (cmd>>4) & 0x0F;
  spi.transceive( data, 3);

  waitBusy();
}

//-------------------------------------------------------------------
void DisplayChar_DIP204spi::writeDat( BYTE dat )
{
  BYTE data[3];

  data[0] =   (0x1F)  // START
            | (1<<6)  // RS
            | (0<<5); // RW
  data[1] = (dat    ) & 0x0F;
  data[2] = (dat>>4 ) & 0x0F;
  spi.transceive( data, 3);

  waitBusy();
}

}  } //namespace

//EOF
