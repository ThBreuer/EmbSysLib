//*******************************************************************
/*!
\file   ExtInt.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   31.01.2024

License: See file "LICENSE"
*/

//*******************************************************************
#include "ExtInt.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// ExtInt
//
//*******************************************************************
//-------------------------------------------------------------------
  void ExtInt::add( ExtInt::Task *ptr )
{
  isrSeq.add( ptr );
}

//-------------------------------------------------------------------
inline void ExtInt::isrHandler( void )
{
  Task *ptr = isrSeq.getFirst();
  while( ptr )
  {
    ptr->update();
    ptr = ptr->getNext();
  }
}

}  } //namespace

// EOF
