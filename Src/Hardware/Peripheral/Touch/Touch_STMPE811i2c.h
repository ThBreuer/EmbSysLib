//*******************************************************************
/*!
\file   Touch_STMPE811i2c.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   17.02.2023
License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_TOUCH_STMPE811_I2C_H
#define _HW_TOUCH_STMPE811_I2C_H

//*******************************************************************
#include "Hardware/Common/I2C/I2Cmaster.h"
#include "Hardware/Common/Touch/Touch.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class Touch_STMPE811i2c

\brief Touch screen controller STMPE811, using I2C interface

\example HwTouch.cpp
*/
class Touch_STMPE811i2c : public Touch
{
  class __attribute(( packed )) TscData
  {
    public:
      BYTE Xhigh;
      BYTE Xlow;
      BYTE Yhigh;
      BYTE Ylow;
      BYTE Z;
  };

  public:
    //---------------------------------------------------------------
    /*! Initialize touch screen controller
        \param i2c    Reference to the I2C-object, which is connected
                      to the touch controller
        \param addr   I2C device (sub-)address of the touch
                      controller (0 or 1)
        \param width  Display width [pixel]
        \param height Display height [pixel]
    */
    Touch_STMPE811i2c( I2Cmaster  &i2c,
                       BYTE        addr,
                       WORD        width,
                       WORD        height,
                       Orientation orientation );

    //---------------------------------------------------------------
    /*! Update coordinates and flags. This method must be called to
        read new data from the touch controller
    */
    virtual void update( void );

  public:
    void write( BYTE addr, BYTE data )
    {
      i2c.write(  addr, data );
    }

    void read( TscData &ptr )
    {
      i2c.read ( (BYTE)0x4D, &ptr );
    }

  private:
    //---------------------------------------------------------------
  I2Cmaster::Device i2c;

}; //class Touch_STMPE811i2c

}  } //namespace

#endif
