//*******************************************************************
/*!
\file   Touch_FT5336.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   04.11.2024
License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_TOUCH_FT5336_H
#define _HW_TOUCH_FT5336_H

//*******************************************************************
#include "Hardware/Common/I2C/I2Cmaster.h"
#include "Hardware/Common/Touch/Touch.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class Touch_FT5336

\brief Touch screen controller FT5336 with I2C interface

\example HwTouch.cpp
*/
class Touch_FT5336 : public Touch
{
  private:
    //---------------------------------------------------------------
    class __attribute(( packed )) TscData
    {
      public:
        BYTE Xhigh;
        BYTE Xlow;
        BYTE Yhigh;
        BYTE Ylow;
    };

  public:
    //---------------------------------------------------------------
    /*! Initialize touch screen controller
        \param i2c    Reference to the I2C-object, which is connected
                      to the touch controller
        \param width  Display width [pixel]
        \param height Display height [pixel]
        \param orientation Screen orientation
    */
    Touch_FT5336( I2Cmaster  &i2c,
                  WORD        width,
                  WORD        height,
                  Orientation orientation );

    //---------------------------------------------------------------
    /*! Update coordinates and flags. This method must be called to
        read new data from the touch controller
    */
    virtual void update( void );

  private:
    //---------------------------------------------------------------
    void write( BYTE addr, BYTE data )
    {
      i2c.write(  addr, data );
    }

    //---------------------------------------------------------------
    void read( TscData &ptr )
    {
      i2c.read ( (BYTE)0x4D, &ptr );
    }

  private:
    //---------------------------------------------------------------
    I2Cmaster::Device i2c;

    static const BYTE hwAddr = 0x70;

}; //class Touch_FT5336;

}  } //namespace

#endif
