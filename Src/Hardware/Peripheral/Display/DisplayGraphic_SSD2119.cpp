//*******************************************************************
/*!
\file   DisplayGraphic_SSD2119.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   09.10.2018

License: See file "LICENSE"
*/

//*******************************************************************
#include "DisplayGraphic_SSD2119.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// DisplayGraphic_SSD2119
//
//*******************************************************************
//-------------------------------------------------------------------
DisplayGraphic_SSD2119::DisplayGraphic_SSD2119( BYTE        *baseAddr,
                                                Port::Pin   &pinReset,
                                                Port::Pin   &pinBL,
                                                Font         fntIn,
                                                BYTE         zoomIn )

: DisplayGraphic( ReportID_Hw::Module::DISPLAY_GRAPHIC_SSD2119,
                  WIDTH,
                  HEIGHT,
                  fntIn,
                  zoomIn ),
  reset(pinReset)

{
  ptrCmd  = (volatile WORD *)(baseAddr + 0x00000000 );
  ptrData = (volatile WORD *)(baseAddr + 0x00100000 );

  reset.setMode( Port::Out );

  pinBL.setMode( Port::Out );
  pinBL.clr(); // backlight off
  init();
  pinBL.set(); // backlight on
}

//-------------------------------------------------------------------
void DisplayGraphic_SSD2119::clear( void )
{
  setArea( 0, 0, WIDTH, HEIGHT );

  for( WORD x = 0; x < WIDTH; x++)
  {
    for( WORD y = 0; y < HEIGHT; y++)
    {
      setPixel( BackColor );
    }
  }
}

//---------------------------------------------------------------
void DisplayGraphic_SSD2119::setArea( WORD x,
                                      WORD y,
                                      WORD w,
                                      WORD h )
{
  writeCmd( H_RAM_START, x );
  writeCmd( H_RAM_END,   x+w-1 );
  writeCmd( V_RAM_POS,   x | ((y+h) << 8) );
  writeCmd( X_RAM_ADDR,  x );
  writeCmd( Y_RAM_ADDR,  y );
  writeCmd( RAM_DATA );
}

//---------------------------------------------------------------
void DisplayGraphic_SSD2119::setPixel( WORD color,
                                       WORD x,
                                       WORD y )
{
  setArea( x, y, 1, 1 );   // set draw position
  writeCmd( RAM_DATA );
  writeDat( color );
}

//---------------------------------------------------------------
void DisplayGraphic_SSD2119::setPixel( WORD color )
{
  writeDat( color );
}

//---------------------------------------------------------------
void DisplayGraphic_SSD2119::init( void )
{
  /* Reset LCD */
  reset.clr(); //
  System::delayMilliSec( 20 );
  reset.set();
  System::delayMilliSec( 20 );

  writeCmd( SLEEP_MODE_1,      0x0001 ); // Enter sleep mode
  writeCmd( PWR_CTRL_5,        0x00B2 );
  writeCmd( VCOM_OTP_1,        0x0006 );
  writeCmd( OSC_START,         0x0001 );
  writeCmd( OUTPUT_CTRL,       0x30EF ); // Set pixel format and basic display orientation (scanning direction)
  writeCmd( LCD_DRIVE_AC_CTRL, 0x0600 );
  writeCmd( SLEEP_MODE_1,      0x0000 ); // Exit sleep mode

  System::delayMilliSec( 20 );

  writeCmd( ENTRY_MODE,        0x6830 ); // Pixel color format and MCU interface parameters
  writeCmd( SLEEP_MODE_2,      0x0999 ); // Analog parameters
  writeCmd( ANALOG_SET,        0x3800 );
  writeCmd( DISPLAY_CTRL,      0x0033 ); // Enable the display
  writeCmd( PWR_CTRL_2,        0x0005 ); // Set VCIX2 voltage to 6.1V
  writeCmd( PWR_CTRL_3,        0x000A ); // Configure Vlcd63 and VCOMl
  writeCmd( PWR_CTRL_4,        0x2E00 );

  /* Configure gamma correction.*/
  writeCmd( GAMMA_CTRL_1,  0x0000 );
  writeCmd( GAMMA_CTRL_2,  0x0303 );
  writeCmd( GAMMA_CTRL_3,  0x0407 );
  writeCmd( GAMMA_CTRL_4,  0x0301 );
  writeCmd( GAMMA_CTRL_5,  0x0301 );
  writeCmd( GAMMA_CTRL_6,  0x0403 );
  writeCmd( GAMMA_CTRL_7,  0x0707 );
  writeCmd( GAMMA_CTRL_8,  0x0400 );
  writeCmd( GAMMA_CTRL_9,  0x0a00 );
  writeCmd( GAMMA_CTRL_10, 0x1000 );

  clear();
}

//-------------------------------------------------------------------
__inline void DisplayGraphic_SSD2119::writeCmd( BYTE reg, WORD value )
{
  *ptrCmd  = reg;
  *ptrData = value;
}

//-------------------------------------------------------------------
 __inline void DisplayGraphic_SSD2119::writeCmd( BYTE reg )
{
  *ptrCmd = reg;
}

//-------------------------------------------------------------------
__inline void DisplayGraphic_SSD2119::writeDat( WORD dat )
{
  /* Write 16-bit GRAM Reg */
  *ptrData = dat;
}

}  } //namespace

//EOF
