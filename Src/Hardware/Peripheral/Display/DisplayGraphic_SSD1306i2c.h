//*******************************************************************
/*!
\file   DisplayGraphic_SSD1306i2c.h
\author Thomas Breuer
\date   09.01.2024

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_DISPLAY_GRAPHIC_SSD1306_I2C_H
#define _HW_DISPLAY_GRAPHIC_SSD1306_I2C_H

//*******************************************************************
#include "Hardware/Common/Display/DisplayChar.h"
#include "Hardware/Common/Display/DisplayGraphic.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class DisplayGraphic_SSD1306i2c

\brief LCD display SSD1306 with I2C

\example  HwDisplayGraphic.cpp
*/
class DisplayGraphic_SSD1306i2c : public DisplayGraphic
{
  private:
    //---------------------------------------------------------------
    static const uint32_t lcd_x_size    = 128;
    static const uint32_t lcd_y_size    = 64;
    static const uint32_t lcd_page_size = lcd_y_size/8;

  public:
    //---------------------------------------------------------------
    DisplayGraphic_SSD1306i2c( I2Cmaster   &i2cMaster,
                               bool         rotate,
                               Font         font,
                               BYTE         zoom = 1 );

    //---------------------------------------------------------------
    virtual void clear( void );

    //---------------------------------------------------------------
    virtual void refresh( void );

    //---------------------------------------------------------------
    virtual void setArea( WORD x,
                          WORD y,
                          WORD w,
                          WORD h );

    //---------------------------------------------------------------
    virtual void setPixel( WORD color,
                           WORD x,
                           WORD y );

    //---------------------------------------------------------------
    virtual void setPixel( WORD color );

  private:
    //---------------------------------------------------------------
    void init();

    //---------------------------------------------------------------
    void writeCommand( BYTE byte );

    //---------------------------------------------------------------
    class DisplayRam
    {
      public:
        class Page
        {
          public:
            BYTE data[lcd_x_size];
        } page[lcd_page_size]; 
    } displayRam;

  private:
    //---------------------------------------------------------------
    I2Cmaster::Device i2c;

    bool rotate;
  
    WORD ax;
    WORD ay;
    WORD aw;
    WORD ah;
    WORD xpos;
    WORD ypos;
    BYTE bmask;
    DisplayRam::Page *page;

}; //class DisplayGraphic_SSD1306i2c

}  } //namespace

#endif

