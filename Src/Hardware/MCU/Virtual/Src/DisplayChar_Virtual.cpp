//*******************************************************************
/*!
\file   DisplayChar_Virtual.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   17.11.2022

License: See file "LICENSE"
*/

//*******************************************************************
#include "DisplayChar_Virtual.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// DisplayChar_Virtual
//
//*******************************************************************
//-------------------------------------------------------------------
DisplayChar_Virtual::DisplayChar_Virtual( WORD        numOfLines,
                                          WORD        numOfColumns,
                                          const char *server )

: DisplayChar( ReportID_Hw::Module::DISPLAY_CHAR_VIRTUAL, numOfLines, numOfColumns )
, client( server )

{
  line  = 0;
  colum = 0;
  clear();
}

//-------------------------------------------------------------------
void DisplayChar_Virtual::clear( void )
{
  data.header.cmd    = Data::CMD_CLEAR;
  data.column        = numberOfColumns;
  data.line          = numberOfLines;

  client.send   ( (BYTE*)&data, sizeof(Data) );
}

//-------------------------------------------------------------------
void DisplayChar_Virtual::refresh( void )
{
}

//---------------------------------------------------------------
void DisplayChar_Virtual::gotoTextPos( BYTE lineIn, BYTE columIn )
{
  line  = lineIn;
  colum = columIn;
}

//---------------------------------------------------------------
void DisplayChar_Virtual::putChar( char c )
{
  if(    line  < numberOfLines
      && colum < numberOfColumns )
  {
    data.header.cmd = Data::CMD_WRITE;
    data.line       = line;
    data.column     = colum;
    data.c          = c;

    client.send   ( (BYTE*)&data, sizeof(Data) );
    client.waitAckn();

    colum++; // emulate cursor increment
  }
}

//*******************************************************************
//
// DisplayChar_Virtual::Handler
//
//*******************************************************************
//-------------------------------------------------------------------
DisplayChar_Virtual::Handler::Handler( UDPserver &server )
: UDPserver::Handler( server )
{
}

//-------------------------------------------------------------------
bool DisplayChar_Virtual::Handler::update( BYTE *data, WORD plen )
{
  BYTE mode = data[0];

  if(    mode == UDP_MsgHeader::DISPLAY_CHAR
      && plen == sizeof(DisplayChar_Virtual::Data) )
  {
    DisplayChar_Virtual::Data *dataPtr = (DisplayChar_Virtual::Data*)data;

    switch( dataPtr->header.cmd )
    {
      case Data::CMD_CLEAR: onClear();                                                 break;
      case Data::CMD_WRITE: onWrite((WORD)dataPtr->column, dataPtr->line, dataPtr->c); break;
    }
    server.sendAckn();
    return( true );
  }
  return( false );
}

}  } //namespace

//EOF
