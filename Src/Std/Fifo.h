//*******************************************************************
/*!
\file   Fifo.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _STD_FIFO_H
#define _STD_FIFO_H

#include "Hardware/MCU/System.h"

//*******************************************************************
namespace EmbSysLib {
namespace Std {

//*******************************************************************
/*!
\class Fifo

\brief First-In-First-Out memory

The FIFO can be used for inter task communication

\example StdFifo.cpp
*/
template <class T> class Fifo
{
  public:
    //---------------------------------------------------------------
    /*! Create a FIFO and allocate it's memory
        \param sizeIn FIFO size (max number of objects stored in FIFO), default = 3
    */
    Fifo( WORD sizeIn = 3 )
    {
      in   = 0;
      out  = 0;
      anz  = 0;
      size = sizeIn;
      if( size > 0 )
      {
        speicher = new T[size];
      }
      else
      {
        size     = 0;
        speicher = NULL;
      }
    }

    //---------------------------------------------------------------
    /*! Checks, if FIFO is full
        \return
                - true, if FIFO is full
                - false, if FIFO is not full
    */
    bool isFull( void )
    {
      bool ret;

      Hw::System::disableInterrupt();
      ret = ( anz >= size );
      Hw::System::enableInterrupt();

      return( ret );
    }

    //---------------------------------------------------------------
    /*! Checks, if FIFO is empty
        \return
                - true, if FIFO is empty
                - false, if FIFO is not empty
    */
    bool isEmpty( void )
    {
      bool ret;

      Hw::System::disableInterrupt();
      ret = ( anz == 0 );
      Hw::System::enableInterrupt();

      return( ret );
    }

    //---------------------------------------------------------------
    /*! Get number of objects, currently stored in the FIFO
        \return Number of objects
    */
    WORD getCount( void )
    {
      WORD ret;

      Hw::System::disableInterrupt();
      ret = anz;
      Hw::System::enableInterrupt();

      return( ret );
    }

    //---------------------------------------------------------------
    /*! Get number of remaining free buffers in the FIFO
        \return Number of objects
    */
    WORD getFree( void )
    {
      WORD ret;

      Hw::System::disableInterrupt();
      ret = (size<=anz)?0:size - anz;
      Hw::System::enableInterrupt();

      return( ret );
    }

    //---------------------------------------------------------------
    /*! Get FIFO size
        \return FIFO size
    */
    WORD getSize( void )
    {
      return( size );
    }

    //---------------------------------------------------------------
    /*! Write an object to the FIFO
        \param ref Reference to an object
        \return
                - true, if succeeded
                - false, if FIFO is full, object not written
    */
    bool operator<<( const T &ref )
    {
      bool ret = false;

      Hw::System::disableInterrupt();
      if( anz < size )
      {
        speicher[in] = ref;
        in = (in < size-1) ? in+1 : 0;
        anz++;
        ret = true;
      }
      Hw::System::enableInterrupt();

      return( ret );
    }

    //---------------------------------------------------------------
    /*! Read an object from FIFO
        \param ref Referenc to an object
        \return
                - true, if succeeded
                - false, if FIFO is empty, no object read
    */
    bool operator>>( T &ref )
    {
      bool ret = false;

      Hw::System::disableInterrupt();
      if( anz != 0 )
      {
        ref = speicher[out];
        out = (out < size-1) ? out+1 : 0;
        anz--;
        ret = true;
      }
      Hw::System::enableInterrupt();

      return( ret );
    }

  private:
    //---------------------------------------------------------------
    T    *speicher;
    WORD  in;
    WORD  out;
    WORD  anz;
    WORD  size;

}; //class Fifo

}  } //namespace

#endif
