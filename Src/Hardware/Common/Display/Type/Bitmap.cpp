//*******************************************************************
/*!
\file   Bitmap.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   25.11.2021

License: See file "LICENSE"
*/

//*******************************************************************
#include "Bitmap.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// Bitmap
//
//*******************************************************************
//-------------------------------------------------------------------
Bitmap::Bitmap( const Header *header,
                const WORD   *data )
{
  this->header = header;
  this->data   = data;
}

//-------------------------------------------------------------------
Bitmap::Bitmap( const BYTE *ptr )
{
  this->header = (Bitmap::Header*)ptr;
  this->data   = (WORD*)(ptr + sizeof(Bitmap::Header));
}

//-------------------------------------------------------------------
WORD Bitmap::getWidth( void ) const
{
  return( header->width );
}

//-------------------------------------------------------------------
WORD Bitmap::getHeight( void ) const
{
  return( header->height );
}

//-------------------------------------------------------------------
WORD Bitmap::getPixel( WORD x, WORD y ) const
{
  DWORD pixel  = (DWORD)x + (DWORD)y * getWidth();

  return( data[pixel] );
}

} } //namespace

//EOF
