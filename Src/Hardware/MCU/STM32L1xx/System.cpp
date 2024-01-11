//*******************************************************************
/*!
\file   System.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

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
  __set_PRIMASK(1); // __disable_irq();
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
     __set_PRIMASK(0); //__enable_irq();
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
  __disable_irq();
  enableWatchdog( 1 );

  while(1);
}

#pragma GCC push_options
#pragma GCC optimize ("-O0")

//-------------------------------------------------------------------
void System::delayMicroSec( unsigned short delay )
{
  // Calibrated with System::delayMilliSec( 1000 ), f_clk = 32 MHz, no interrupts
  unsigned steps = 0.1420E-6*system_PCLK*delay;
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
  // Clock control register
  RCC->CR   |=      RCC_CR_MSION;     // MSI clock enable: MSI oscillator ON

  // Clock configuration register
  RCC->CFGR &= ~(   RCC_CFGR_MCOPRE   // reset ...
                  | RCC_CFGR_MCOSEL
                  | RCC_CFGR_PPRE1
                  | RCC_CFGR_PPRE2
                  | RCC_CFGR_HPRE
                  | RCC_CFGR_SW );

  // Clock control register
  RCC->CR   &= ~(   RCC_CR_HSION      // reset ...
                  | RCC_CR_HSEON
                  | RCC_CR_PLLON
                  | RCC_CR_CSSON );
  RCC->CR   &= ~(   RCC_CR_HSEBYP );  // reset ...

  // Clock configuration register
  RCC->CFGR &= ~(   RCC_CFGR_PLLMUL   // reset ...
                  | RCC_CFGR_PLLDIV
                  | RCC_CFGR_PLLSRC );

  // Clock interrupt register
  RCC->CIR   =  0;                    // Disable all interrupts

  // Flash Acess Control Register
  FLASH->ACR |= FLASH_ACR_ACC64;      // 64-bit access: enable
  FLASH->ACR |= FLASH_ACR_PRFTEN;     // Prefetch: enable
  FLASH->ACR |= FLASH_ACR_LATENCY;    // Latency: one wait state

  // Advanced Peripheral Bus Enable Register
  RCC->APB1ENR |= RCC_APB1ENR_PWREN;  // Power interface clock: enable

  // Power Control Register
  PWR->CR = PWR_CR_VOS_0;             // Voltage scaling range selection: range 1 (1.8 V)
  while( PWR->CSR & PWR_CSR_VOSF )    // Wait until voltage regulator is ready
  {
    asm volatile("nop");
  }

  // Clock Configuration Register
  RCC->CFGR |= RCC_CFGR_HPRE_DIV1;    // AHB prescaler:                   HCLK  = SYSCLK
  RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;   // APB high-speed prescaler (APB2): PCLK2 = HCLK
  RCC->CFGR |= RCC_CFGR_PPRE1_DIV1;   // APB low-speed  prescaler (APB1): PCLK1 = HCLK

  //-----------------------------------------------------------------
  /* Clock configuration
  
    //---------------------------------------------------------------
    SYSCLK = 32MHz, using PLL
     
    Settings by compiler flags:
     
      compiler flag     | value
      ------------------+----------------+-------
      OSCSRC            | HSE,           | HSI   
      oscillator source | HSE_BYPASS     |
      ------------------+----------------+-------
      OSCFREQ           | 4000,...,48000 | ---   
      f_osc/kHz         |                |       
      ------------------+----------------+-------


    //---------------------------------------------------------------
    PLL configuration
    
       f_vco = f_osc * N

       f_osc  +-----+ f_vco        +------+   f_pllclk
       ------>| VCO |----+------+->| /DIV |---------->
           +->|     |    |      |  +------+
           |  +-----+    |      |
           |             |      |  +------+   f_pllvco2
           |  +------+   |      +->| /2   |---------->
           +--| *MUL |<--+         +------+
              +------+           

        f_osc: HSI16 or HSE         (min: 2 MHz)
        f_vco         = f_in  * MUL (max:  96 MHz, MUL = 3,4,6,8,12,16,24,32,48)
        f_pllclk      = f_out / DIV (max:  80 MHz, DIV = 2,3,4)

        Allowed frequencies due to USB:
        f_vco = 96 MHz
  */

  //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  // Enable HSE or HSI clock, set PLL
  //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  //-----------------------------------------------------------------
  /* This configuration:
     f_vco = 96 MHz (wg. USBCLK = 48MHz = 96MHz/2)
 
       f_osc (MHz) =  32  24  16  12   8   6   4   3   2
       MUL         =   3   4   6   8  12  16  24  32  48
       
       f_pllclk = 32MHz = f_vco/DIV => DIV = 3
  */

  // values used by compiler flag OSCSRC:
  #define HSE        1
  #define HSE_BYPASS 2
  #define HSI        3

  // set PLL input
  //
  //---- HSE --------------------------------------------------------
  #if  OSCSRC == HSE || OSCSRC == HSE_BYPASS

      #if OSCSRC == HSE 
        RCC->CR |= (RCC_CR_HSEON );
      #else
        RCC->CR |= (RCC_CR_HSEON | RCC_CR_HSEBYP);
      #endif
      const DWORD pll_Src = RCC_CFGR_PLLSRC_HSE;
      while( !(RCC->CR & RCC_CR_HSERDY) ) // wait for HSE ready
      {
        asm volatile("nop");
      }

  //--- HSI --------------------------------------------------------
  #elif OSCSRC == HSI

      RCC->CR |= RCC_CR_HSION;
      const DWORD pll_Src = RCC_CFGR_PLLSRC_HSI;
      while( !(RCC->CR & RCC_CR_HSIRDY) ) // wait for HSI ready
      {
        asm volatile("nop");
      }

      #undef  OSCFREQ
      #define OSCFREQ 16000

  //----------------------------------------------------------------
  #else
      #error "OSCSRC not defined or wrong"
  #endif
  //----------------------------------------------------------------

  #undef HSE
  #undef HSE_BYPASS
  #undef HSI
  #undef MSI

  // calculate PLL
  #if OSCFREQ == 4000
    const DWORD pll_Mul = RCC_CFGR_PLLMUL24;
  #elif OSCFREQ == 6000
    const DWORD pll_Mul = RCC_CFGR_PLLMUL16;
  #elif OSCFREQ == 8000
    const DWORD pll_Mul = RCC_CFGR_PLLMUL12;
  #elif OSCFREQ == 12000
    const DWORD pll_Mul = RCC_CFGR_PLLMUL8;
  #elif OSCFREQ == 16000
    const DWORD pll_Mul = RCC_CFGR_PLLMUL6;
  #elif OSCFREQ == 24000
    const DWORD pll_Mul = RCC_CFGR_PLLMUL4;
  #elif OSCFREQ == 32000
    const DWORD pll_Mul = RCC_CFGR_PLLMUL3;
  #else
    #error "OSCFREQ value not supported"
  #endif

  // PLL configuration register
  RCC->CFGR |= ( pll_Src 
               | pll_Mul 
               | RCC_CFGR_PLLDIV3 );

  // Clock control register
  RCC->CR |= RCC_CR_PLLON;            // PLL enable: ON
  while( !(RCC->CR & RCC_CR_PLLRDY) )// Wait until PLL is ready
  {
    asm volatile("nop");
  }

  // Clock Configuration Register
  RCC->CFGR |= RCC_CFGR_SW_PLL;       // System clock switch: PLL used as system clock

  // Wait for system clock switch is ready
  while( (RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL )
  {
    asm volatile("nop");
  }


  //<<< Enable HSE or clock, set PLL <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  // System Control Block
  SCB->VTOR = FLASH_BASE | 0x00; // Vector Table Relocation in Internal FLASH
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
