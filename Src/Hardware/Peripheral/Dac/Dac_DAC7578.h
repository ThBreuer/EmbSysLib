//*******************************************************************
/*!
\file   Dac_DAC7578.h
\author ...
\date

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_DAC_DAC7578_H
#define _HW_DAC_DAC7578_H

//*******************************************************************
#include "Hardware/Common/Analog/Dac.h"
#include "Hardware/Common/I2C/I2Cmaster.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*! 
\class Dac_DAC7578

\brief Implementation of the digital anallog converter DAC7578 (I2C-device)

\example HwDac.cpp
*/
class Dac_DAC7578 : public Dac
{
  private:
    typedef enum
    {
      WRITE_UPDATE  = 0x30,
      POWER_DOWN_ON = 0x40,
    } Command;

  public:
    //---------------------------------------------------------------
    /*! Initialize DAC7578
        \param i2c    Reference to I2C interface
        \param hwAddr Sub address of DAC7578 (address pins A0,A1,A2)
    */
    Dac_DAC7578( I2Cmaster &i2c,
                 BYTE       hwAddr );

  //---------------------------------------------------------------
  virtual void set( BYTE ch,
                    WORD value );

  //---------------------------------------------------------------
  virtual void enable( BYTE ch );

  private:
    //---------------------------------------------------------------
    I2Cmaster::Device  i2c;

    //---------------------------------------------------------------
    static const BYTE baseAdr = 0x90;
    static const BYTE maskAdr = 0x07;

}; //class Dac_DAC7578

}  } //namespace

#endif
