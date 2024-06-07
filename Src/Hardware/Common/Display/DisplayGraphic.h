//*******************************************************************
/*!
\file   DisplayGraphic.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   01.12.2021

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_DISPLAY_GRAPHIC_H
#define _HW_DISPLAY_GRAPHIC_H

//*******************************************************************
#include "Std/Report.h"

#include "DisplayChar.h"
#include "Type/Font.h"
#include "Type/Bitmap.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class DisplayGraphic

\brief Abstract class supporting graphic displays

This class is derived from \a DisplayChar and adds some basic
methods to draw graphic primitives. \n
The position is given relative to the upper left
corner with coordinates x (=colum) and y (=line).\n
Colors are coded in 16 bit (RGB565):  RRRR.RGGG.GGGB.BBBB\n
  - Bit 15 - 11 red
  - Bit 10 -  6 green
  - Bit  5 -  0 blue

Derived classes should at least:
  - initialize the hardware
  - implement clear() to clear the display and fill it
    with background color
  - implement setArea() and setPixel()

\example HwDisplayGraphic.cpp
*/
class DisplayGraphic : public DisplayChar
{
  protected:
    //---------------------------------------------------------------
    // Initialize hardware and set font type and font zoom size
    // moduleId: Id of this module, used for report
    // width:    Display width in pixel
    // height:   Display height in pixel
    // font:     Character font used in putChar()
    // zoom:     Zoom size of character
    //
    DisplayGraphic( WORD moduleId,
	                  WORD width,
                    WORD height,
                    const Font &font,
                    BYTE zoom = 1 );

  public:
    //---------------------------------------------------------------
    /*! Set a new font type and font zoom size
        \param font Character font used in putChar()
        \param zoom Zoom size of character
    */
    virtual void setFont( const Font &font, BYTE zoom = 1 );

    //---------------------------------------------------------------
    /*! Set font zoom size. The actual font is not changed
        \param zoom Zoom size of character
    */
    virtual void setZoom( BYTE zoom = 1 );

    //---------------------------------------------------------------
    /*! Set background color, which is used in clear() und putChar()
        \param color Background Color (RGB565)
    */
    virtual void setBackColor( WORD color );

    //---------------------------------------------------------------
    /*! Set painting color, which is used in putPixel() and
        putRectangle()
        \param color Paint color (RGB565)
    */
    virtual void setPaintColor( WORD color );

    //---------------------------------------------------------------
    /*! Set text color, which is used in putChar()
        \param color Text color (RGB565)
    */
    virtual void setTextColor( WORD color );

    //---------------------------------------------------------------
    /*! Set position of next character
        \param x Horizontal position in pixel coordinates
        \param y Vertical position in pixel coordinates
    */
    virtual void gotoPixelPos( WORD x, WORD y );

    //---------------------------------------------------------------
    /*! Set position of next character
        \param line Horizontal position in character coordinates
        \param colum Vertical position in character coordinates
    */
    virtual void gotoTextPos( BYTE line, BYTE colum );

    //---------------------------------------------------------------
    /*! Put character at actual position and auto increment column
        for the next character
        \param c Character (ASCII code)
    */
    virtual void putChar( char c );

    //---------------------------------------------------------------
    /*! Paint a pixel in actual painting color
        \param x Horizontal position in pixel coordinates
        \param y Vertical position in pixel coordinates
    */
    virtual void putPixel( WORD x, WORD y );

    //---------------------------------------------------------------
    /*! Paint a filled rectangle in actual painting color
        \param x Horizontal position in pixel coordinates
        \param y Vertical position in pixel coordinates
        \param w Width
        \param h Height
    */
    virtual void putRectangle( WORD x,
                               WORD y,
                               WORD w,
                               WORD h );

    //---------------------------------------------------------------
    /*! Draw a bitmap\n
        Format: 16 bits (RGB565) per pixel
        \param x Horizontal position in pixel coordinates (left)
        \param y Vertical position in pixel coordinates (top)
        \param bitmap Pointer to bitmap data
    */
    virtual void putBitmap( WORD   x,
                            WORD   y,
                            const Bitmap &bitmap );

    //---------------------------------------------------------------
    /*! Refresh the display.
        Some display need a refresh to display new content
    */
    virtual void refresh( void )
    {
    }

    //---------------------------------------------------------------
    /*! Get horizontal display size
        \return Number of pixel
    */
    virtual WORD getWidth( void )
    {
      return( Hsize );
    }

    //---------------------------------------------------------------
    /*! Get vertical display size
        \return Number of pixel
    */
    virtual WORD getHeight( void )
		{
			return( Vsize );
		}

    //---------------------------------------------------------------
    /*! color coding (RGB565): RRRR RGGG GGGB BBBB
        \param red
        \param green
        \param blue
        \return Color code
    */
    static WORD RGB2color( BYTE red, BYTE green, BYTE blue )
    {
      return(    ((blue  & 0xF8) >> 3)  // 5 bit,  0.. 4
               | ((green & 0xFC) << 3)  // 6 bit,  5..10
               | ((red   & 0xF8) << 8)  // 5 bit, 11..15
             );
    }

  private:
    //---------------------------------------------------------------
    // Set painting area. The next calls to setPixel(BYTE color) will
    // draw a pixel in this area with auto increment of horizontal
    // and vertical coordinates
    // x:  Horizontal position in pixel coordinates (left)
    // y:  Vertical position in pixel coordinates (top)
    // w:  Width
    // h:  Height
    //
    virtual void setArea( WORD x,
                          WORD y,
                          WORD w,
                          WORD h ) = 0;

    //---------------------------------------------------------------
    // Draw a pixel at absolute position
    // color:  Pixel color (RGB565)
    // x:      Horizontal position in pixel coordinate
    // y:      Vertical position in pixel coordinate
    //
    virtual void setPixel( WORD color,
                           WORD x,
                           WORD y ) = 0;

    //---------------------------------------------------------------
    // Draw a pixel at relative position within the area defined by
    // setArea(). The horizontal and vertical position is incremented
    // automatically (horizontal, vertical at end of line)
    // color: Pixel color (RGB565)
    //
    virtual void setPixel( WORD color ) = 0;

  protected:
    //---------------------------------------------------------------
    WORD xPos;
    WORD yPos;

	  WORD Hsize;
	  WORD Vsize;

    //---------------------------------------------------------------
    Font font;
    BYTE zoom;

    //---------------------------------------------------------------
    WORD BackColor;
    WORD PaintColor;
    WORD TextColor;

  public:
    //---------------------------------------------------------------
    Std::Report report;

}; //class DisplayGraphic

}  } //namespace

#endif
