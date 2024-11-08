//*******************************************************************
/*!
\file   Touch_FT5336.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   04.11.2024

\see
*/

//*******************************************************************
#include "Touch_FT5336.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// Touch_FT5336
//
//*******************************************************************
//-------------------------------------------------------------------
Touch_FT5336::Touch_FT5336( I2Cmaster  &i2cBus,
                            WORD        width,
                            WORD        height,
                            Orientation orientation )

: Touch( ReportID_Hw::Module::TOUCH_FT5336, 
         width, 
         height,
         orientation ),

  i2c( i2cBus, hwAddr )
{
}

//-------------------------------------------------------------------
void Touch_FT5336::update( void )
{
  TscData tsc;

  i2c.read( (BYTE)0x03, &tsc );
  if( !i2c.isError() )
  {
    if( ( tsc.Xhigh & 0xC0 ) == 0x80 /* contact */ )
    {
      //!< \todo coordinate mapping and offsets depending on display orientation
      
      yPos = ( ((WORD)(tsc.Xhigh&0x0F)<<8) | tsc.Xlow );
      xPos = ( ((WORD)(tsc.Yhigh&0x0F)<<8) | tsc.Ylow );
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
