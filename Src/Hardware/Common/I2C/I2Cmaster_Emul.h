//*******************************************************************
/*!
\file   I2Cmaster_Emul.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_I2C_MASTER_EMUL_H
#define _HW_I2C_MASTER_EMUL_H

//*******************************************************************
#include "Hardware/Common/Port/Port.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class I2Cmaster_Emul

\brief Implementation of the I2C interface in master mode

This class implements an emulated (bit banging) I2C bus interface

  Timing: Due to the unknown execution time of GPIO access, the 
  desired SCL rate can not allways be generated. Instead of this, a 
  timeout \b twait can be used to decrement SCL rate

\example HwI2Cmaster.cpp
*/
class I2Cmaster_Emul : public I2Cmaster
{
  public:
    //---------------------------------------------------------------
    /*! Initialize I2C hardware.\n
        \param pinSCL pin number SCL
        \param pinSDA pin number SDA
        \param twait  Additional waiting time [us] reducing SCL rate
    */
    I2Cmaster_Emul( Port::Pin &pinSCL,
                    Port::Pin &pinSDA,
                    WORD       twait = 0 );

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
    // Local method, wait until slave is responding
    //
    virtual void waitForAckn( void );

    //---------------------------------------------------------------
    virtual void wait( void );

  private:
    //---------------------------------------------------------------
    WORD twait;

    Port::Pin &pinSCL;
    Port::Pin &pinSDA;

}; //class I2Cmaster_Emul

} } //namespace

#endif
