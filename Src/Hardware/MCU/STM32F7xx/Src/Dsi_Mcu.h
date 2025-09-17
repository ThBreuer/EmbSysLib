//*******************************************************************
/*!
\file   Dsi_Mcu.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   16.10.2022

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_DSI_MCU_H
#define _HW_DSI_MCU_H

//*******************************************************************
#include "Std/Std.h"
#include "Ltdc_Mcu.h"
#include "Fmc_Mcu.h"

//*******************************************************************
DWORD System_GetDSIclock( void );

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class Dsi_Mcu

\brief ...

\remark DSI doesn't work with HSI
*/
class Dsi_Mcu
{
  public:
    //---------------------------------------------------------------
    Dsi_Mcu( uint32_t startAddr )

    : hwLTDC( startAddr, false )

    {
      // Enable clock and reset-cycle
      RCC->APB2ENR  |=  RCC_APB2ENR_DSIEN;
      RCC->APB2RSTR |=  RCC_APB2RSTR_DSIRST;
      RCC->APB2RSTR &= ~RCC_APB2RSTR_DSIRST;
    }

    //-----------------------------------------------------------------
    void Init( WORD Hsize, WORD Vsize, bool useLTDC )
    {
      DSI->WCR   &= ~DSI_WCR_DSIEN;   // Wrapper disable
      DSI->CR    &= ~DSI_CR_EN;       // DSI disable
      DSI->PCTLR &= ~DSI_PCTLR_CKE;   // D-PHY clock disable
      DSI->PCTLR &= ~DSI_PCTLR_DEN;   // Digital disable
      DSI->WRPCR &= ~DSI_WRPCR_PLLEN; // PLL disable
      DSI->WRPCR &= ~DSI_WRPCR_REGEN; // Regulator disable

      // Turn on the regulator and enable the DSI PLL
      //---------------------------------------------
      DSI->WRPCR |= DSI_WRPCR_REGEN; // Regulator Enable

      // TODO: timeout
      while( !(DSI->WISR & DSI_WISR_RRS) ) // Wait until the regulator is ready
      {}

      // Clear and set the PLL division factors
      // WRPCR -  Wrapper Regulator and PLL Control Register
      //
      // Calculation: (see: Reference Manual, DocID028270, page 664 and 737)
      //   F_VCO = (CLKIN / IDF) * 2 * NDIV,
            //   F_PHI = F_VCO /(2 * 2^ODF )
      // =>
      //   F_PHI = (_HSE_CLK / IDF) * NDIV /(2^ODF) * kHz
      //
      // here: _HSE_CLK = 25000, NDIV=100, IDF=5; ODF=0
      //       => F_PHI = 25E3/5*100/(2^0)*kHz = 500 MHz

      //System_GetDSIclock(); // get CLKIN
      //
      // NDIV = 10,...,125
      // IDF = 1,...,7
      // INFIN = CLKIN/IDF = 4,...,25MHz
      // Fvco = 500MHz - 1 GHz
      // ODF 2^0=1, 2^1=2, 2^2=4, 2^3=8
      // PHI 31,25 to 500MHz

      const DWORD pll_ODF  = 0;
      const DWORD pll_IDF  = 5;
      const DWORD pll_NDIV = RANGE( (DWORD)250000*pll_IDF/System_GetDSIclock(), (DWORD)10, (DWORD)125 );
      const double F_PHI   = 400E6; //500E6; /*Hz*/

      DSI->WRPCR &= ~( DSI_WRPCR_PLL_NDIV
                     | DSI_WRPCR_PLL_IDF
                     | DSI_WRPCR_PLL_ODF );
      DSI->WRPCR |=  ( (pll_NDIV <<  2)   // PLL N-Div
                     | (pll_IDF  << 11)   // PLL IDF
                     | (pll_ODF  << 16)); // PLL ODF

      DSI->WRPCR |= DSI_WRPCR_PLLEN; // PLL Enable

      // TODO: timeout
      while( !(DSI->WISR & DSI_WISR_PLLLS) ); // Wait for the lock of the PLL

      // Set Host PHY
      //-------------
      // D-PHY clock and digital lane
      DSI->PCTLR  = ( DSI_PCTLR_CKE    // Clock Enable
                    | DSI_PCTLR_DEN ); // Digital Enable

      // Clock Lane Configuration
      DSI->CLCR  =  ( DSI_CLCR_DPCC    // D-PHY Clock Control: High-Speed mode
                    |!DSI_CLCR_ACR );  // Automatic Clock lane Control: disabled

      // Host PHY Configuration Register
      DSI->PCONFR  =  ( (10 <<8 )      // Stop Wait Time: 10
                      | 0x01 );        // Number of Lanes: 2

      // Set Clock configuration
      //------------------------
      // Clock Control register
      DSI->CCR  =  ( (0 << 8)          // Timeout Clock Division
                   | (4 << 0) );       // TX Escape Clock Division

      //  Wrapper PHY Configuration Register 0
      DSI->WPCR[0] = (WORD)(1.0/(0.25E-9*F_PHI)); // Unit Interval multiplied by 4

       // Disable all error interrupts
      DSI->IER[0] = 0;
      DSI->IER[1] = 0;

      // Host mode Configuration Register
      DSI->MCR = DSI_MCR_CMDM; // Command mode: Command

      //  Wrapper     Configuration Register
      DSI->WCFGR  = ( DSI_WCFGR_DSIM    // Video transmission mode: Adapted Command mode
                    | !DSI_WCFGR_TESRC  // TE Source: DSI Link
                    | !DSI_WCFGR_TEPOL  // TE Polarity: rising edge
                    | !DSI_WCFGR_AR     // Automatic Refresh: disable
                    | !DSI_WCFGR_VSPOL  // VSync Polarity: fallig edge
                    | (0x00<<1) );      // Color multiplexing used by DSI Host: 24 bit

      // Host LTDC VCID Register
      DSI->LVCIDR = 0;                // Virtual Channel ID

      //  Host LTDC Polarity Configuration Register
      DSI->LPCR  =  (!DSI_LPCR_DEP    // Data Enable Polarity: active high
                    |!DSI_LPCR_VSP    // VSYNC Polarity: shutdown pin active high
                    |!DSI_LPCR_HSP ); // HSYNC Polarity: HSYNC pin active high

      // Host LTDC Color Coding Register
      DSI->LCOLCR = ( !DSI_LCOLCR_LPE  // Loosely Packet Enable: disable
                    | 0x00           );// Color Coding: 24 bit

      // Host LTDC Command Configuration Register
      DSI->LCCR = Hsize; // Command Size, max size for LTDC write memory command (pixel)

      // Host Command mode Configuration Register,
      // select High-speed or Low-power mode:
      // Enable Low-power command
      DSI->CMCR = ( DSI_CMCR_GSW0TX
                  | DSI_CMCR_GSW1TX
                  |  DSI_CMCR_GSW2TX
                  | DSI_CMCR_GSR0TX
                  | DSI_CMCR_GSR1TX
                  | DSI_CMCR_GSR2TX
                  | DSI_CMCR_GLWTX
                  | DSI_CMCR_GLWTX
                  | DSI_CMCR_DSW1TX
                  | DSI_CMCR_DSR0TX
                  | DSI_CMCR_DLWTX
                  | 0   // Maximum Read Packet Size: High-speed
                  | 0); // Acknowledge Request Enable: disable

      if( useLTDC )
      {
        hwLTDC.Init( Hsize, Vsize );
      }

      // DSI and wrapper enable
      DSI->CR  |= DSI_CR_EN;
      DSI->WCR |= DSI_WCR_DSIEN;

      // Configure DSI PHY HS2LP and LP2HS timings
      //------------------------------------------
      // Host Clock Lane Timer Configuration Register
      //
      // see: https://www.keil.com/boards2/stmicroelectronics/stm32f746g_discovery, Keil.STM32F7xx_DFP.2.9.0.pack
      //      "But the DSI Host is not calculating LP2HS_TIME + HS2LP_TIME but 2 x HS2LP_TIME.
      //        Workaround : Configure HS2LP_TIME and LP2HS_TIME with the same value being the max of HS2LP_TIME or LP2HS_TIME."
      DSI->CLTCR = ( (35 <<  0)    // Low-Power to High-Speed Time
                    | (35 << 16) ); // High-Speed to Low-Power Time

      //  Host Data Lane Timer Configuration Register
      DSI->DLTCR = ( ( 0 <<  0 )   // Maximum Read Time
                   | (35 << 16 )   // Low-Power To High-Speed Time
                   | (35 << 24 ) );// High-Speed To Low-Power Time

    } // end of init()

