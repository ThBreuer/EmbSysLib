//*******************************************************************
/*!
\file   Pointer.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   01.02.2023

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _DEV_POINTER_H
#define _DEV_POINTER_H

//*******************************************************************
#include "Hardware/Common/Touch/Touch.h"

//*******************************************************************
namespace EmbSysLib {
namespace Dev {

//*******************************************************************
/*!
\class   Pointer

\brief   Screen pointer (e.g. touchscreen)

\example DevPointer.cpp
*/
class Pointer
{
  public:
    //---------------------------------------------------------------
    /*!
    \class Data
    \brief Data structure with actual pointer state
    */
    class Data
    {
      public:
        //-----------------------------------------------------------
        /*! \enum Flags
            \brief Pointer status flags
        */
        typedef enum
        {
          NONE     = 0,            //!< No event
          PRESSED  = 0x01,         //!< Pointer is pressed
          MOVE_X   = 0x02|PRESSED, //!< X-Move with more than 15steps
          MOVE_Y   = 0x04|PRESSED, //!< Y-Move with more than 15steps
          CTRL_DWN = 0x10,         //!< Pointer is activated (event)
          CTRL_UP  = 0x20          //!< Pointer is released (event)
        } Flags;

        //-----------------------------------------------------------
        WORD  posX;  //!< X-Position of pointer
        WORD  posY;  //!< Y-Position of pointer
        int   delta; //!< Moved distance
        Flags flags; //!< Event and status flags
    };

    //---------------------------------------------------------------
    /*! Initialize Pointer device with Touch hardware
        \param touch Reference to hardware
    */
    Pointer( Touch &touch );

    //---------------------------------------------------------------
    /*! Read new data from hardware
        \return Actual pointer state
    */
    virtual Data get( void );

  private:
    //---------------------------------------------------------------
    Touch  &touch;
    BYTE    isTouchedPrev;
    Data    data;
    int     posXprev;
    int     posYprev;

}; //class Pointer

} } //namespace

#endif
