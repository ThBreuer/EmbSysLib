//*******************************************************************
/*!
\file   I2Cmaster_Mcu.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

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
    /*! I2C hardware ID's
    */
    typedef enum
    {
      I2C_1 = 1, //!< Use I2C1 
      I2C_2,     //!< Use I2C2
      I2C_3,     //!< Use I2C3
      I2C_4      //!< Use I2C4
    } I2C_ID;
    
  public:
    //---------------------------------------------------------------
    /*! Initialize I2C hardware
        \param id   Hardware idetification
        \param fclk SCL clock rate [kHz]
    */
    I2Cmaster_Mcu( I2C_ID id, 
                   WORD   fclk );

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

  private:
    //---------------------------------------------------------------
    I2C_TypeDef* ptr;

}; //class I2Cmaster_Mcu

}  } //namespace

#endif
