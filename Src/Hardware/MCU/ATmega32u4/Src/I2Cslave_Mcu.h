//*******************************************************************
/*!
\file   I2Cslave_Mcu.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   02.03.2011

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_I2C_SLAVE_MCU_H
#define _HW_I2C_SLAVE_MCU_H

//*******************************************************************
#include "Hardware/Common/I2C/I2Cslave.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class I2Cslave_0

\brief Implementation of the I2C interface in slave mode

\warning A bus error on I2C will trigger a system reset

\example cHwI2Cslave.cpp
*/
class I2Cslave_0 : public I2Cslave
{
  public:
    //---------------------------------------------------------------
    /*! Initialize hardware
        \param handler  Reference to the application specific
                        cHwI2CslaveDataHandler
        \param hwAddr   Hardware address of this I2C device (LSB = 0)
    */
    I2Cslave_0( DataHandler &handler,
                   BYTE         hwAddr  );

    //---------------------------------------------------------------
    virtual void isr( void );

  private:
    //---------------------------------------------------------------
    virtual void SetControlReg( void );

  public:
    //---------------------------------------------------------------
    static I2Cslave_0 *i2cPtr;

  private:
    //---------------------------------------------------------------
    BYTE cnt; // Counter for repeated transactions

}; //I2Cslave_0

}  } //namespace

#endif
