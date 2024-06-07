//*******************************************************************
/*!
\file   Encoder_Mcu.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   26.01.2024

License: See file "LICENSE"
*/

//*******************************************************************
#include "Encoder_Mcu.h"
#include "PinConfig.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
// 
// Encoder_Mcu
//
//*******************************************************************
//-------------------------------------------------------------------
Encoder_Mcu::Encoder_Mcu( TimerId timerId,
                          Mode    mode )
                            
:Encoder( mode )

{
  switch( timerId )
  {  
    default:
      // todo error!
      break;

////    case TIM_1: 
////      ptr           = TIM1;
////      RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

////      cHwPinConfig::set( cHwPinConfig::TIM1_CH1 );
////      cHwPinConfig::set( cHwPinConfig::TIM1_CH2 );
////      break;

////    case TIM_2: 
////      ptr           = TIM2;
////      RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;

////      cHwPinConfig::set( cHwPinConfig::TIM2_CH1 );
////      cHwPinConfig::set( cHwPinConfig::TIM2_CH2 );
////      break;

//    case TIM_3: 
//      ptr           = TIM3;
//      RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

//      cHwPinConfig::set( cHwPinConfig::TIM3_CH1 );
//      cHwPinConfig::set( cHwPinConfig::TIM3_CH2 );
//      break;

//    case TIM_4: 
//      ptr           = TIM4;
//      RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

//      cHwPinConfig::set( cHwPinConfig::TIM4_CH1 );
//      cHwPinConfig::set( cHwPinConfig::TIM4_CH2 );
//      break;

//    case TIM_5: 
//      ptr           = TIM5;
//      RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;

//      cHwPinConfig::set( cHwPinConfig::TIM5_CH1 );
//      cHwPinConfig::set( cHwPinConfig::TIM5_CH2 );
//      break;

  }
  
  ptr->CR1  = TIM_CR1_CEN;  // counter enable
  
  ptr->DIER = !TIM_DIER_UIE; // update interrupt enable
  ptr->EGR  = TIM_EGR_UG;   // re-initialize timer
  ptr->PSC  = 0;
  ptr->ARR  = 0xFFFF;

  ptr->SMCR &= ~(0x07<<0);
  ptr->SMCR |=  (0x03<<0); // Encode mode, both edges
  
  // \todo Check setting of CCER
  ptr->CCER  &= ~( (1<<1)
                  |(1<<5));
  
  // \todo polarity OK? compare STM32F4xx
  if( mode & REVERSE )        // Input polarity
  {    
    ptr->CCER  |=  (1<<1)    // CC1P
                  |(0<<5);   // CC2P
  }
  else // NORMAL
  {
    ptr->CCER  |=  (0<<1)    // CC1P
                  |(0<<5);   // CC2P
  }

  ptr->CNT = 0;
  cntLast  = 0;
}

//-------------------------------------------------------------------
int Encoder_Mcu::get( void )
{
  // warning: Do not change data types and/or casting!
  WORD  cnt = ptr->CNT;
  int   ret = (short int)(cnt - cntLast);
  
  cntLast = cnt;
  
  return( ret );
}

}  } //namespace

//EOF
