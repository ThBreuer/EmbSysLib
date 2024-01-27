//*******************************************************************
/*!
\file   SharedMem.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   19.12.2023

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _STD_SHARED_MEM_H
#define _STD_SHARED_MEM_H

//*******************************************************************
#include <string.h>

#include "Hardware/MCU/System.h"

//*******************************************************************
namespace EmbSysLib {
namespace Std {

//*******************************************************************
/*!
\class SharedMem

\brief Shared memory, locked memory read and write.
The shared memory can be used for inter task communication

\example StdSharedMem.cpp
*/
template <class T> class SharedMem
{
  public:
    //---------------------------------------------------------------
    /*! Create a shared memory. The memory is zero initialized
    */
    SharedMem( void )
    {
      memset( &speicher, 0, sizeof(T) );
    }

    //---------------------------------------------------------------
    /*! Write an object to the shared memory
        \param ref Reference to an object
    */
    void operator<<( const T &ref )
    {
      Hw::System::disableInterrupt();
      speicher = ref;
      Hw::System::enableInterrupt();
    }

    //---------------------------------------------------------------
    /*! Read from shared memory
        \param ref Reference to an object
    */
    void operator>>( T &ref )
    {
      Hw::System::disableInterrupt();
      ref = speicher;
      Hw::System::enableInterrupt();
    }

  private:
    //---------------------------------------------------------------
    T speicher;

}; //class SharedMem

}  } //namespace

#endif
