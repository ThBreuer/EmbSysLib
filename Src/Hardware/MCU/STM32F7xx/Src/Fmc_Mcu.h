//*******************************************************************
/*!
\file   Fmc_Mcu.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   16.10.2022

License: See file "LICENSE"
*/

// see: www.element14.com/community, STM: Software examples for STM32F4xx, STM32F4xx_LCD_Example.zip

//*******************************************************************
#ifndef _HW_FMC_MCU_H
#define _HW_FMC_MCU_H

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class Fmc_Mcu

\brief ...
*/
class Fmc_Mcu
{
  //DMA xdma;
  public:
    typedef enum
    {
      NOR         = 0x00,
      NAND        = 0x01,
      SDRAM       = 0x02,
      SDRAM_Bank1 = (0<<4) | SDRAM,
      SDRAM_Bank2 = (1<<4) | SDRAM,
      SDRAM_Both  = (2<<4) | SDRAM
    } FMCtype;

    typedef enum
    {
      DATA_BUS_WIDTH_16BIT      = 0x00,
      DATA_BUS_WIDTH_32BIT      = 0x01

    } BusConfig_type;

  public:
    //---------------------------------------------------------------
    /*! Initialize the FSCM hardware
        \param xxx  xxx
    */
    Fmc_Mcu( FMCtype typeIn, BusConfig_type busConfigIn )
    {
      type      = typeIn&0x0F;
      bank      = (typeIn>>4)&0x01;
      both      = ((typeIn>>4)&0x02)?true:false;

      busConfig = busConfigIn;

      WORD mode =  PinConfig::FAST_SPEED
                 | PinConfig::PUSH_PULL
                 | PinConfig::PULL_UP // ???
                 | PinConfig::NO_PUPD;

      PinConfig::set(PinConfig::FMC_D0,mode);
      PinConfig::set(PinConfig::FMC_D1,mode);
      PinConfig::set(PinConfig::FMC_D2,mode);
      PinConfig::set(PinConfig::FMC_D3,mode);
      PinConfig::set(PinConfig::FMC_D4,mode);
      PinConfig::set(PinConfig::FMC_D5,mode);
      PinConfig::set(PinConfig::FMC_D6,mode);
      PinConfig::set(PinConfig::FMC_D7,mode);
      PinConfig::set(PinConfig::FMC_D8,mode);
      PinConfig::set(PinConfig::FMC_D9,mode);
      PinConfig::set(PinConfig::FMC_D10,mode);
      PinConfig::set(PinConfig::FMC_D11,mode);
      PinConfig::set(PinConfig::FMC_D12,mode);
      PinConfig::set(PinConfig::FMC_D13,mode);
      PinConfig::set(PinConfig::FMC_D14,mode);
      PinConfig::set(PinConfig::FMC_D15,mode);
      if( busConfig & DATA_BUS_WIDTH_32BIT )
      {
        PinConfig::set(PinConfig::FMC_D16,mode);
        PinConfig::set(PinConfig::FMC_D17,mode);
        PinConfig::set(PinConfig::FMC_D18,mode);
        PinConfig::set(PinConfig::FMC_D19,mode);
        PinConfig::set(PinConfig::FMC_D20,mode);
        PinConfig::set(PinConfig::FMC_D21,mode);
        PinConfig::set(PinConfig::FMC_D22,mode);
        PinConfig::set(PinConfig::FMC_D23,mode);
        PinConfig::set(PinConfig::FMC_D24,mode);
        PinConfig::set(PinConfig::FMC_D25,mode);
        PinConfig::set(PinConfig::FMC_D26,mode);
        PinConfig::set(PinConfig::FMC_D27,mode);
        PinConfig::set(PinConfig::FMC_D28,mode);
        PinConfig::set(PinConfig::FMC_D29,mode);
        PinConfig::set(PinConfig::FMC_D30,mode);
        PinConfig::set(PinConfig::FMC_D31,mode);
      }
      PinConfig::set(PinConfig::FMC_NBL0,mode);
      PinConfig::set(PinConfig::FMC_NBL1,mode);

      PinConfig::set(PinConfig::FMC_A0,mode);
      PinConfig::set(PinConfig::FMC_A1,mode);
      PinConfig::set(PinConfig::FMC_A2,mode);
      PinConfig::set(PinConfig::FMC_A3,mode);
      PinConfig::set(PinConfig::FMC_A4,mode);
      PinConfig::set(PinConfig::FMC_A5,mode);
      PinConfig::set(PinConfig::FMC_A6,mode);
      PinConfig::set(PinConfig::FMC_A7,mode);
      PinConfig::set(PinConfig::FMC_A8,mode);
      PinConfig::set(PinConfig::FMC_A9,mode);
      PinConfig::set(PinConfig::FMC_A10,mode);
      PinConfig::set(PinConfig::FMC_A11,mode);

      // \todo this does not depend on data bus width, use other flag!
      if( busConfig & DATA_BUS_WIDTH_32BIT )
      {
        PinConfig::set(PinConfig::FMC_A12,mode);
        PinConfig::set(PinConfig::FMC_A13,mode);
        PinConfig::set(PinConfig::FMC_A14,mode);
        PinConfig::set(PinConfig::FMC_A15,mode);
      }
      PinConfig::set(PinConfig::FMC_SDCLK,mode);
      PinConfig::set(PinConfig::FMC_SDNCAS,mode);

      PinConfig::set(PinConfig::FMC_SDNRAS,mode);

      PinConfig::set(PinConfig::FMC_SDCKE0,mode);
      PinConfig::set(PinConfig::FMC_SDNE0,mode);
      PinConfig::set(PinConfig::FMC_SDNWE,mode);

      // \todo this does not depend on data bus width, use other flag!
      if( busConfig & DATA_BUS_WIDTH_32BIT )
      {
        PinConfig::set(PinConfig::FMC_NBL2,mode);
        PinConfig::set(PinConfig::FMC_NBL3,mode);
      }

      /* Enable FMC clock */
      RCC->AHB3ENR |= RCC_AHB3ENR_FMCEN;

      if( type == SDRAM )
      {
        SDRAM_Init( );
      }
    }

