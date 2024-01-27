//*******************************************************************
/*!
\file   DisplayGraphic_SSD1306i2c.cpp
\author Thomas Breuer
\date   08.01.2024

The implementation of this class is based on a library written by 
Olivier Van den Eede and modified by Aleksander Alekseev.
\see GitHub: https://github.com/afiskon/stm32-ssd1306

License: See file "LICENSE"
*/

//*******************************************************************
#include "DisplayGraphic_SSD1306i2c.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// DisplayGraphic_SSD1306i2c
//
//*******************************************************************
//-------------------------------------------------------------------
DisplayGraphic_SSD1306i2c::DisplayGraphic_SSD1306i2c( I2Cmaster   &i2cMaster,
                                                      bool         rotate,
                                                      Font         font,
                                                      BYTE         zoom )

: DisplayGraphic( ReportID_Hw::Module::DISPLAY_GRAPHIC_OTM8009A,
                  lcd_x_size,
                  lcd_y_size,
                  font,
                  zoom )
, i2c( i2cMaster, (0x3C << 1) /*I2C address*/ )
{
  this->rotate = rotate;

  setArea(0,0,0,0);
  init();
}

//---------------------------------------------------------------
void DisplayGraphic_SSD1306i2c::refresh( void )
{
  // Write data to each page of RAM. 
  for( BYTE i = 0; i < lcd_page_size; i++ )
  {
    writeCommand(0xB0 + i);  // Set the current RAM page address.
    writeCommand(0x00 + 0 ); // SSD1306_X_OFFSET_LOWER
    writeCommand(0x10 + 0 ); // SSD1306_X_OFFSET_UPPER

    i2c.write((BYTE)0x40, displayRam.page[i] );
  }
}

//---------------------------------------------------------------
void DisplayGraphic_SSD1306i2c::clear( void )
{
  memset( (BYTE*)&displayRam, BackColor?0xFF:0x00, sizeof(DisplayRam) );
  refresh();
}

//---------------------------------------------------------------
void DisplayGraphic_SSD1306i2c::setArea( WORD x,
                                         WORD y,
                                         WORD w,
                                         WORD h )
{
  ax = x;
  ay = y;
  aw = w;
  ah = h;
  xpos  = ax;
  ypos  = ay;
  page  = &displayRam.page[ypos>>3];
  bmask = 1<<(ypos & 0x07);
}

//---------------------------------------------------------------
void DisplayGraphic_SSD1306i2c::setPixel( WORD color,
                                          WORD x,
                                          WORD y  )
{
  if( x >= lcd_x_size || y >= lcd_y_size ) 
  {
    // Don't write outside the buffer
    return;
  }

  if( color )
  {
    displayRam.page[y>>3].data[x] |= 1 << (y & 0x07); 
  } 
  else 
  { 
    displayRam.page[y>>3].data[x] &= ~(1 << (y & 0x07));
  }
}

//---------------------------------------------------------------
void DisplayGraphic_SSD1306i2c::setPixel( WORD color )
{
  if( xpos >= lcd_x_size || ypos >= lcd_y_size ) 
  {
    // Don't write outside the buffer
    return;
  }

  if( color )
  {
    page->data[xpos] |= bmask; 
  } 
  else 
  { 
    page->data[xpos] &= ~bmask;
  }

  xpos++;
  if( xpos >= ax+aw)
  {
    xpos = ax;
    ypos++;
    page  = &displayRam.page[ypos>>3];
    bmask = 1<<(ypos & 0x07);
  }
}

//---------------------------------------------------------------
void DisplayGraphic_SSD1306i2c::init( )
{
    // Init OLED
    writeCommand(0xAE);  //display off
  
    writeCommand(0x20); //Set Memory Addressing Mode
    writeCommand(0x00); // 00b,Horizontal Addressing Mode; 01b,Vertical Addressing Mode;
                        // 10b,Page Addressing Mode (RESET); 11b,Invalid

    writeCommand(0xB0); //Set Page Start Address for Page Addressing Mode,0-7

    if( rotate )
    {
      writeCommand(0xC0); // Mirror vertically: off
      writeCommand(0xA0); // Mirror horizontally: off
    }
    else
    {
      writeCommand(0xC8); //Mirror vertically:Set COM Output Scan Direction
      writeCommand(0xA1); //Mirror horizontally:--set segment re-map 0 to 127 - CHECK
    }

    writeCommand(0x00); //---set low column address
    writeCommand(0x10); //---set high column address

    writeCommand(0x40); //--set start line address - CHECK

    writeCommand(0x81); //--set contrast
    writeCommand(0xFF);

    writeCommand(0xA6); //--set normal color

    // Set multiplex ratio.
    //#if (SSD1306_HEIGHT == 128)
    // Found in the Luma Python lib for SH1106.
    //writeCommand(0xFF);
    //#else
    writeCommand(0xA8); //--set multiplex ratio(1 to 64) - CHECK
    //#endif

    //#if (SSD1306_HEIGHT == 32)
    //    writeCommand(0x1F); //
    //#elif (SSD1306_HEIGHT == 64)
    writeCommand(0x3F); //
    //#elif (SSD1306_HEIGHT == 128)
    //    writeCommand(0x3F); // Seems to work for 128px high displays too.
    //#else
    //#error "Only 32, 64, or 128 lines of height are supported!"
    //#endif

    writeCommand(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content

    writeCommand(0xD3); //-set display offset - CHECK
    writeCommand(0x00); //-not offset

    writeCommand(0xD5); //--set display clock divide ratio/oscillator frequency
    writeCommand(0xF0); //--set divide ratio

    writeCommand(0xD9); //--set pre-charge period
    writeCommand(0x22); //

    writeCommand(0xDA); //--set com pins hardware configuration - CHECK
    //#if (SSD1306_HEIGHT == 32)
    //    writeCommand(0x02);
    //#elif (SSD1306_HEIGHT == 64)
    writeCommand(0x12);
    //#elif (SSD1306_HEIGHT == 128)
    //    writeCommand(0x12);
    //#else
    //#error "Only 32, 64, or 128 lines of height are supported!"
    //#endif

    writeCommand(0xDB); //--set vcomh
    writeCommand(0x20); //0x20,0.77xVcc

    writeCommand(0x8D); //--set DC-DC enable
    writeCommand(0x14); //
  
    writeCommand(0xAF); //--turn on SSD1306 panel  

    clear();
}

//---------------------------------------------------------------
void DisplayGraphic_SSD1306i2c::writeCommand( BYTE byte ) 
{
  i2c.write( (BYTE)0x00, byte );
}

}  } //namespace

//EOF
