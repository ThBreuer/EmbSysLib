//*******************************************************************
/*!
\file   Dac_Mcu.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_DAC_MCU_H
#define _HW_DAC_MCU_H

//*******************************************************************
#include "Hardware/Common/Analog/Dac.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class Dac_Mcu

\brief Implementation of the digital to analog converter (DAC)

\example HwDac.cpp
*/
class Dac_Mcu : public Dac
{
  public:
    //---------------------------------------------------------------
    /*! \enum ChannelId
        \brief DAC channel identification
    */
    typedef enum
    {
      CH1 = 1, //!< DAC channel 1
      CH2,     //!< DAC channel 2
    } ChannelId;

  public:
    //---------------------------------------------------------------
    /*! Initialize the DAC hardware
    */
    Dac_Mcu( void );

    //---------------------------------------------------------------
    virtual void set( BYTE ch, 
                      WORD value );

    //---------------------------------------------------------------
    virtual void enable( BYTE ch );

}; //class Dac_Mcu

}  } //namespace

#endif
