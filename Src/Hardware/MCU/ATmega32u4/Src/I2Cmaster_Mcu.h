//*******************************************************************
/*!
\file   I2Cmaster_Mcu.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   30.04.2023

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_I2C_MASTER_MCU_H
#define _HW_I2C_MASTER_MCU_H

//*******************************************************************
#include "Hardware/Common/I2C/I2Cmaster.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class I2Cmaster_Mcu

\brief Implementation of the I2C interface in master mode

\example HwI2Cmaster.cpp
*/
class I2Cmaster_Mcu : public I2Cmaster
{
  public:
    //---------------------------------------------------------------
    /*! Initialize I2C hardware
        \param fclk SCL clock rate [kHz]
    */
    I2Cmaster_Mcu( WORD fclk ); 

  private:
    //---------------------------------------------------------------
    virtual void start( BYTE size );

    //---------------------------------------------------------------
    virtual void stop( void );

    //---------------------------------------------------------------
    virtual void sendAddr( BYTE addr );

    //---------------------------------------------------------------
    virtual void writeByte( BYTE byte );

    //---------------------------------------------------------------
    virtual BYTE readByteAck( void );

    //---------------------------------------------------------------
    virtual BYTE readByteNack( void );

    //---------------------------------------------------------------
    virtual void waitForAckn( MTYPE state );

}; //class I2Cmaster_Mcu

}  } //namespace

#endif
