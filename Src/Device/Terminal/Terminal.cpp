//*******************************************************************
/*!
\file   Terminal.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   06.02.2023

License: See file "LICENSE"
*/

//*******************************************************************
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

//*******************************************************************
#include "Terminal.h"

//*******************************************************************
namespace EmbSysLib {
namespace Dev {

//*******************************************************************
//
// Terminal
//
//*******************************************************************
//-------------------------------------------------------------------
Terminal::Terminal( Uart       &uart,
                    WORD        inBufSize,
                    WORD        outBufSize,
                    const char *key,
                    const char *eos )
: uart( uart )

{
  this->inBufSize  = inBufSize;
  this->outBufSize = outBufSize;
  this->key        = key;
  this->eos        = eos;
  strIn            = new char[ inBufSize  + 1 ];
  strOut           = new char[ outBufSize + 1 ];
  idx              = 0;
  escape           = 0;
  last             = 0;
  pending          = 0;
  ctrl             = 0;
  num              = 0;
}

//-------------------------------------------------------------------
char* Terminal::getString( void )
{
  WORD c        = 0;
  BYTE complete = false;

  // new string, if ESC sequence pending
  if( pending )
  {
    strIn[0] = pending >> 8;
    strIn[1] = pending & 0xFF;
    strIn[2] = 0;
    pending = 0;
    return( strIn );
  }

  // read character
  while( (c = get()) && !complete )
  {
    if( c & ESC_SEQUENCE ) // ESC sequence
    {
      if( idx == 0 )
      {
        strIn[idx++] = c >> 8;
        strIn[idx++] = c & 0xFF;
      }
      else
      {
        pending = c;
      }
      complete = true;
    }
    else
    {
      strIn[idx++] = c;

      // check end of string condition
      if(    ( idx >= inBufSize              )   //    buffer full
          || ( idx == 1 &&  strchr( key, c ) )   // OR first character and single key
          || (              strchr( eos, c ) ) ) // OR end of string character
      {
        complete = true;
      }
    }

    if( complete )
    {
      strIn[idx] = 0;
      idx        = 0;
      return( strIn );
    }
  }
  return( NULL );
}

//-------------------------------------------------------------------
WORD Terminal::get()
{
  BYTE c = 0;

  while( uart.get( c ) )
  {
    switch( escape )
    {
      default:
      case 0: // we are not in an escape sequence
        switch( c )
        {
          case 27  : escape = 1; ctrl = 0;break; // start of esc sequence
          default  : last   = c; return( last ); // normal
        }
        break;

      case 1: // last character was an escape key
        switch( c )
        {
          case '[': escape = 2; ctrl |= ESC_SEQUENCE;            break;
          case 'O': escape = 2; ctrl |= ESC_SEQUENCE|SHIFT_CTRL; break;
          case 27:  escape = 1; ctrl |= ALT;                     break;
          default:  escape = 0; break;           // unvalid sequence
        }
        break;

      case 2: // we have received 'O' or '[' after ESC
        if( isNumber( c ) )
        {
          num = (c-0x30);
          escape=3;
        }
        else
        {
          escape = 0;
          return( ctrl | remapESC(c) );
        }
        break;

      case 3:
      case 4:
        if( isNumber( c ) )
        {
          num = 10*num + (c-0x30);
          escape++;
        }
        else
        {
          escape=0;
          return(  ctrl | num );
        }
        break;

      case 5:
        escape=0;
        return(  ctrl | num );

    } //switch
  }
  return( 0 );
}

//-------------------------------------------------------------------
void Terminal::printf( const char *format, ... )
{
  va_list argzeiger;

  va_start ( argzeiger, format );
  vsnprintf( strOut, outBufSize, format, argzeiger );
  va_end   ( argzeiger );

  // Set output
  uart.set( (BYTE*)strOut, strlen(strOut) );
}

} } //namespace

//EOF
