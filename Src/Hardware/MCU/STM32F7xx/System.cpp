//*******************************************************************
/*!
\file   System.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   16.10.2022

License: See file "LICENSE"

\brief  Controller specific implementation of class cSystem
*/

//*******************************************************************
#include "Hardware/MCU/System.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// cSystem
//
//*******************************************************************
//-------------------------------------------------------------------
unsigned char System::cntInterrupt = 0;

//-------------------------------------------------------------------
System::System( bool disableInterrupts )
{
  disableWatchdog();
  if( disableInterrupts )
  {
    disableInterrupt();
  }
}

//-------------------------------------------------------------------
void System::start( void )
{
  enableInterrupt();
}

//-------------------------------------------------------------------
void System::disableInterrupt( void )
{
  __disable_irq();
  cntInterrupt++;
}

//-------------------------------------------------------------------
void System::enableInterrupt( void )
{
  if( cntInterrupt > 0 )
  {
    cntInterrupt--;
  }
  if( cntInterrupt == 0 )
  {
     __enable_irq();
  }
}

//-------------------------------------------------------------------
void System::enableWatchdog( WORD resetTime )
{
  DWORD pr  = 0;
  DWORD rlr = 0;

  /*----------------------------------------
   timeout =  (4*2^pr) / f_LSI * rlr
   => rlr = timeout * f_LSI / (4*2^pr)

   f_LSI = 40kHz
   RLR: 12 Bit (0,..,4095)
   PR: 0,..6
  ----------------------------------------*/
  rlr = resetTime*10; // 10 = ms*40kHz/4
  while( resetTime > 0x0FFF && pr < 6)
  {
    resetTime >>= 2;
    pr++;
  }

  IWDG->KR  = 0x5555;
  IWDG->PR  = pr;
  IWDG->KR  = 0x5555;
  IWDG->RLR = rlr;
  IWDG->KR  = 0xAAAA;
  IWDG->KR  = 0xCCCC;
}

//-------------------------------------------------------------------
void System::disableWatchdog( void )
{
  // The watchdog can NOT be disabled by software
}

//-------------------------------------------------------------------
void System::feedWatchdog( void )
{
  IWDG->KR  = 0xAAAA;
}

//-------------------------------------------------------------------
void System::reset( void )
{
  __DSB();
  *((unsigned long *)(0x2000FFF0)) = 0xDEADBEEF;
  NVIC_SystemReset();
}

#pragma GCC push_options
#pragma GCC optimize ("-O0")
//-------------------------------------------------------------------
void System::delayMicroSec( unsigned short delay )
{
  //! \todo check calibration
  // Calibrated at delayMilliSec( 1000 )
  // Calibrated with System::delayMilliSec( 1000 ), f_clk = 200 MHz, no interrupts

  unsigned steps = 0.499E-6*system_PCLK*delay;
  for( ; steps > 0; steps-- )
  {
    asm volatile("nop");
  }
}

//-------------------------------------------------------------------
void System::delayMilliSec( unsigned short delay )
{
  for( ; delay > 0; delay-- )
  {
    delayMicroSec(1000);
  }
}
#pragma GCC pop_options

}  } //namespace

//-------------------------------------------------------------------
#include "Sys/stm32f7xx.h"

//-------------------------------------------------------------------
#if !defined  (HSE_STARTUP_TIMEOUT)
  #define HSE_STARTUP_TIMEOUT  (0x05000)  //!< Time out for HSE start up
#endif