    //---------------------------------------------------------------
    uint32_t startLTDC( WORD Hsize, WORD Vsize )
    {
      // Host Command mode Configuration Register,
      // select High-speed or Low-power mode
      // Disable Low-power command
      DSI->CMCR = (!DSI_CMCR_GSW0TX
                  |!DSI_CMCR_GSW1TX
                  |!DSI_CMCR_GSW2TX
                  |!DSI_CMCR_GSR0TX
                  |!DSI_CMCR_GSR1TX
                  |!DSI_CMCR_GSR2TX
                  |!DSI_CMCR_GLWTX
                  |!DSI_CMCR_GLWTX
                  |!DSI_CMCR_DSW1TX
                  |!DSI_CMCR_DSR0TX
                  |!DSI_CMCR_DLWTX
                  | 0   // Maximum Read Packet Size: High-speed
                  | 0); // Acknowledge Request Enable: disable

      // Host Protocol Configuration Register
      DSI->PCR =  (!DSI_PCR_CRCRXE
                  |!DSI_PCR_ECCRXE
                  | DSI_PCR_BTAE
                  |!DSI_PCR_ETRXE
                  |!DSI_PCR_ETTXE );

      DisplayRefresh();

      return( hwLTDC.LayerConfig(Hsize, Vsize) );
    }

