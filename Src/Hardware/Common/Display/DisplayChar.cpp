//*******************************************************************
/*!
\file   DisplayChar.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   05.04.2016

License: See file "LICENSE"
*/

//*******************************************************************
#include "DisplayChar.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// DisplayChar
//
//*******************************************************************
//-------------------------------------------------------------------
DisplayChar::DisplayChar( WORD moduleId,
	                        BYTE numberOfLines,
                          BYTE numberOfColumns )
: report( moduleId ) 
{
  this->numberOfLines   = numberOfLines;
  this->numberOfColumns = numberOfColumns;
}

//-------------------------------------------------------------------
inline BYTE DisplayChar::getNumberOfLines( void )
{
  return( numberOfLines );
}

//-------------------------------------------------------------------
inline BYTE DisplayChar::getNumberOfColumns( void )
{
  return( numberOfColumns );
}

//-------------------------------------------------------------------
void DisplayChar::putString( const char *str )
{
  while( *str != 0 )
  {
    putChar( *str );
    str++;
  }
}

} } //namespace

//EOF
