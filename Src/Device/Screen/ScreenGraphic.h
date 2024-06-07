//*******************************************************************
/*!
\file   ScreenGraphic.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   01.02.2023

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _DEV_SCREEN_GRAPHIC_H
#define _DEV_SCREEN_GRAPHIC_H

//*******************************************************************
#include "Hardware/Common/Display/DisplayGraphic.h"

//*******************************************************************
namespace EmbSysLib {
namespace Dev {

//*******************************************************************
/*!
\class ScreenGraphic

\brief Graphic Screen

The screen coordinates are zero in the upper left corner.
- 'x': left to right
- 'y': top to bottom

\example DevScreenGraphic.cpp
*/
class ScreenGraphic
{
  public:
    //---------------------------------------------------------------
    /*! Initialize Screen

        Using DisplayGraphic hardware
        \param display //!< Reference to a DisplayGraphic object
    */
    ScreenGraphic( DisplayGraphic &display );

    //---------------------------------------------------------------
    /*! Clear Screen
    */
    virtual void clear( void );

    //---------------------------------------------------------------
    /*! Refresh Screen
    */
    virtual void refresh( void );

    //---------------------------------------------------------------
    /*! Print a formated text string

        The string is printed on position (\a line, \a column).
        Beginning with the \b format parameter, this method is
        equal to the correspondent ANSI-C function
        \param line   Line number
        \param column Column number of first character
        \param format Formated string to print
    */
    virtual void printf( BYTE        line,
                         BYTE        column,
                         const char *format,
                         ...);

    //---------------------------------------------------------------
    /*! Set a new font and zoom factor
         \param font Font object
         \param zoom Font zoom factor (default: 1)
    */
    virtual void setFont( const Font &font,
                          BYTE zoom = 1 );

    //---------------------------------------------------------------
    /*! Set a new zoom factor
        \param zoom Font zoom factor (default: 1)
    */
    virtual void setZoom( BYTE zoom = 1 );

    //---------------------------------------------------------------
    /*! Set background color
        \param color RGB color
    */
    virtual void setBackColor( WORD color );

    //---------------------------------------------------------------
    /*! Set Text color
        \param color RGB color
    */
    virtual void setTextColor( WORD color );
    //---------------------------------------------------------------
    /*! Draw a formated text string

        The string is printed on position (\a x, \a y)
        Beginning with the \b format parameter, this method is
        equal to the ANSI-C function 'printf'
        \param x      X-position
        \param y      Y-position
        \param format Format string as defined in ANSI-C
    */
    virtual void drawText( WORD        x,
                           WORD        y,
                           const char *format,
                           ... );

    //---------------------------------------------------------------
    /*! Draw a single pixel
        \param x     X-position
        \param y     Y-position
        \param color RGB color
    */
    virtual void drawPixel( WORD x,
                            WORD y,
                            WORD color );

    //---------------------------------------------------------------
    /*! Draw a filled rectangle
        \param x     X-position
        \param y     Y-position
        \param w     Width
        \param h     Height
        \param color RGB color
    */
    virtual void drawRectangle( WORD x,
                                WORD y,
                                WORD w,
                                WORD h,
                                WORD color );

    //---------------------------------------------------------------
    /*! Draw an open rectangle (frame)
        \param x     X-position
        \param y     Y-position
        \param w     Width
        \param h     Height
        \param th    Thickness
        \param color RGB color
    */
    virtual void drawRectangle( WORD x,
                                WORD y,
                                WORD w,
                                WORD h,
                                WORD th,
                                WORD color );

    //---------------------------------------------------------------
    /*! Draw an open circle
        \param x     X-position
        \param y     Y-position
        \param r     Radius
        \param th    Thickness
        \param color RGB color
    */
    virtual void drawCircle( WORD x,
                             WORD y,
                             WORD r,
                             WORD th,
                             WORD color );

    //---------------------------------------------------------------
    /*! Draw an filled circle
        \param x     X-position
        \param y     Y-position
        \param r     Radius
        \param color RGB color
    */
   virtual void drawCircle( WORD x,
                            WORD y,
                            WORD r,
                            WORD color );

    //---------------------------------------------------------------
    /*! Draw a line
        \param x0    start X-position
        \param y0    start Y-position
        \param x1    end X-position
        \param x1    end Y-position
        \param th    Thickness
        \param color RGB color
   */
    virtual void drawLine( WORD x0,
                           WORD y0,
                           WORD x1,
                           WORD y1,
                           WORD th,
                           WORD color );

    //---------------------------------------------------------------
    /*! Draw a bitmap picture

        Format: 16 bits per pixel
        \param x     X-position
        \param y     Y-position
        \param &bitmap reference to a Bitmap
    */
    virtual void drawBitmap( WORD        x,
                             WORD        y,
                             const Bitmap &bitmap );

    //---------------------------------------------------------------
    // Get Screen width
    // \return width
    WORD getWidth( void )
    {
      return( display.getWidth() );
    }

    //---------------------------------------------------------------
    // Get Screen height
    // \return height
    WORD getHeight( void )
    {
      return( display.getHeight() );
    }

  private:
    //---------------------------------------------------------------
    DisplayGraphic &display;
    BYTE            maxStrLength;
    char           *str;

}; //class ScreenGraphic

} } //namespace

#endif
