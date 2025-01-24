//*******************************************************************
/*!
\file   Timer_Mcu.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   16.10.2022

License: See file "LICENSE"
*/

//*******************************************************************
#include "Timer_Mcu.h"
#include "PinConfig.h"
#include "Hardware/ReportID_Hw.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// Timer_Mcu
//
//*******************************************************************
//-------------------------------------------------------------------
#if ( system_PCLK_TIM != 80000000 )
  #error "system_PCLK_TIM not defined or wrong value"
#endif

//-------------------------------------------------------------------
Timer_Mcu *Timer_Mcu::timerPtr_1  = 0;
Timer_Mcu *Timer_Mcu::timerPtr_2  = 0;
Timer_Mcu *Timer_Mcu::timerPtr_6  = 0;
Timer_Mcu *Timer_Mcu::timerPtr_7  = 0;

//-------------------------------------------------------------------
Timer_Mcu::Timer_Mcu( TimerId timerIdIn,
                      DWORD   cycleTimeIn )

: Timer( ReportID_Hw::Module::TIMER_MCU )

{
  timerId = timerIdIn;

    switch( timerId )
    {
      case TIM_1:
      ptr           = (TIM_TypeDef*)TIM1_BASE;
      timerPtr_1    = this;
      RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
      NVIC_EnableIRQ( TIM1_CC_IRQn );
      break;

    case TIM_2:
      ptr           = (TIM_TypeDef*)TIM2_BASE;
      timerPtr_2    = this;
      RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
      NVIC_EnableIRQ( TIM2_IRQn );
      break;

    case TIM_6:
      ptr           = (TIM_TypeDef*)TIM6_BASE;
      timerPtr_6    = this;
      RCC->APB1ENR1 |= RCC_APB1ENR1_TIM6EN;
      NVIC_EnableIRQ( TIM6_IRQn );
      break;

    case TIM_7:
      ptr           = (TIM_TypeDef*)TIM7_BASE;
      timerPtr_7    = this;
      RCC->APB1ENR1 |= RCC_APB1ENR1_TIM7EN;
      NVIC_EnableIRQ( TIM7_IRQn );
      break;

    default:
      report.error( ReportID_Hw::Event::WRONG_ID );
      break;
  }

  // calculate tics and prescaler
  float rawTics = (float)system_PCLK_TIM * cycleTimeIn * 1E-6f;
  DWORD ps      = 1;
  while( rawTics > 0xFFFF ) // tics must be <= 16Bits
  {
    ps      *= 10;
    rawTics /= 10;
  }

  tics      = (DWORD)rawTics;
  cycleTime = (DWORD)(1E6 * tics * ps / (float)system_PCLK_TIM);

  ptr->DIER = TIM_DIER_UIE; // update interrupt enable
  ptr->EGR  = TIM_EGR_UG;   // re-initialize timer
  ptr->PSC  = ps - 1;
  ptr->ARR  = tics - 1;
  ptr->CR1  = TIM_CR1_CEN;  // counter enable
}

//-------------------------------------------------------------------
void Timer_Mcu::enablePWM( BYTE channel, Mode polarity )
{
  switch( timerId )
  {
    case TIM_1:
      switch( channel )
      {
        case 0: PinConfig::set( PinConfig::TIM1_CH1 ); break;
        case 1: PinConfig::set( PinConfig::TIM1_CH2 ); break;
        case 2: PinConfig::set( PinConfig::TIM1_CH3 ); break;
        case 3: PinConfig::set( PinConfig::TIM1_CH4 ); break;
        default:report.error( ReportID_Hw::Event::WRONG_CHANNEL );
      }
      //TIM_1 is an advaned Timer
      ptr->BDTR |= TIM_BDTR_MOE; //Main Output Enable
      break;

    case TIM_2:
      switch( channel )
      {
        case 0: PinConfig::set( PinConfig::TIM2_CH1 ); break;
        case 1: PinConfig::set( PinConfig::TIM2_CH2 ); break;
        case 2: PinConfig::set( PinConfig::TIM2_CH3 ); break;
        case 3: PinConfig::set( PinConfig::TIM2_CH4 ); break;
        default:report.error( ReportID_Hw::Event::WRONG_CHANNEL );
      }
      break;

    case TIM_6:
      report.error( ReportID_Hw::Event::FUNCTION_NOT_EXECUTED );
      break;

    case TIM_7:
      report.error( ReportID_Hw::Event::FUNCTION_NOT_EXECUTED );
      break;

    default:
      report.error( ReportID_Hw::Event::WRONG_ID );
      break;
  }

  WORD ccmr =  (7<<4)  // OCxM: PWM-mode 2
              |(1<<3); // OCxPE: preload enable

  WORD ccer =  (1<<0)             // CCxE: enable
              |(((polarity==NORMAL)?1:0)<<1);
                                  // CCxP: polarity
                                  //       invers: active high
                                  //       normal: active low
  switch( channel )
  {
    case 0:
      ptr->CCMR1 |= ccmr;
      ptr->CCER  |= ccer;
      break;

    case 1:
      ptr->CCMR1 |= ccmr<<8;
      ptr->CCER  |= ccer<<4;
      break;

    case 2:
      ptr->CCMR2 |= ccmr;
      ptr->CCER  |= ccer<<8;
      break;

    case 3:
      ptr->CCMR2 |= ccmr<<8;
      ptr->CCER  |= ccer<<12;
      break;

    default:report.error( ReportID_Hw::Event::WRONG_CHANNEL );
      break;
  }
}

//-------------------------------------------------------------------
void Timer_Mcu::setPWM( BYTE ch, WORD value )
{
  value = (WORD)((DWORD)value*tics >> 16);

  switch( ch )
  {
    case 0: ptr->CCR1 = value; break;
    case 1: ptr->CCR2 = value; break;
    case 2: ptr->CCR3 = value; break;
    case 3: ptr->CCR4 = value; break;
    default:report.error( ReportID_Hw::Event::WRONG_CHANNEL );
      break;
  }
}

//-------------------------------------------------------------------
void Timer_Mcu::isr(void)
{
  ptr->SR = !TIM_SR_TIF;
  isrHandler();
}

//-------------------------------------------------------------------
extern "C" {
  void TIM1_IRQHandler(void)
  {
    Timer_Mcu::timerPtr_1->isr();
  }
}

//-------------------------------------------------------------------
extern "C" {
  void TIM2_IRQHandler(void)
  {
    Timer_Mcu::timerPtr_2->isr();
  }
}

//-------------------------------------------------------------------
extern "C" {
  void TIM6_DAC_IRQHandler(void)
  {
    Timer_Mcu::timerPtr_6->isr();
  }
}

//-------------------------------------------------------------------
extern "C" {
  void TIM7_IRQHandler(void)
  {
    Timer_Mcu::timerPtr_7->isr();
  }
}

}  } //namespace

//EOF
