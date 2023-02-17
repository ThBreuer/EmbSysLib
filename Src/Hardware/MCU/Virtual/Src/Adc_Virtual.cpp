//*******************************************************************
/*!
\file   Adc_Virtual.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   15.10.2013

License: See file "LICENSE"
*/

//*******************************************************************
#include "Adc_Virtual.h"
#include "Hardware/ReportID_Hw.h"


//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// Adc_Virtual
//
//*******************************************************************
//-------------------------------------------------------------------
Adc_Virtual::Adc_Virtual( const char *server,
                          Timer      &timer )

: Adc( ReportID_Hw::Module::ADC_VIRTUAL, 255, timer )
, client( server )
{
}

//-------------------------------------------------------------------
void Adc_Virtual::configCh( BYTE ch, BYTE para )
{
}

//-------------------------------------------------------------------
inline WORD Adc_Virtual::getResult( void )
{
  return( currentValue );
}

//-------------------------------------------------------------------
inline void Adc_Virtual::startCh( BYTE ch )
{
  data.header.cmd = 0;
  data.ch         = ch;

  client.send((BYTE*)&data, sizeof(Data));
  if( client.receive((BYTE*)&data, sizeof(Data)))
  {
    currentValue = (WORD)data.value;
  }
  else
  {
    currentValue = 0;
  }

  // simulate ADC interrupt:
  isr();
}

//*******************************************************************
//
// Adc_Virtual::Handler
//
//*******************************************************************
//-------------------------------------------------------------------
Adc_Virtual::Handler::Handler( UDPserver &server )
: UDPserver::Handler( server )
{
}

//-----------------------------------------------------------
bool Adc_Virtual::Handler::update( BYTE *data, WORD plen )
{
  BYTE mode = data[0];

  if( mode == UDP_MsgHeader::ADC )
  {
    Adc_Virtual::Data *dataPtr = (Adc_Virtual::Data*)data;
    dataPtr->value = onGetValue( dataPtr->ch );
    server.send( (BYTE*)dataPtr,sizeof(Adc_Virtual::Data) );
    return true;
  }
  return false;
}

}  } //namespace

//EOF
