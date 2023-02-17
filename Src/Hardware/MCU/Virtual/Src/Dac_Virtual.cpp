//*******************************************************************
/*!
\file   Dac_Virtual.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   21.01.2013

License: See file "LICENSE"
*/

//*******************************************************************
#include "Dac_Virtual.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// Dac_Virtual
//
//*******************************************************************
//-------------------------------------------------------------------
Dac_Virtual::Dac_Virtual( const char *server )

: Dac( ReportID_Hw::Module::DAC_VIRTUAL ),
  client( server )

{
}

//-------------------------------------------------------------------
inline void Dac_Virtual::set( BYTE ch,
                              WORD value )
{
  data.header.cmd = 0;
  data.ch         = ch;
  data.value      = value;
  client.send((BYTE*)&data, sizeof(Data));
  client.waitAckn();
}

//*******************************************************************
//
// Dac_Virtual::Handler
//
//*******************************************************************
//-------------------------------------------------------------------
Dac_Virtual::Handler::Handler( UDPserver &server )
: UDPserver::Handler( server )
{
}

//-----------------------------------------------------------
bool Dac_Virtual::Handler::update( BYTE *data, WORD plen )
{
  BYTE mode = data[0];

  if(   mode == UDP_MsgHeader::DAC
     && plen == sizeof(Dac_Virtual::Data) )
  {
    Dac_Virtual::Data *dataPtr = (Dac_Virtual::Data*)data;
    onSetValue( dataPtr->ch, dataPtr->value );
    server.sendAckn();
    return( true );
  }
  return( false );
}

}  } //namespace

//EOF