    //---------------------------------------------------------------
    virtual void writeShort( uint8_t para0, uint8_t para1 )
    {
      // TODO: timeout
      while( !(DSI->GPSR & DSI_GPSR_CMDFE) ); // Wait for Command FIFO Empty

      // Host Generic Header Configuration Register
      DSI->GHCR = ( 0x15                      // data type: DCS Write Short Packet 1 Parameters
                  | (              0 <<  6)   // channel ID
                  | ((uint32_t)para0 <<  8)   // data
                  | ((uint32_t)para1 << 16) );// data
    }

    //---------------------------------------------------------------
    virtual void writeLong( uint8_t*para, uint32_t N )
    {
      uint32_t n = 0;

      // TODO: timeout
      while( !(DSI->GPSR & DSI_GPSR_CMDFE) ); // Wait for Command FIFO Empty

      // TODO: Fehler,wenn N nicht durch 4 teilbar !!! Scheint aber nichts auszumachen ....

      // Set the DCS code hexadecimal on payload byte 1, and the other parameters on the write FIFO command
      while(n < N)
      {
        DSI->GPDR = (  (uint32_t)(*(para + n  ))
                    | ((uint32_t)(*(para + n+1)) <<  8)
                    | ((uint32_t)(*(para + n+2)) << 16)
                    | ((uint32_t)(*(para + n+3)) << 24) );
        n += 4;
      }

      // Host Generic Header Configuration Register
      DSI->GHCR = ( 0x39                 // data type: DCS Write Long Packet
                  | ( 0         << 6)    // Virtual channel ID
                  | ((N&0xFFFF) << 8) ); // Word Count MSB+LSB
    }

    //---------------------------------------------------------------
    void DisplayRefresh()
    {
      DSI->WCR |= DSI_WCR_LTDCEN;
    }

  private:
    //---------------------------------------------------------------
    Ltdc_Mcu hwLTDC;

}; //class Dsi_Mcu

}  } //namespace

#endif
