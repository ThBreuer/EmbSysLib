//*******************************************************************
/*!
\file   Port_Virtual.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   10.11.2016

License: See file "LICENSE"
*/

//*******************************************************************
#include "Port_Virtual.h"
#include "Sys/UDP_Types.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// Port_Virtual
//
//*******************************************************************
//-------------------------------------------------------------------
Port_Virtual::Port_Virtual( const char *server )
: Port( ReportID_Hw::Module::PORT_VIRTUAL )
, client( server )
{
  valueDir = 0;
  valueOut = 0;
  valueIn  = 0;
}

//-------------------------------------------------------------------
void Port_Virtual::setMode( MTYPE mask, Mode mode )
{
  if( mode & Out )
  {
    valueDir |= mask;
  }
  else
  {
    valueDir &= ~mask;
  }

  transceive();
}

//-------------------------------------------------------------------
void Port_Virtual::setPinMode( BYTE pinId, Mode modeIn )
{
  setMode( 1<< pinId, modeIn );
}

//-------------------------------------------------------------------
void Port_Virtual::set( MTYPE mask )
{
  valueOut |= mask;

  transceive();
}

//-------------------------------------------------------------------
void Port_Virtual::set( MTYPE mask, MTYPE value )
{
  set( mask & value );
  clr( mask & (~value) );
}

//-------------------------------------------------------------------
void Port_Virtual::clr( MTYPE mask )
{
  valueOut &= ~mask;

  transceive();
}

//-------------------------------------------------------------------
MTYPE Port_Virtual::get( void )
{
  transceive();
  return( valueIn );
}

//-------------------------------------------------------------------
void Port_Virtual::transceive( )
{
  System::disableInterrupt();
  data.dir   = valueDir;
  data.value = valueOut;

  client.send( (BYTE*)&data, sizeof(Data) );
  if( client.receive((BYTE*)&data, sizeof(Data)) ==  sizeof(Data) )
  {
    valueIn = data.value;
  }
  System::enableInterrupt();
}

//*******************************************************************
//
// Port_Virtual::Handler
//
//*******************************************************************
//-------------------------------------------------------------------
Port_Virtual::Handler::Handler( UDPserver &server )
: UDPserver::Handler( server )
{
}

//-----------------------------------------------------------
bool Port_Virtual::Handler::update( BYTE *data, WORD plen )
{
  BYTE mode = data[0];

  if( mode == UDP_MsgHeader::PORT )
  {
    Port_Virtual::Data *dataPtr = (Port_Virtual::Data*)data;
    dataPtr->value = onValueRequest( dataPtr->dir, dataPtr->value );
    server.send( (BYTE*)dataPtr,sizeof(Data) );
    return( true );
  }
  return( false );
}

}  } //namespace

//EOF