    //---------------------------------------------------------------
    uint32_t startAddr( void )
    {
      if( bank == 0 )
        return( 0xC0000000 );
      else
        return( 0xD0000000 );
    }

  private:
    //---------------------------------------------------------------
    void SDRAM_Init(  )
    {
      uint32_t reg  = 0;

      // Step 1 and 2
      // Common initialization,
      // see Reference Manual, DocID028270-Rev 3, page 388
      //--------------------------------------------------
      // SDRAM Control registers
      reg  = FMC_Bank5_6->SDCR[0/*BANK1*/];
      reg &= ~( FMC_SDCR1_SDCLK
              | FMC_SDCR1_RBURST
              | FMC_SDCR1_RPIPE );
      reg |=  (  (2<<10)          // Clock configuration: 2xHCLK periods
              |  FMC_SDCR1_RBURST // Burst read:enable
              |  (0<<13) );       // Read pipe: No HCLK clock cycle delay
      FMC_Bank5_6->SDCR[0/*BANK1*/] = reg;

      // SDRAM Timing registers
      reg  = FMC_Bank5_6->SDTR[0/*BANK1*/];
      reg &= ~( FMC_SDTR1_TRC
              | FMC_SDTR1_TRP );
      reg |=  ( ((7 - 1) << 12)    // Row cycle delay: 7 cycles
              | ((2 - 1) << 20) ); // Row precharge delay: 2 cycles
      FMC_Bank5_6->SDTR[0/*BANK1*/] = reg;

      // Bank specific initialization
      //-----------------------------
      // SDRAM Control registers
      reg  = FMC_Bank5_6->SDCR[bank];
      reg &= ~( FMC_SDCR1_NC
              | FMC_SDCR1_NR
              | FMC_SDCR1_MWID
              | FMC_SDCR1_NB
              | FMC_SDCR1_CAS
              | FMC_SDCR1_WP );
      reg |=  ( (0 <<  0)       // Number of column address bits:8
              | (1 <<  2)       // Number of row address bits:12
              | (((busConfig & DATA_BUS_WIDTH_32BIT)?2:1) <<  4)       // Memory data bus width
              | (1 <<  6)       // Number of internal banks:4
              | (3 <<  7)       // CAS Latency:3 (must be same as in Load Mode Register)
              |!FMC_SDCR1_WP ); // Write protection: disable
      FMC_Bank5_6->SDCR[bank] = reg;

      // SDRAM Timing registers
      reg  = FMC_Bank5_6->SDTR[bank];
      reg &= ~( FMC_SDTR1_TMRD
              | FMC_SDTR1_TXSR
              | FMC_SDTR1_TRAS
              | FMC_SDTR1_TWR
              | FMC_SDTR1_TRCD );
      reg |=  ( ((2 - 1) <<  0)   // Load Mode Register to Active: 2 cycles
              | ((7 - 1) <<  4)   // Exit Self-refresh delay: 7 cycles
              | ((4 - 1) <<  8)   // Self refresh time: 4 cycles
              | ((2 - 1) << 16)   // Recovery delay: 2 cycles
              | ((2 - 1) << 24)); // Row to column delay: 2 cycles
      FMC_Bank5_6->SDTR[bank] = reg;


      // Device specific initialization
      //-------------------------------

      // Step 3: Configure a clock configuration enable command
      SendCommand( 0x01/*Clock Configuration Enable*/, 1, 0 );

      // Step 4: Insert 100 us minimum delay
      System::delayMilliSec(1);

      // Step 5: Configure a PALL (precharge all) command
      SendCommand( 0x02/*PALL Command*/, 1, 0 );

      // Step 6: Configure an Auto Refresh command
      SendCommand( 0x03/*Auto Refresh Command*/, 8, 0 );

      // Step 7: Program the external memory mode register
      // see: Micron SDR SDRAM "MT48LC4M32B2", Datasheet
      reg = ( (0<<0)   // Burst length: 1
            | (0<<3)   // Burst type:sequential
            | (3<<4)   // CAS Latency:3 (must be same as in SDCR)
            | (0<<7)   // Operating Mode: standard
            | (1<<9) );// Write Burst Mode: single
      SendCommand(0x04/*Load Mode Register*/, 1, reg);

      // Step 8: Set the refresh rate counter
      // see: Micron SDR SDRAM "MT48LC4M32B2", Datasheet:
      //      Refresh period: 64ms
      //      Refresh count:  4k = 4096
      //
      // here:fclkSD = 100 MHz
      //      COUNT = RefreshPeriod/RefreshCount*fclkSD + 20
      //            = 64E-3/4096*100E6 +20 = 1583
      FMC_Bank5_6->SDRTR |= (1583 << 1);
    }

