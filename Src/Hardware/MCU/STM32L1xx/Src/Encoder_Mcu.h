//*******************************************************************
/*!
\file   Encoder_Mcu.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   26.01.2024

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_ENCODER_MCU_H
#define _HW_ENCODER_MCU_H

//*******************************************************************
#include "Hardware/Common/Encoder/Encoder.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*! 
\class Encoder_Mcu

\brief Implementation of encoder in timer 2/3/4

\example HwEncoder.cpp
*/
class Encoder_Mcu : public Encoder
{
  public:
    //---------------------------------------------------------------
    /*! \enum TimerId
        \brief Timer identification
    */
    typedef enum
    {
      TIM_2 = 2,  //!< Timer 2 
      TIM_3,      //!< Timer 3 
      TIM_4,      //!< Timer 4
    } TimerId;

  public:
    //---------------------------------------------------------------
    /*! Instantiate encoder
        \param timerId Timer hardware selection 
        \param mode    Operating mode
    */
    Encoder_Mcu( TimerId timerId, Mode mode );

    //---------------------------------------------------------------
    virtual int get();

  private:
    //---------------------------------------------------------------
    TIM_TypeDef *ptr;
    WORD         cntLast;

}; //clas  Encoder_Mcu

}  } //namespace

#endif
