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
      SPI_2,      //!< SPI 2
      SPI_3       //!< SPI 3
    } SPI_ID;

    //---------------------------------------------------------------
    /*! \enum SPI baudrate
    */
    typedef enum
    {
      CR_250kHz  =  0, //!< Clock rate = 250kHz
      CR_500kHz,       //!< Clock rate = 500kHz
      CR_1000kHz,      //!< Clock rate = 1MHz
      CR_2000kHz,      //!< Clock rate = 2MHz
      CR_4000kHz,      //!< Clock rate = 4MHz
      CR_8000kHz       //!< Clock rate = 8MHz
    } SPI_Baudrate;

  public:
    //---------------------------------------------------------------
    /*! Initialize SPI hardware
        \param id           Hardware selection
               mode         Clock rate
               clockPolPha  Clock polarity and phase
    */
    SPImaster_Mcu( SPI_ID        id,      
                    SPI_Baudrate baudrate,       
                    ClockPolPha  clockPolPha );

  private:
    //---------------------------------------------------------------
    virtual BYTE transceiveByte( BYTE value = 0 );

  private:
    //---------------------------------------------------------------
    SPI_TypeDef *ptr;

}; //class SPImaster_Mcu

}  } //namespace

#endif
