//*******************************************************************
/*!
\file   Bitmap.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   25.11.2021

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_BITMAP_H
#define _HW_BITMAP_H

//*******************************************************************
#include "Mcu_Types.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class Bitmap

\brief Handle bitmaps

This Bitmap is a 2-dimensional array of WORDs, usually used to
store images. It can have different sizes (width x height) and can be stored in RAM, ROM or Flash.

The bitmap object can be instantiated by the template class Bitmap::Data and converted to a Bitmap. The tool \b Bmp2Cpp.exe can be used to generate code for a Bitmap::Data object.


\code
  Bitmap::Data<320,240> bmpRAM = {{320,240,16},{0,...}};

  Bitmap bmp1(bmpRAM);
\endcode

Alternatively, the Bitmap constructor can be passed a pointer to a memory (e.g. flash) that contains a Bitmap::Data.

\code
  Bitmap bmp2((WORD*)0x08080000);
\endcode

The application can read the Bitmap using getWidth(), getHeight(), and getPixel(). Write access is not provided.


\see DisplayGraphic
\example HwDisplayGraphic.cpp
*/
class Bitmap
{
  public:
    //*******************************************************************
    /*!
    \class Header
    \brief POD covering bitmap header

    \todo The implementation ignores 'bitsPerPixel', only bitmaps with 16 bits per pixel are supported

    */
    class Header
    {
      public:
        //---------------------------------------------------------------
        WORD width;        //!< Image width
        WORD height;       //!< Image height
        WORD bitsPerPixel; //!< Number of bits per pixel (should be equal to 1, 4, 8 or 16)
    }; //Header

    //*******************************************************************
    /*!
    \class Data
    \brief POD covering bitmap header and data
    \remark Use Bitmap to access the bitmap
    */
    template <WORD WIDTH, WORD HEIGHT> class Data
    {
      public:
        //---------------------------------------------------------------
        Header header;
        WORD   data[WIDTH][HEIGHT];

        //---------------------------------------------------------------
        /*! Convert header and data
        */
        operator Bitmap( void ) const
        {
          return( Bitmap( &header, (WORD*)data ) );
        }
    }; //Data

  private:
    //---------------------------------------------------------------
    // Instantiate a Bitmap object
    // Use with Bitmap::Data only
    // header: Pointer to bitmap header
    // data:   Pointer to bitmap data
    //
     Bitmap( const Header *header,
             const WORD   *data );

  public:
    //---------------------------------------------------------------
    /*! Instantiate a Bitmap object
        \param ptr Pointer to bitmap memory
    */
     Bitmap( const BYTE *ptr );

    //---------------------------------------------------------------
    /*! Get the image width
        \return Width in pixel
    */
    WORD getWidth( void ) const;

    //---------------------------------------------------------------
    /*! Get the image height
        \return Height in pixel
    */
    WORD getHeight( void ) const;

    //---------------------------------------------------------------
    /*! Get pixel color
        \param x Horizontal position within character bitmap
        \param y Vertical position within character bitmap
        \return color
    */
    WORD getPixel( WORD x, WORD y ) const;

  private:
    //---------------------------------------------------------------
    const Header *header;
    const WORD   *data;

}; //Bitmap

} } //namespace

#endif
