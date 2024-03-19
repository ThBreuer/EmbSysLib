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
/*! \param ... 
    \return ...
*/
WORD hton( const WORD in )
{
  return( htons( in )  );
}

//*******************************************************************
/*! \param ... 
    \return ...
*/
DWORD hton( const DWORD in )
{
  return( htonl( in ) );
}

//*******************************************************************
/*! \param ... 
    \return ...
*/
WORD ntoh( const WORD in )
{
  return( ntohs(in) );
}

//*******************************************************************
/*! \param ... 
    \return ...
*/
DWORD ntoh( const DWORD in )
{
  return( ntohl(in) );
}

//EOF
