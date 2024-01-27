//*******************************************************************
/*!
\file   Crc.cpp
\author Thomas Breuer
\date   18.12.2023

License: See file "LICENSE"
*/

//*******************************************************************
#include "Crc.h"

//*******************************************************************
namespace EmbSysLib {
namespace Std {

//*******************************************************************
//
// Crc
//
//*******************************************************************
//-------------------------------------------------------------------
Crc::Crc( MODE mode,
          WORD startCRC_In,
          WORD generatorPolynomIn )
{
  startCRC         = startCRC_In;
  crc              = startCRC;
  generatorPolynom = generatorPolynomIn;

  if( mode == FAST )
  {
    tabArray = new WORD[256];

    for( WORD i = 0; i < 256 && tabArray; i++ )
    {
      tabArray[i] = tabMethod( i );
    }
  }
  else
  {
    tabArray = NULL;
  }
}

//-------------------------------------------------------------------
Crc::~Crc( void )
{
  if( tabArray )
  {
    delete tabArray;
  }
}

//-------------------------------------------------------------------
void Crc::operator()( const BYTE  *ptr,
                      const DWORD  size )
{
  for( DWORD i = 0; i < size; i++ )
  {
    BYTE idx = ( *(ptr++) ) ^ crc;

    if( tabArray )
    {
      crc = (crc>>8) ^ tabArray[idx];
    }
    else
    {
      crc = (crc>>8) ^ tabMethod( idx );
    }
  }
}

//-------------------------------------------------------------------
void Crc::operator()( const BYTE data )
{
  BYTE idx = data ^ crc;

  if( tabArray )
  {
    crc = (crc>>8) ^ tabArray[idx];
  }
  else
  {
    crc = (crc>>8) ^ tabMethod( idx );
  }
}

//-------------------------------------------------------------------
inline WORD Crc::tabMethod( BYTE idx )
{
  WORD crcLoc = 0;

  for( BYTE bit = 0; bit < 8; bit++ )
  {
    if( (crcLoc^idx) & 0x01 )
    {
      crcLoc = (crcLoc>>1) ^ generatorPolynom;
    }
    else
    {
      crcLoc = (crcLoc>>1);
    }
    idx = (idx>>1);
  }
  return( crcLoc );
}

}  } //namespace

// EOF
