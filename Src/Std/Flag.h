//*******************************************************************
/*!
\file   Flag.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   21.02.2022

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _STD_FLAG_H
#define _STD_FLAG_H

//*******************************************************************
#include "Hardware/MCU/System.h"

//*******************************************************************
namespace EmbSysLib {
namespace Std {

//*******************************************************************
/*!
\class Flag

\brief Supports synchronization of different software components (similar to semaphore)

\example StdFlag.cpp
*/
template <class T> class Flag
{
  public:
    //---------------------------------------------------------------
    /*! Create a Flag
        \param def Initial value of the Flag (optional, otherwise value = 0)
    */
    Flag( T def = (T)0 )
    {
      value = def;
      prev  = (T)0;
    }

    //---------------------------------------------------------------
    /*! Set the Flag
        \param value New value of Flag
    */
    void set( const T &value )
    {
      this->value = value;
    }

  public:
    //---------------------------------------------------------------
    /*! Get the Flag without changing
        \return Current value of Flag
    */
    T get( void )
    {
      return( this->value );
    }

    //---------------------------------------------------------------
    /*! Get and \b reset the Flag: Indicating a single event
        \return Last value of Flag
    */
    T getEvent( void )
    {
      Hw::System::disableInterrupt();
      T tmp = value;
      value = (T)0;
      Hw::System::enableInterrupt();

      return( tmp );
    }

    //---------------------------------------------------------------
    /*! Checks, if value of the Flag has been changed since last call
        \return value, if value has been changed and value is not 0\n
                0, otherwise
    */
    T getUnique( void )
    {
      T tmp = value;

      if( tmp == prev )
      {
        return( (T)0 );
      }
      else
      {
        prev = tmp;
        return( tmp );
      }
    }

    //---------------------------------------------------------------
    /*! Set the Flag
        \param value New value of Flag
    */
    void operator=( const T &value )
    {
      this->value = value;
    }

    //---------------------------------------------------------------
    /*! Get the Flag without changing
        \return Current value of Flag
    */
    operator T()
    {
      return( this->value );
    }

  private:
    //---------------------------------------------------------------
    volatile T  value;
    T  prev;

}; //class Flag

}  } //namespace

#endif
