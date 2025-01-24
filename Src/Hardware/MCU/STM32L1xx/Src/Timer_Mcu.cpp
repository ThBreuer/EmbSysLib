//*******************************************************************
/*!
\file   Timer_Mcu.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

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
#if ( system_PCLK_TIM != 32000000 )
  #error "system_PCLK_TIM not defined or wrong value"
#endif

//-------------------------------------------------------------------
Timer_Mcu *Timer_Mcu::timerPtr_2  = 0;
Timer_Mcu *Timer_Mcu::timerPtr_3  = 0;
Timer_Mcu *Timer_Mcu::timerPtr_4  = 0;
Timer_Mcu *Timer_Mcu::timerPtr_5  = 0;
Timer_Mcu *Timer_Mcu::timerPtr_6  = 0;
Timer_Mcu *Timer_Mcu::timerPtr_7  = 0;
Timer_Mcu *Timer_Mcu::timerPtr_9  = 0;
Timer_Mcu *Timer_Mcu::timerPtr_10 = 0;
Timer_Mcu *Timer_Mcu::timerPtr_11 = 0;

//-------------------------------------------------------------------
Timer_Mcu::Timer_Mcu( TimerId timerIdIn,
                      DWORD   cycleTimeIn )

: Timer( ReportID_Hw::Module::TIMER_MCU )

{
  timerId = timerIdIn;

  switch( timerId )
  {
    case TIM_2:
      ptr           = (TIM_TypeDef*)TIM2_BASE;
      timerPtr_2    = this;
      RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
      NVIC_EnableIRQ( TIM2_IRQn );
      break;

    case TIM_3:
      ptr           = (TIM_TypeDef*)TIM3_BASE;
      timerPtr_3    = this;
      RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
      NVIC_EnableIRQ( TIM3_IRQn );
      break;

    case TIM_4:
      ptr           = (TIM_TypeDef*)TIM4_BASE;
      timerPtr_4    = this;
      RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
      NVIC_EnableIRQ( TIM4_IRQn );
      break;

    case TIM_5:
      ptr           = (TIM_TypeDef*)TIM5_BASE;
      timerPtr_5    = this;
      RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
      NVIC_EnableIRQ( TIM5_IRQn );
      break;

    case TIM_6:
      ptr           = (TIM_TypeDef*)TIM6_BASE;
      timerPtr_6    = this;
      RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
      NVIC_EnableIRQ( TIM6_IRQn );
      break;

    case TIM_7:
      ptr           = (TIM_TypeDef*)TIM7_BASE;
      timerPtr_7    = this;
      RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
      NVIC_EnableIRQ( TIM7_IRQn );
      break;

    case TIM_9:
      ptr           = (TIM_TypeDef*)TIM9_BASE;
      timerPtr_9    = this;
      RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;
      NVIC_EnableIRQ( TIM9_IRQn );
      break;

    case TIM_10:
      ptr           = (TIM_TypeDef*)TIM10_BASE;
      timerPtr_10   = this;
      RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;
      NVIC_EnableIRQ( TIM10_IRQn );
      break;

    case TIM_11:
      ptr           = (TIM_TypeDef*)TIM11_BASE;
      timerPtr_11    = this;
      RCC->APB2ENR |= RCC_APB2ENR_TIM11EN;
      NVIC_EnableIRQ( TIM11_IRQn );
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

  ptr->CR1  = TIM_CR1_CEN;  // counter enable
  ptr->DIER = TIM_DIER_UIE; // update interrupt enable
  ptr->EGR  = TIM_EGR_UG;   // re-initialize timer
  ptr->PSC  = ps   - 1;
  ptr->ARR  = tics - 1;
}

//-------------------------------------------------------------------
void Timer_Mcu::enablePWM( BYTE channel, Mode polarity )
{
  switch( timerId )
  {
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

    case TIM_3:
      switch( channel )
      {
        case 0: PinConfig::set( PinConfig::TIM3_CH1 ); break;
        case 1: PinConfig::set( PinConfig::TIM3_CH2 ); break;
        case 2: PinConfig::set( PinConfig::TIM3_CH3 ); break;
        case 3: PinConfig::set( PinConfig::TIM3_CH4 ); break;
        default:report.error( ReportID_Hw::Event::WRONG_CHANNEL );
      }
      break;

    case TIM_4:
      switch( channel )
      {
        case 0: PinConfig::set( PinConfig::TIM4_CH1 ); break;
        case 1: PinConfig::set( PinConfig::TIM4_CH2 ); break;
        case 2: PinConfig::set( PinConfig::TIM4_CH3 ); break;
        case 3: PinConfig::set( PinConfig::TIM4_CH4 ); break;
        default:report.error( ReportID_Hw::Event::WRONG_CHANNEL );
      }
      break;

    case TIM_5:
      report.error( ReportID_Hw::Event::FUNCTION_NOT_EXECUTED );
      break;

    case TIM_6:
      report.error( ReportID_Hw::Event::FUNCTION_NOT_EXECUTED );
      break;

    case TIM_7:
      report.error( ReportID_Hw::Event::FUNCTION_NOT_EXECUTED );
      break;

    case TIM_9:
      switch( channel )
      {
        case 0: PinConfig::set( PinConfig::TIM9_CH1 ); break;
        case 1: PinConfig::set( PinConfig::TIM9_CH2 ); break;
        default:report.error( ReportID_Hw::Event::WRONG_CHANNEL );
      }
      break;

    case TIM_10:
      switch( channel )
      {
        case 0: PinConfig::set( PinConfig::TIM10_CH1 ); break;
        default:report.error( ReportID_Hw::Event::WRONG_CHANNEL );
      }
      break;

    case TIM_11:
      switch( channel )
      {
        case 0: PinConfig::set( PinConfig::TIM11_CH1 ); break;
        default:report.error( ReportID_Hw::Event::WRONG_CHANNEL );
      }
      break;

    default:
      report.error( ReportID_Hw::Event::WRONG_ID );
      break;
  }

  WORD ccmr =  (7<<4)  // OCxM: PWM-mode 2
              |(0<<3); // OCxPE: preload enable

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
  void TIM2_IRQHandler(void)
  {
    Timer_Mcu::timerPtr_2->isr();
  }
}

//-------------------------------------------------------------------
extern "C" {
  void TIM3_IRQHandler(void)
  {
    Timer_Mcu::timerPtr_3->isr();
  }
}

//-------------------------------------------------------------------
extern "C" {
  void TIM4_IRQHandler(void)
  {
    Timer_Mcu::timerPtr_4->isr();
  }
}

//-------------------------------------------------------------------
extern "C" {
  void TIM5_IRQHandler(void)
  {
    Timer_Mcu::timerPtr_5->isr();
  }
}

//-------------------------------------------------------------------
extern "C" {
  void TIM6_IRQHandler(void)
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

//-------------------------------------------------------------------
extern "C" {
  void TIM9_IRQHandler(void)
  {
    Timer_Mcu::timerPtr_9->isr();
  }
}

//-------------------------------------------------------------------
extern "C" {
  void TIM10_IRQHandler(void)
  {
    Timer_Mcu::timerPtr_10->isr();
  }
}

//-------------------------------------------------------------------
extern "C" {
  void TIM11_IRQHandler(void)
  {
    Timer_Mcu::timerPtr_11->isr();
  }
}

}  } //namespace

//EOF
