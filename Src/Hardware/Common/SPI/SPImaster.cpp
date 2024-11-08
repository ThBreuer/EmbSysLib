//*******************************************************************
/*!
\file   SPImaster.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   20.03.2023

License: See file "LICENSE"
*/

//*******************************************************************
#include "SPImaster.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// cHwSPImaster
//
//*******************************************************************
//-------------------------------------------------------------------
SPImaster::SPImaster( void )
{
  // nothing to do ...
}

//-------------------------------------------------------------------
BYTE SPImaster::Device::transceiveByte( BYTE data )
{
  return( spi.transceiveByte( data ) );
}


//-------------------------------------------------------------------
BYTE SPImaster::Device::transceive( BYTE data )
{
  start();
  BYTE ret = spi.transceiveByte( data );
  stop();
  return(ret);
}

//-------------------------------------------------------------------
void SPImaster::Device::transceive( BYTE *data,
                                    WORD  size )
{
  start();
  for( WORD i = 0; i < size; i++ )
  {
    data[i] = spi.transceiveByte(data[i]);
  }
  stop();
}

//-------------------------------------------------------------------
inline BYTE SPImaster::Device::read( void )
{
  return( transceive() );
}

//-------------------------------------------------------------------
template <class T>
void SPImaster::Device::read( T *data )
{
  start();
  for( WORD i = 0; i < sizeof(T); i++ )
  {
    ((BYTE *)data)[i] = spi.transceiveByte();
  }
  stop();
}

//-------------------------------------------------------------------
void SPImaster::Device::write( BYTE data )
{
  transceive( data );
}

//-------------------------------------------------------------------
void SPImaster::Device::write( WORD data )
{
  start();
  spi.transceiveByte( data >> 8    );
  spi.transceiveByte( data &  0xFF );
  stop();
}

//-------------------------------------------------------------------
template <class T>
inline void SPImaster::Device::write( T *data )
{
  start();
  for( WORD i = 0; i < sizeof(T); i++ )
  {
    spi.transceiveByte( ((BYTE*)data)[i] );
  }
  stop();
}

} } //namespace

//EOF
