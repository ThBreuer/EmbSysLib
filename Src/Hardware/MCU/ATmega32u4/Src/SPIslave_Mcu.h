//*******************************************************************
/*!
\file   SPIslave_Mcu.h
\author Thomas Breuer
\date   02.03.2011

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_SPI_SLAVE_MCU_H
#define _HW_SPI_SLAVE_MCU_H

//*******************************************************************
#include "Hardware/Common/SPI/SPIslave.h"
//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class SPIslave_0

\brief Implementation of the SPI interface in slave mode

\example SPIslave.cpp
*/
class SPIslave_0 : public SPIslave
{
  public:
    //---------------------------------------------------------------
    /*! Initialize SP hardware.\n
        \param handler  Reference to the aplication specific
                        SPIslave::DataHandler
    */
    SPIslave_0( SPIslave::DataHandler &handler );

    //---------------------------------------------------------------
    void isr( void );

  public:
    //---------------------------------------------------------------
    static SPIslave_0 *spiPtr;

}; //class SPIslave_0

}  } //namespace

#endif