//*******************************************************************
//
// SystemInit
//
//*******************************************************************
void SystemInit( void )
{
  // Set FPU
  #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    // System Control Block (SCB), Coprocessor Access Control Register
    SCB->CPACR |=   (3UL << 10*2)  // CP10: Full access
                  | (3UL << 11*2); // CP11: Full access
  #endif

//	SCB_DisableDCache();

  // Clock control register
  RCC->CR |= RCC_CR_HSION;    //! \todo why is it switched on here?

  // Clock configuration register
  RCC->CFGR = 0x00000000;       // reset ...

  // Clock control register
  RCC->CR &= ~(   RCC_CR_PLLON  // reset ...
                | RCC_CR_CSSON
                | RCC_CR_HSEON );

  // PLL configuration register
  RCC->PLLCFGR = 0x24003010;   // Reset value,
                               // see Reference manual

  // Clock control register
  RCC->CR &= ~(RCC_CR_HSEBYP); // reset ...

  // Clock interrupt register
  RCC->CIR = 0;                // Disable all interrupts


  // Flash Acess Control Register
  FLASH->ACR =  // FLASH_ACR_ICEN          // Instruction cache enable
               //| FLASH_ACR_DCEN          // Data cache enable
               0| FLASH_ACR_LATENCY_7WS;  // Latency: 5 wait states

  // Advanced Peripheral Bus Enable Register
  RCC->APB1ENR |= RCC_APB1ENR_PWREN;    // Power interface clock: enable

  // Power Control Register
//    PWR->CR   |= (1<<14);                 // Regulator voltage scaling output selection:
                                        // VOS=1: Scale 1 mode

  // Clock Configuration Register
  //! \todo Check clock configuration
  RCC->CFGR |=   RCC_CFGR_HPRE_DIV1     // AHB prescaler:                   HCLK  = SYSCLK
               | RCC_CFGR_PPRE2_DIV4    // APB high-speed prescaler (APB2): PCLK2 = HCLK/4
               | RCC_CFGR_PPRE1_DIV4;   // APB low-speed  prescaler (APB1): PCLK1 = HCLK/2

  //-----------------------------------------------------------------
  /* Clock configuration

    //---------------------------------------------------------------
    SYSCLK = 200MHz, using PLL

    Settings by compiler flags:

      compiler flag     | value
      ------------------+------------------+-------
      OSCSRC            | HSE,             | HSI
      oscillator source | HSE_BYPASS       |
      ------------------+------------------+-------
      OSCFREQ           | 4000,...,26000?  | ---
      f_osc/kHz         |                  |
                        |                  |
      ------------------+------------------+-------

    //---------------------------------------------------------------
       PLL configuration

        f_osc  +----+ f_in  +-----+ f_out      +----+   f_pllclk
        ------>| /M |------>| VCO |--+------+->| /P |---------->
               +----+    +->|     |  |      |  +----+
                         |  +-----+  |      |
                         |           |      |  +----+   f_pll48ck
                         |  +----+   |      +->| /Q |----------->
                         +--| *N |<--+      |  +----+
                            +----+          |
                                            |  +----+    f_plldsi
                                            +->| /R |----------->
                                               +----+
        f_osc: HSI or HSE
        f_in      = f_osc / M (range:   1 to   2MHz, M =   2,..., 63)
        f_out     = f_in  * N (range: 192 to 432MHz, N = 192,...,432)
        f_pllclk  = f_out / P (range:  up to 168MHz, P = 2,4,6,8    )
        f_pll48ck = f_out / Q (equal to 48 MHz,      Q =   2,..., 15)

        Allowed frequencies due to f_pll48ck:
        f_out = 192, 240, 288, 336, 384, 432
            Q =   4    5    6    7    8    9
  */

  //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  // Enable HSE, HSI or MSI clock, set PLL
  //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  //-----------------------------------------------------------------
  /* This configuration:
       - f_pll48m1clk NOT used
       - f_osc = 4,...,48MHz
       - f_out = 400 MHz
  */

  // values used by compiler flag OSCSRC:
  #define HSE        1
  #define HSE_BYPASS 2
  #define HSI        3

  // set PLL input
  //
  //---- HSE --------------------------------------------------------
  #if OSCSRC == HSE || OSCSRC == HSE_BYPASS

      #if OSCSRC == HSE
        RCC->CR |= (RCC_CR_HSEON );
      #else
        RCC->CR |= (RCC_CR_HSEON | RCC_CR_HSEBYP);
      #endif
      const DWORD pll_Src = RCC_PLLCFGR_PLLSRC_HSE;
      while( !(RCC->CR & RCC_CR_HSERDY) ); // wait for HSE ready

  //--- HSI --------------------------------------------------------
  #elif OSCSRC == HSI

      RCC->CR |= RCC_CR_HSION;
      const DWORD pll_Src = RCC_PLLCFGR_PLLSRC_HSI;
      while( !(RCC->CR & RCC_CR_HSIRDY) ); // wait for HSI ready
      #undef  OSCFREQ
      #define OSCFREQ 16000


  //----------------------------------------------------------------
  #else
      #error "OSCSRC not defined or wrong"
  #endif
  //-----------------------------------------------------------------

  #undef HSE
  #undef HSE_BYPASS
  #undef HSI


  //! \todo Check code
  // Clock control register
  RCC->CR |= RCC_CR_HSION;   // Need HSION in ADC (only?)

  //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  //-----------------------------------------------------------------
  // calculate PLL
  // f_in  =   1 MHz => M = f_osc/f_in
  // f_out = 400 MHz => N = f_out/f_in

  const DWORD pll_M = (OSCFREQ)/1000UL;  // -> f_in      =     1 MHz
  const DWORD pll_N = 400;               // -> f_out     = 400*1 MHz

  //-----------------------------------------------------------------
  // calculate clock divider PLL
  const DWORD pll_P = 2;                 // -> f_pllclk  = 400/2 MHz = 200  MHz
  const DWORD pll_Q = 8;                 // -> f_pll48ck = 400/8 MHz =  50  MHz
  const DWORD pll_R = 7;                 // -> f_plldsi  = 400/7 MHz =  57.1 MHz

  // PLL configuration register
  RCC->PLLCFGR =   pll_Src
                 | ( pll_M           )
                 | ( pll_N      <<  6)
                 | ((pll_P/2-1) << 16)
                 | ( pll_Q      << 24)
                 | ( pll_R      << 28);

  // Clock control register
  RCC->CR |= RCC_CR_PLLON;            // PLL enable: ON
  while( !(RCC->CR & RCC_CR_PLLRDY) );// Wait until PLL is ready

  // Clock Configuration Register
  RCC->CFGR &= ~(RCC_CFGR_SW);          // reset system clock switch
  RCC->CFGR |=  (RCC_CFGR_SW_PLL);      // System clock switch:
                                        //  PLL selected as system clock

  // Wait for system clock switch is ready
  while( (RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL ) {asm("NOP");}
  //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

  //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  //-----------------------------------------------------------------
  // calculate PLLSAI
  // f_in  =   1 MHz
  // f_out = 192 MHz => N = f_out/f_in  {f_out=[100,...,432]MHz}
  const DWORD pllsai_N = 192; // {50,...,432}

  //-----------------------------------------------------------------
  // calculate clock divider PLLSAI
  const DWORD pllsai_P = 4; // {2,4,6,8}  -> f_pllsaip = 192/4 MHz = 48 MHz (USB)
  const DWORD pllsai_Q = 2; // {2,...,15} -> f_pllsaiq = 192/2 MHz = 96 MHz (default)
  const DWORD pllsai_R = 4; // {2,...,7}  -> f_pllsaip = 192/4 MHz = 48 MHz (LCD-TFT)

  RCC->CR &= ~RCC_CR_PLLSAION;         // PLLSAI disable   ???
  while( RCC->CR & RCC_CR_PLLSAIRDY ); // Wait until disabled ????

  RCC->PLLSAICFGR =    ( pllsai_N      <<  6)
                     | ((pllsai_P/2-1) << 16)
                     | ( pllsai_Q      << 24)
                     | ( pllsai_R      << 28);

  RCC->DCKCFGR1 = (RCC->DCKCFGR1 & ~RCC_DCKCFGR1_PLLSAIDIVR)  | 0/*RCC_PLLSAIDIVR_2*/;
  RCC->DCKCFGR2 |= RCC_DCKCFGR2_CK48MSEL; // PLLSAI is 48Mhz-USB clock source

  /* Enable PLLSAI Clock */
  RCC->CR |= RCC_CR_PLLSAION;
  while( !(RCC->CR & RCC_CR_PLLSAIRDY) );
  //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

  // System Control Block
  SCB->VTOR = FLASH_BASE | 0x00; // Vector Table Relocation in internal FLASH

  SCB_InvalidateDCache();
	SCB_DisableDCache();
}

//*******************************************************************
//
// System_GetDSIclock
//
//*******************************************************************
DWORD System_GetDSIclock( void )
{
  return( OSCFREQ );
}



//EOF
