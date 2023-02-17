//*******************************************************************
/*!
\file   Font.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   21.03.2012

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_FONT_H
#define _HW_FONT_H

//*******************************************************************
#include "Mcu_Types.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class Font

\brief Font manager

This class manages the display font. Supports bitmap fonts only.
The font is defined by Font::Header and Font::Data. The Font::Header 
describes the content of the Font::Data, which is an array of glyphs.
Each glyph is a bitmap with 1 bit per pixel.

The font object can be instantiated by the template class Font::Data and converted to a Font. The tool \b Bmp2Font.exe can be used to generate code for a Font::Data object.

\code
  Font::Data<96,25> font10x20 = {{32,127,25,10,20},{{0,...},{},...}};

  Font f = (Font)font10x20;
\endcode

\see DisplayGraphic
\example HwDisplayGraphic.cpp
*/
class Font
{
  public:
    //*******************************************************************
    /*!
    \class Header
    \brief POD covering font header
    */
    class Header
    {
      public:
        //---------------------------------------------------------------
        BYTE firstId;      //!< First ASCII code of the font
        BYTE lastId;       //!< Last ASCII code of the font
        BYTE bytePerChar;  //!< Number of memory bytes per character
        BYTE charWidth;    //!< Width of a character in pixel
        BYTE charHeight;   //!< Height of a character in pixel
    }; //Header

    //*******************************************************************
    /*!
    \class Data
    \brief POD covering font header and data
    \remark Use Font to access the font
    */
    template <WORD NUM_OF_CHAR, BYTE BYTE_PER_CHAR> class Data
    {
      public:
        //---------------------------------------------------------------
        Header header;
        BYTE   data[NUM_OF_CHAR][BYTE_PER_CHAR];

        //---------------------------------------------------------------
        /*! Convert header and data
         */
        operator Font( void ) const
        {
          return( Font( &header, (BYTE*)data ) );
        }
    }; //Data

  private:
    //---------------------------------------------------------------
    // Instantiate a Font object
    // Use with Font::Data only
    // header: Pointer to font header
    // data:   Pointer to font data
    //
    Font( const Header *header,
          const BYTE   *data );

  public:
    //---------------------------------------------------------------
    /*! Instantiate a Font object
        \param ptr Pointer to font memory
    */
     Font( const BYTE *ptr );

    //---------------------------------------------------------------
    /*! Get the width of a character
        \return Character width in pixel
     */
    BYTE getCharWidth( void ) const;

    //---------------------------------------------------------------
    /*! Get the height of a character
        \return Character height in pixel
     */
    BYTE getCharHeight( void ) const;

    //---------------------------------------------------------------
    /*! Set the ASCII code of the character, which is handled in the
        next call of getPixel()
        \param c  Character code
     */
    void setChar( BYTE c );

    //---------------------------------------------------------------
    /*! Check, if the pixel at the given Position is set in the
        selected character
        \see setChar()
        \param x  Horizontal position within character bitmap
        \param y  Vertical position within character bitmap
        \return
                - true: Pixel is set
                - false Pixel is not set
     */
    BYTE getPixel( BYTE x, BYTE y );

  private:
    //---------------------------------------------------------------
    const Header *header;
    const BYTE   *data;
    const BYTE   *selected;

}; //Font

}  } //namespace

#endif
