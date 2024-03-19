//*******************************************************************
/*!
\file   Std.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"

\brief Standard include file
*/

//*******************************************************************
#ifndef _STD_H
#define _STD_H

//*******************************************************************
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//*******************************************************************
namespace EmbSysLib {
namespace Std {

//*******************************************************************
/*! Compares two values and returns the maximum
    \param  x,y
    \return maximum of (x,y)
*/
template <class T> inline T MAX( T x, T y )
{
  if( x > y )
    return( x );
  else
    return( y );
}

//*******************************************************************
/*! Compares two values and returns the minimum
    \param  x,y
    \return minimum of (x,y)
*/
template <class T> inline T MIN( T x, T y )
{
  if( x < y )
    return( x );
  else
    return( y );
}

//*******************************************************************
/*! Compares input value x with upper and lower limit
    and returns limited value
    \param x input value
    \param min lower limit
    \param max upper limit
    \return
            - input value x, if min < x < max
            - min, if x < min
            - max, if x > max
*/
template <class T> inline T RANGE( T x, T min, T max )
{
  if( x > max )
    return( max );
  if( x < min )
    return( min );
  return( x );
}

//*******************************************************************
/*! Get sign of a value
    \param  x
    \return +1, if x >= 0\n
            -1, if x < 0
*/
template <class T> inline T SIGN( T x )
{
  if( x >= 0 )
    return( +1 );
  else
    return( -1 );
}

//*******************************************************************
//! a well known nature constant ...
const float PI = (float)3.1415926535897932384626433832795;

// ...
static const WORD DEFAUT_STRING_LEN = 20;

}  } //namespace

#endif
