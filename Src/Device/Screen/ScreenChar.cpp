//*******************************************************************
/*!
\file   ScreenChar.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   01.02.2023

License: See file "LICENSE"
*/

//*******************************************************************
#include <stdio.h>
#include <stdarg.h>

#include "ScreenChar.h"

//*******************************************************************
namespace EmbSysLib {
namespace Dev {

//*******************************************************************
//
// ScreenChar
//
//*******************************************************************
//-------------------------------------------------------------------
ScreenChar::ScreenChar( DisplayChar &display )

: display( display )

{
  maxStrLength = display.getNumberOfColumns();
  str = new char[maxStrLength+1];
  display.clear();
}

//-------------------------------------------------------------------
void ScreenChar::clear(void)
{
  display.clear();
}

//-------------------------------------------------------------------
void ScreenChar::refresh(void)
{
  display.refresh();
}

//-------------------------------------------------------------------
void ScreenChar::printf( BYTE        line,
                         BYTE        column,
                         const char *format,
                         ... )
{
  va_list argzeiger;
  va_start ( argzeiger, format );
  vsnprintf( str, maxStrLength+1, format, argzeiger );
  va_end   ( argzeiger );

  display.gotoTextPos( line, column );
  display.putString( str );
}

} } //namespace

//EOF
