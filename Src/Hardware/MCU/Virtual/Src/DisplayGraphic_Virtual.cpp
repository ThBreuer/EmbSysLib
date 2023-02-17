//*******************************************************************
/*!
\file   DisplayGraphic_Virtual.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   14.02.2012

License: See file "LICENSE"
*/

//*******************************************************************
#include "DisplayGraphic_Virtual.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// DisplayGraphic_Virtual
//
//*******************************************************************
//-------------------------------------------------------------------
DisplayGraphic_Virtual::DisplayGraphic_Virtual( WORD        width,
                                                WORD        height,
                                                const char *server,
                                                Font      fontIn,
                                                BYTE        zoomIn )

: DisplayGraphic( ReportID_Hw::Module::DISPLAY_GRAPHIC_VIRTUAL,
                  width, height, fontIn, zoomIn )
, client(server)

{
  clear();
}

//-------------------------------------------------------------------
void DisplayGraphic_Virtual::clear( void )
{
  transceive( Data::CMD_CLEAR, Hsize, Vsize, BackColor );
}

//---------------------------------------------------------------
void DisplayGraphic_Virtual::setPixel( WORD color,
                                       WORD x,
                                       WORD y )
{
  transceive( Data::CMD_PIXEL, x, y, color );
}

//---------------------------------------------------------------
void DisplayGraphic_Virtual::setPixel( WORD color )
{
  if( areaIndex < AreaData::maxNumOfPixel)
  {
    area.color[areaIndex] = color;
    areaIndex++;
    areaCount++;
  }
  if( areaIndex >= AreaData::maxNumOfPixel || areaCount >= areaSize)
  {
    area.header.cmd = Data::CMD_RELATIVE;
    area.len = areaIndex;

    client.send   ( (BYTE*)&area, (areaIndex+3)*sizeof(UDP_WORD)+sizeof(UDP_MsgHeader) );
    client.waitAckn();

    areaIndex = 0;
  }
}

//---------------------------------------------------------------
void DisplayGraphic_Virtual::setArea( WORD x,
                                      WORD y,
                                      WORD w,
                                      WORD h )
{
  area.len = 0;
  areaCount = 0;
  areaSize = (DWORD)w*h;
  areaIndex = 0;

  transceive( Data::CMD_AREA, x, y, w, h );
}

//---------------------------------------------------------------
void DisplayGraphic_Virtual::putRectangle( WORD x,
                                           WORD y,
                                           WORD w,
                                           WORD h )
{
  transceive( Data::CMD_FILL_AREA, x, y, w, h, PaintColor );
}

//-------------------------------------------------------------------
void DisplayGraphic_Virtual::transceive( BYTE cmd,
                                         WORD x,
                                         WORD y,
                                         WORD c,
                                         WORD d,
                                         WORD color )
{
  data.header.cmd = cmd;
  data.x          = x;
  data.y          = y;
  data.c          = c;
  data.d          = d;
  data.color      = color;

  client.send( (BYTE*)&data, sizeof(Data) );
  client.waitAckn();
}

//*******************************************************************
//
// DisplayGraphic_Virtual::Handler
//
//*******************************************************************
//-------------------------------------------------------------------
DisplayGraphic_Virtual::Handler::Handler( UDPserver &server )
: UDPserver::Handler( server )
{
}

//-------------------------------------------------------------------
bool DisplayGraphic_Virtual::Handler::update( BYTE *data, WORD plen )
{
  BYTE mode = data[0];

  if(    mode == UDP_MsgHeader::DISPLAY_GRAPHIC
      && plen >= sizeof(DisplayGraphic_Virtual::Data) )
  {
    DisplayGraphic_Virtual::Data *dataPtr = (DisplayGraphic_Virtual::Data*)data;
    switch( dataPtr->header.cmd )
    {
      case Data::CMD_CLEAR:  onClear( dataPtr->c );  break;
      case Data::CMD_PIXEL:  onSetPixel( dataPtr->c, dataPtr->x, dataPtr->y );  break;
      case Data::CMD_FILL_AREA:onSetRectangle( dataPtr->x, dataPtr->y, dataPtr->c, dataPtr->d, dataPtr->color); break;
      case Data::CMD_AREA:   onSetArea( dataPtr->x, dataPtr->y, dataPtr->c, dataPtr->d);    break;
      case Data::CMD_RELATIVE:
        if( plen >= 16 ) // todo: sizeof(DisplayGraphic_Virtual::Data))
        {
          DisplayGraphic_Virtual::AreaData *dataPtr = (DisplayGraphic_Virtual::AreaData*)data;
          WORD len = dataPtr->len;
          for( WORD i=0 ;i<len ;i++)
          {
            onSetPixel( dataPtr->color[i] );
          }
        }
        break;
    }
    server.sendAckn();
    return( true );
  }
  return( false );
}


}  } //namespace

//EOF