  private:
    //---------------------------------------------------------------
    void SendCommand( uint32_t Command,       //!< Command issued to the SDRAM device
                      uint32_t AutoRefresh,   //!< Number of consecutive auto refresh
                                              //   command issued in auto refresh mode
                      uint32_t ModeRegister ) //!< SDRAM Mode register content
    {
      uint32_t ctb;

      // see: Reference Manual, DocID028270-Rev 3, page 402:
      //      "If two SDRAM banks are used, the Auto-refresh and
      //       PALL command must be issued simultaneously to the two devices
      //       with CTB1 and CTB2 bits set"
      if( both && (   Command == 0x02    // PALL Command
                   || Command == 0x03 )) // Auto Refresh Mode
        ctb = 3; // use CTB1 and CTB2 simultaneously
      else if( bank == 0)
        ctb = 1<<1; // CTB1
      else
        ctb = 1<<0; // CTB2

      FMC_Bank5_6->SDCMR = ( (Command         << 0)
                           | (ctb             << 3)
                           | ((AutoRefresh-1) << 5)
                           | (ModeRegister    << 9) );
      // TODO: timeout
      while( FMC_Bank5_6->SDSR & FMC_SDSR_BUSY ); // wait until command is send
    }

  private:
    //---------------------------------------------------------------
    uint8_t         type;
    uint8_t         bank;
    bool            both;
    BusConfig_type  busConfig;

}; //class Fmc_Mcu

}  } //namespace

#endif
