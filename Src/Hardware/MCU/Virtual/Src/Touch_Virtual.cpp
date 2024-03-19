//*******************************************************************
/*!
\file   Touch_Virtual.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   11.03.2013

License: See file "LICENSE"
*/

//*******************************************************************
#include "Touch_Virtual.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// Touch_Virtual
//
//*******************************************************************
//-------------------------------------------------------------------
Touch_Virtual::Touch_Virtual( const char *server,
                              WORD        width,
                              WORD        height )
: Touch( ReportID_Hw::Module::TOUCH_VIRTUAL, width, height, ROTATION_0 )
, client( server )

{
}

//-------------------------------------------------------------------
void Touch_Virtual::update( void )
{
  client.send   ( (BYTE*)&data, sizeof(Data) );
  client.receive( (BYTE*)&data, sizeof(Data) );

  if( data.isTouched && data.x < width && data.y < height )
  {
    xPos          = data.x;
    yPos          = data.y;
    isTouchedFlag = true;
  }
  else
  {
    xPos          = 0;
    yPos          = 0;
    isTouchedFlag = false;
  }
}

//*******************************************************************
//
// Touch_Virtual::Handler
//
//*******************************************************************
//-------------------------------------------------------------------
Touch_Virtual::Handler::Handler( UDPserver &server )
: UDPserver::Handler( server )
{
}

//-----------------------------------------------------------
bool Touch_Virtual::Handler::update( BYTE *data, WORD plen )
{
  BYTE mode = data[0];

  if( mode == UDP_MsgHeader::TOUCH )
  {
    Touch_Virtual::Data *dataPtr = (Touch_Virtual::Data*)data;

    Touch_Virtual::Handler::Data h = onGetTouch();
    dataPtr->x         = h.x;
    dataPtr->y         = h.y;
    dataPtr->isTouched = h.isTouched;
    server.send( (BYTE*)dataPtr,sizeof(Touch_Virtual::Data) );

    return true;
  }
  return false;
}

}  } //namespace

//EOF
