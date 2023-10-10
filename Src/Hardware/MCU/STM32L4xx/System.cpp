//*******************************************************************
/*!
\file   System.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   16.10.2022

License: See file "LICENSE"

\brief  Controller specific implementation of class cSystem

\todo Check Clock Tree
\todo Check Interrupt enable/disable
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
  NVIC_SystemReset();
}

#pragma GCC push_options
#pragma GCC optimize ("-O0")
//-------------------------------------------------------------------
void System::delayMicroSec( unsigned short delay )
{
  // Calibrated with System::delayMilliSec( 1000 ), f_clk = 80 MHz, no interrupts

  unsigned steps = 0.25E-6*system_PCLK*delay;
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

  // Clock control register
//  RCC->CR |= RCC_CR_MSION;  //! \todo why is it switched on here?

  // Clock configuration register
  RCC->CFGR = 0x00000000;       // reset ...

  // Clock control register
  RCC->CR &= ~(   RCC_CR_PLLON  // reset ...
                | RCC_CR_CSSON
                | RCC_CR_HSEON
                | RCC_CR_HSION );

  // PLL configuration register
  RCC->PLLCFGR = 0x00001000;   // Reset value,
                               // see Reference manual

  // Clock control register
  RCC->CR &= ~(RCC_CR_HSEBYP); // reset ...

  // Clock interrupt register
  RCC->CIER = 0;                // Disable all interrupts

  // Flash Acess Control Register
  FLASH->ACR =   FLASH_ACR_ICEN          // Instruction cache enable
               | FLASH_ACR_DCEN          // Data cache enable
               | FLASH_ACR_LATENCY_4WS;  // Latency: 4 wait states

  // Advanced Peripheral Bus Enable Register
  RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN;    // Power interface clock: enable

  // Clock Configuration Register
  //! \todo Check clock configuration
  RCC->CFGR |=   RCC_CFGR_HPRE_DIV1     // AHB prescaler:                   HCLK  = SYSCLK
               | RCC_CFGR_PPRE2_DIV1    // APB high-speed prescaler (APB2): PCLK2 = HCLK/1
                                        //   PCLK2 must not exceed 84 MHz
               | RCC_CFGR_PPRE1_DIV1;   // APB low-speed  prescaler (APB1): PCLK1 = HCLK/1
                                        //   PCLK1 must not exceed 42 MHz
  //-----------------------------------------------------------------
  /* Clock configuration
  
    //---------------------------------------------------------------
    SYSCLK = 80MHz, using PLL
     
    Settings by compiler flags:
     
      compiler flag     | value
      ------------------+------------------+-------+-----------------
      OSCSRC            | HSE,             | HSI   | MSI                   
      oscillator source | HSE_BYPASS       |       |                       
      ------------------+------------------+-------+-----------------
      OSCFREQ           | 4000,8000,12000, | ---   | 4000,8000,16000,      
      f_osc/kHz         | 16000,24000,32000|       | 24000, 32000
                        | or 48000         |       | or 48000 
      ------------------+------------------+-------+------------------

    //---------------------------------------------------------------
    PLL configuration
    
       f_out = f_osc * N /M

       f_osc  +----+ f_in  +-----+ f_out      +----+   f_pllsai1clk
       ------>| /M |------>| VCO |--+------+->| /P |---------->
              +----+    +->|     |  |      |  +----+
                        |  +-----+  |      |
                        |           |      |  +----+   f_pll48m1clk
                        |  +----+   |      +->| /Q |----------->
                        +--| *N |<--+      |  +----+
                           +----+          |
                                           |  +----+   f_pllclk
                                           +->| /R |----------->
                                              +----+
        f_osc: HSI16 or HSE or MSI
        f_in          = f_osc / M (range:   4 to  16MHz, M = 1,...,8)
        f_out         = f_in  * N (range:  64 to 344MHz, N = 8,...,86)
        f_pllsai1clk  = f_out / P (range:  up to  80MHz, P = 2,3,..,31)
        f_pll48m1clk  = f_out / Q (equal to 48 MHz,      Q = 2,4,6,8)
        f_pllclk      = f_out / R (range: up to 80 MHz,  R = 2,4,6,8)

        Allowed frequencies due to f_pll48ck:
        f_out = 96, 192, 288
            Q =  2    4    6  
  */

  //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  // Enable HSE, HSI or MSI clock, set PLL
  //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  //-----------------------------------------------------------------
  /* This configuration:
       - f_pll48m1clk NOT used
       - f_osc = 4,...,48MHz
       - f_out = 160 MHz
  */

  // values used by compiler flag OSCSRC:
  #define HSE        1
  #define HSE_BYPASS 2
  #define HSI        3
  #define MSI        4
  
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

  //--- MSI --------------------------------------------------------
  #elif  OSCSRC == MSI

      // set MSI frequency
      RCC->CR &= ~RCC_CR_MSIRANGE_Msk;
      #if OSCFREQ == 4000                   
        RCC->CR |= RCC_CR_MSIRGSEL|RCC_CR_MSIRANGE_6;
      #elif OSCFREQ == 8000
        RCC->CR |= RCC_CR_MSIRGSEL|RCC_CR_MSIRANGE_7;
      #elif OSCFREQ == 16000
        RCC->CR |= RCC_CR_MSIRGSEL|RCC_CR_MSIRANGE_8;
      #elif OSCFREQ == 24000
        RCC->CR |= RCC_CR_MSIRGSEL|RCC_CR_MSIRANGE_9;
      #elif OSCFREQ == 32000
        RCC->CR |= RCC_CR_MSIRGSEL|RCC_CR_MSIRANGE_10;
      #elif OSCFREQ == 48000
        RCC->CR |= RCC_CR_MSIRGSEL|RCC_CR_MSIRANGE_11;
      #else
        #error "OSCFREQ value not supported"
      #endif

      RCC->CR |= RCC_CR_MSION;
      const DWORD pll_Src = RCC_PLLCFGR_PLLSRC_MSI;
      while( !(RCC->CR & RCC_CR_MSIRDY) ); // wait for MSI ready

  //----------------------------------------------------------------
  #else
      #error "OSCSRC not defined or wrong"
  #endif
  //----------------------------------------------------------------

  #undef HSE
  #undef HSE_BYPASS
  #undef HSI
  #undef MSI

  //-----------------------------------------------------------------
  // calculate PLL
                                       //  f_osc /M = f_in *N  = f_out           
  #if OSCFREQ == 4000                  //-----------------------------
    const DWORD pll_M = 1, pll_N = 40; //    4   /1  =  4  *40 =  160
  #elif OSCFREQ == 8000
    const DWORD pll_M = 2, pll_N = 40; //    8   /2  =  8  *40 =  160
  #elif OSCFREQ == 12000
    const DWORD pll_M = 3, pll_N = 40; //   12   /3  =  4  *40 =  160
  #elif OSCFREQ == 16000
    const DWORD pll_M = 2, pll_N = 20; //   16   /2  =  8  *20 =  160
  #elif OSCFREQ == 24000
    const DWORD pll_M = 3, pll_N = 20; //   24   /3  =  8  *20 =  160
  #elif OSCFREQ == 32000
    const DWORD pll_M = 4, pll_N = 20; //   32   /4  =  8  *20 =  160
  #elif OSCFREQ == 48000
    const DWORD pll_M = 6, pll_N = 20; //   48   /6  =  8  *20 =  160
  #else
    #error "OSCFREQ value not supported"
  #endif

  //-----------------------------------------------------------------
  // calculate clock divider
  const DWORD pll_P = 2; // f_plsai1clk = f_out/2 = 80 MHz
  const DWORD pll_R = 2; // f_pllclk    = f_out/2 = 80 MHz

  // PLL configuration register
  RCC->PLLCFGR =   pll_Src
                 | RCC_PLLCFGR_PLLPEN
                 | RCC_PLLCFGR_PLLREN
                 | ( (pll_M-1)      <<  4 )
                 | ( (pll_N  )      <<  8 )
                 | ( (pll_R/2-1)    << 25 )
                 | ( (pll_P)        << 27 );

  // Clock control register
  RCC->CR |= RCC_CR_PLLON;            // PLL enable: ON
  while( !(RCC->CR & RCC_CR_PLLRDY) );// Wait until PLL is ready

  // Clock Configuration Register
  RCC->CFGR &= ~RCC_CFGR_SW;          // reset system clock switch
  RCC->CFGR |=  RCC_CFGR_SW_PLL;      // System clock switch:
                                      //  PLL selected as system clock

  // Wait for system clock switch is ready
  while( (RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL );

  //<<< Enable HSE, HSI or MSI clock, set PLL <<<<<<<<<<<<<<<<<<<<<<<

  // System Control Block
  SCB->VTOR = FLASH_BASE | 0x00; // Vector Table Relocation in internal FLASH
}

//*******************************************************************
//
// SystemInit_CLK48
//
//*******************************************************************
void SystemInit_48MHzClock( void )
{
  // Switch the voltage reference for the HSI48 oscillator on
  RCC->APB2ENR  |= RCC_APB2ENR_SYSCFGEN;
  RCC->APB1ENR1  |= RCC_APB1ENR1_CRSEN;
  
  // configure CRS
  // RELOAD: use reset value (f_TARGET=48MHz, f_SYNC = 1 kHz, SOF signal from USB)
  // FELIM: use reset value
  CRS->CFGR |= (2<<28);
  CRS->CR |=   CRS_CR_CEN 
             | CRS_CR_AUTOTRIMEN;

  // Switch the HSI48 oscillator on
  RCC->CRRCR |= RCC_CRRCR_HSI48ON;

  // Wait until the HSI48 oscillator is ready
  while( !(RCC->CRRCR & RCC_CRRCR_HSI48RDY) );

  // Select the HSI48 oscillator as clock source for USB
  RCC->CCIPR |= (0<<26);
}

//*******************************************************************
extern "C"
{
  void __cxa_pure_virtual( void )
  {
    abort();
  }
}

//EOF
