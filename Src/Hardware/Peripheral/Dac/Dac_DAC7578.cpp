//*******************************************************************
/*!
\file   Dac_DAC7578.cpp
\author ...
\date   ...

License: See file "LICENSE"
*/

//*******************************************************************
#include "Dac_DAC7578.h"
#include "Hardware/ReportID_Hw.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
// 
// Dac_DAC7578
//
//*******************************************************************
//-------------------------------------------------------------------
Dac_DAC7578::Dac_DAC7578( I2Cmaster &i2cIn,
                          BYTE       hwAddr )

: Dac( ReportID_Hw::Module::DAC_MCU )
, i2c(i2cIn, baseAdr | ( (hwAddr&maskAdr) << 1 ))

{
}

//-------------------------------------------------------------------
void Dac_DAC7578::set( BYTE ch, WORD value )
{
  WORD x = (value>>8)|(value<<8);
  i2c.write( (BYTE)(WRITE_UPDATE | ch), x );
}

//-------------------------------------------------------------------
void Dac_DAC7578::enable( BYTE ch )
{
  WORD x = (WORD)0x01<<(ch+5);
  x = (x>>8)|(x<<8);
  i2c.write( (BYTE)POWER_DOWN_ON, x );
}

}  } //namespace

//EOF
