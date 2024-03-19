//*******************************************************************
/*!
\file   Mcu_Types.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   02.02.2024

License: See file "LICENSE"

\brief ...

*/

//*******************************************************************
#include "Mcu.h"

//*******************************************************************
WORD hton( const WORD in )
{
  return(  (((in>>8)&0xFF)   )
         | (((in   )&0xFF)<<8) );
}

//*******************************************************************
DWORD hton( const DWORD in )
{
  return( (((in>>24)&0xFF)    )
         |(((in>>16)&0xFF)<< 8)
         |(((in>> 8)&0xFF)<<16)
         |(((in    )&0xFF)<<24) );
}

//*******************************************************************
WORD ntoh( const WORD in )
{
  return( hton(in) );
}

//*******************************************************************
DWORD ntoh( const DWORD in )
{
  return( hton(in) );
}

//EOF
