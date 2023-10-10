//*******************************************************************
/*!
\file   SPImaster_Mcu.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   02.03.2011

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
\class SPImaster_0

\brief Implementation of the SPI interface in master mode

\example cHwSPImaster.cpp
*/
class SPImaster_0 : public SPImaster
{
  public:
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
    /*! Initialize SPI hardware.\n
        \param mode   SPI clock rate selection
    */
    SPImaster_0( SPI_Baudrate        baud,
                 ClockPolPha clockPolPhaIn );

  private:
    //---------------------------------------------------------------
    virtual BYTE transceiveByte(BYTE value = 0);

    //---------------------------------------------------------------
    virtual BYTE transceiveByteExt( BYTE bit = 0, BYTE value = 0);

}; //class SPImaster_0

}  } //namespace

#endif
