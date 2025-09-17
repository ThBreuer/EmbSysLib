//*******************************************************************
/*!
\file   Touch_FT6206.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   03.04.2019

\see
*/

//*******************************************************************
#include "Touch_FT6206.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// Touch_FT6206
//
//*******************************************************************
//-------------------------------------------------------------------
Touch_FT6206::Touch_FT6206( I2Cmaster  &i2cBus,
                            WORD        width,
                            WORD        height,
                            Orientation orientation,
                            BYTE        variant )

: Touch( ReportID_Hw::Module::TOUCH_FT6206, 
         width, 
         height,
         orientation ),

  i2c( i2cBus, variant /* variant I2C addr */ )
{
}

//-------------------------------------------------------------------
void Touch_FT6206::update( void )
{
  TscData tsc;

  i2c.read( (BYTE)0x03, &tsc );
  if( !i2c.isError() )
  {
    if( ( tsc.Xhigh & 0xC0 ) == 0x80 /* contact */ )
    {
      //!< \todo coordinate mapping and offsets depending on display orientation
      
      xPos = ( ((WORD)(tsc.Xhigh&0x0F)<<8) | tsc.Xlow );
      yPos = ( ((WORD)(tsc.Yhigh&0x0F)<<8) | tsc.Ylow );
      isTouchedFlag = true;
    }
    else
    {
      isTouchedFlag = false;
    }
  }
}

}  } //namespace

//EOF
