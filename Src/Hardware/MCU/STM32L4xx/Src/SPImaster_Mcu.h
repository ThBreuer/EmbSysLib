//*******************************************************************
/*!
\file   SPImaster_Mcu.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   20.03.2023

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_SPI_MASTER_MCU_H
#define _HW_SPI_MASTER_MCU_H

//*******************************************************************
#include "Hardware/Common/SPI/SPImaster.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class SPImaster_Mcu

\brief Implementation of SPI hardware in master mode

\example cHwSPImaster.cpp
*/
class SPImaster_Mcu : public SPImaster
{
  public:
    //---------------------------------------------------------------
    /*! \enum SPI_ID
        \brief SPI identification
    */
    typedef enum
    {
      SPI_1 = 1,  //!< SPI 1
      SPI_3 = 3   //!< SPI 3
    } SPI_ID;

    //---------------------------------------------------------------
    /*! \enum SPI baudrate
    */
    typedef enum
    {
      CR_312kHz  =  0, //!< Clock rate =  312.5 kHz
      CR_625kHz,       //!< Clock rate =  625   kHz
      CR_1250kHz,      //!< Clock rate = 1250   kHz
      CR_2500kHz,      //!< Clock rate = 2500   kHz
      CR_5MHz,         //!< Clock rate =  5 MHz
      CR_10MHz,        //!< Clock rate = 10 MHz
      CR_20MHz,        //!< Clock rate = 20 MHz
      CR_40MHz         //!< Clock rate = 40 MHz
    } SPI_Baudrate;

  public:
    //---------------------------------------------------------------
    /*! Initialize SPI hardware
        \param id           Hardware selection
        \param mode         Clock rate
        \param clockPolPha  Clock polarity and phase
        \param lsbFirst
    */
    SPImaster_Mcu( SPI_ID        id,
                    SPI_Baudrate baudrate,
                    ClockPolPha  clockPolPha,
                    bool         lsbFirst );

    //---------------------------------------------------------------
    /*! todo comment
    */
    virtual BYTE transceiveByte( BYTE value = 0 );

  private:
    //---------------------------------------------------------------
    SPI_TypeDef *ptr;

}; //class SPImaster_Mcu

}  } //namespace

#endif
