//*******************************************************************
/*!
\file   Sequence.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.11.2021

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _STD_SEQUENCE_H
#define _STD_SEQUENCE_H

//*******************************************************************
#include "Mcu_Types.h"

//*******************************************************************
namespace EmbSysLib {
namespace Std {

//*******************************************************************
/*!
\class Sequence

\brief Base class for single linked lists

\example StdSequence.cpp
*/
template <class T> class Sequence
{
  public:
    //***************************************************************
    /*!
    \class Item

    \brief Base class for list items

    \example StdSequence.cpp
    */
    class Item
    {
      public:
        //-----------------------------------------------------------
        /*! Instatiate a Sequence::Item
            \param sequence If not zero, the item is added to \a Sequence
        */
        Item( Sequence<T> *sequence = 0 )
        {
          PtrNext = 0;
          if( sequence )
          {
            sequence->add( (T*)this );
          }
        }

        //-----------------------------------------------------------
        /*! Get the next item of linked list
            \return Pointer to next item or zero
        */
        T *getNext( void )
        {
          return( PtrNext );
        }

      private:
        //-----------------------------------------------------------
        T *PtrNext; // Pointer to next entry

      friend class Sequence;

    }; //class Sequence::Item
    //***************************************************************

  public:
    //---------------------------------------------------------------
    /*! Instantiate an empty list
    */
    Sequence( void )
    {
      PtrFirst = 0;
    }

    //---------------------------------------------------------------
    /*! Add an item to the list
        \param itemPtr Pointer to the item
        \return
                - true: Item added
                - false: Item not added, because it is already in list
    */
    BYTE add( T *itemPtr )
    {
      T *ptr;

      // Is itemPtr still in list?
      ptr = PtrFirst;

      while( ptr )
      {
        if( ptr == itemPtr )
        {
          return( false );
        }
        if(ptr->PtrNext) // Does curent object have a next object?
        {
          ptr = ptr->PtrNext;
        }
        else
        {
          break;
        }
      }

      // Add a new object,
      // ptr may referes to last object in the list
      if( ptr ) // append new object to last object in list
      {
        ptr->PtrNext = itemPtr;
      }
      else // insert first object into list
      {
        PtrFirst = itemPtr;
      }
      return( true );
    }

    //---------------------------------------------------------------
    /*! Get the pointer to the first item in the list
        \return Pointer to the first item\n
                Zero, if list is empty
    */
    T* getFirst( void )
    {
      return( PtrFirst );
    }

    //---------------------------------------------------------------
    /*! Get the pointer to an indexed item in the list
        \return Pointer to the item\n
                Zero, if list is empty or index exeeds size of list
    */
    T* operator[](WORD index)
    {
      T* ptr = PtrFirst;
      while( ptr && index-- )
      {
        ptr = ptr->PtrNext;
      }
      return( ptr );
    }

  private:
    //---------------------------------------------------------------
    T *PtrFirst; // Pointer to first entry

}; //class Sequence

}  } //namespace

#endif
