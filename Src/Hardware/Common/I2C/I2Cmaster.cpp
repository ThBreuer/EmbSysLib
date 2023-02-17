//*******************************************************************
/*!
\file   I2Cmaster.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"
*/

//*******************************************************************
#include "I2Cmaster.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// I2Cmaster
//
//*******************************************************************
//-------------------------------------------------------------------
I2Cmaster::I2Cmaster( WORD moduleId )
: report( moduleId ) 
{
  isHardwareOK = true;
}

//-------------------------------------------------------------------
BYTE I2Cmaster::write( BYTE  hwAddr,
                       BYTE *adr,
                       BYTE  sizeAdr,
                       BYTE *data,
                       BYTE  sizeData )
{
  BYTE i = 0;

  isHardwareOK = true;

  if( sizeAdr || sizeData )
  {
    start( sizeAdr + sizeData );
    sendAddr( hwAddr ); // master write
    for( i=0; i< sizeAdr; i++ )
    {
       writeByte( adr[i] );
    }
    for( i=0; i< sizeData; i++ )
    {
       writeByte( data[i] );
    }
  }
  stop();
  return( !isHardwareOK );
}

//-------------------------------------------------------------------
BYTE I2Cmaster::read( BYTE  hwAddr,
                      BYTE *adr,
                      BYTE  sizeAdr,
                      BYTE *data,
                      BYTE  sizeData )
{
  BYTE i = 0;

  isHardwareOK = true;

  if( sizeAdr )
  {
    start( sizeAdr );
    sendAddr( hwAddr ); // master write
    for( i=0; i< sizeAdr; i++ )
    {
      writeByte( adr[i] );
    }
  }
  if( sizeData )
  {
    start(sizeData);
    sendAddr( hwAddr | 0x01 ); // master read
    for( i=0; i < sizeData - 1; i++ )
    {
      data[i] = readByteAck();
    }
    data[i] = readByteNack();
  }
  stop();
  return( !isHardwareOK );
}

} } //namespace

//EOF
