//*******************************************************************
/*!
\file   Font.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   21.03.2012

License: See file "LICENSE"
*/

//*******************************************************************
#include "Font.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// Font
//
//*******************************************************************
//-------------------------------------------------------------------
Font::Font( const Header *header,
            const BYTE   *data )
{
  this->header   = header;
  this->data     = data;
  this->selected = data;  // default is first character
}

//-------------------------------------------------------------------
Font::Font( const BYTE *ptr )
{
  this->header = (Font::Header*)ptr;
  this->data   = (BYTE*)(ptr + sizeof(Font::Header));
}

//-------------------------------------------------------------------
 BYTE Font::getCharWidth( void ) const
{
  return( GET_BYTE_PROGMEM(header->charWidth) );
}

//-------------------------------------------------------------------
BYTE Font::getCharHeight( void ) const
{
  return( GET_BYTE_PROGMEM(header->charHeight) );
}

//-------------------------------------------------------------------
void Font::setChar( BYTE c )
{
  BYTE id = 0;

  if(         GET_BYTE_PROGMEM(header->firstId) <= c
      && c <= GET_BYTE_PROGMEM(header->lastId)        ) // is char c defined?
  {
    id = c - GET_BYTE_PROGMEM(header->firstId);
  }
  selected = data + (WORD)id * GET_BYTE_PROGMEM(header->bytePerChar);
}

//-------------------------------------------------------------------
BYTE Font::getPixel( BYTE x, BYTE y )
{                                                 // calc pixel number
  WORD pixel  = ( (WORD)x + (WORD)y * getCharWidth() );

                                                // get pixel value
  BYTE ret = GET_BYTE_PROGMEM_PTR(selected+(pixel>>3)) & ( 0x80>>(pixel&0x07) );
  pixel++;
  return( ret ? true : false );
}

}  } //namespace

//EOF
