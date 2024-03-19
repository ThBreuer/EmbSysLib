//*******************************************************************
/*!
\file   Touch.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   10.03.2013

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_DISPLAY_TOUCH_H
#define _HW_DISPLAY_TOUCH_H

//*******************************************************************
#include "Mcu_Types.h"
#include "Std/Report.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class Touch

\brief Abstract class supporting touch sensor of a graphic display

The display position is given by the coordinates (x,y), with a
starting point in the (left,top)-corner

Derived classes should at least:
 - initialize the hardware interface
 - implement the method update()
 - set the member variables xpos, ypos and isTouchedFlag

\example HwTouch.cpp
*/
class Touch
{
  public:
    //---------------------------------------------------------------
    typedef enum
    {
      ROTATION_0     = 0x00,
      ROTATION_90    = 0x01,
      ROTATION_180   = 0x02,
      ROTATION_270   = 0x03
    } Orientation;

  protected:
    //---------------------------------------------------------------
    // Initialize the touch controller
    // moduleId:
    // width:  Display area width [pixel]
    // height: Display area height [pixel]
    // orientation:
    Touch( WORD        mduleId,
           WORD        width,
           WORD        height,
           Orientation orientation );

  public:
    //---------------------------------------------------------------
    /*! Update coordinates and flags. This method must be called to
        read new data from the touch controller
    */
    virtual void update( void ) = 0;

    //---------------------------------------------------------------
    /*! Get x-position of the point, where the display is touched
        \return
                - X position, if touched
                - 0, if display is not touched
    */
    virtual WORD getPosX( void );

    //---------------------------------------------------------------
    /*! Get y-position of the point, where the display is touched
        \return
                - Y position, if touched
                - 0, if display is not touched
    */
    virtual WORD getPosY( void );

    //---------------------------------------------------------------
    /*! Return whether the display is touched or not
        \return
                - false: Display is NOT touched
                - true:  Display is touched
    */
    virtual BYTE isTouched( void );

  protected:
    //---------------------------------------------------------------
    WORD xPos;
    WORD yPos;
    BYTE isTouchedFlag;

    WORD        width;
    WORD        height;
    Orientation orientation;

  public:
    //---------------------------------------------------------------
    Std::Report report;

}; //class Touch;

}  } //namespace

#endif
