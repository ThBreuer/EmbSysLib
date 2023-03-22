//*******************************************************************
/*!
\file   Fsmc_Mcu.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   17.01.2023

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_FSMC_MCU_H
#define _HW_FSMC_MCU_H

//*******************************************************************
#include "Mcu_Types.h"
#include "Port_Mcu.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*! 
\class Fsmc_Mcu

\brief Implementation of Flexible static memory controller (FSMC)
  
\remark NOR/PSRAM only

\todo Idee: FSMC erbt von 'Bus', Display o.ä. verwenden dann 'Bus' ExtSRAM MemoryController?
*/
class Fsmc_Mcu
{
  public:
    //---------------------------------------------------------------
    /*! Initialize the FSCM hardware
        \param bankId memory bank ID
    */
    Fsmc_Mcu( BYTE bankId )
    { 
      const WORD mode =  PinConfig::FAST_SPEED
                       | PinConfig::PUSH_PULL
                       | PinConfig::NO_PUPD;
          
      PinConfig::set( PinConfig::FSMC_D0,  mode );  
      PinConfig::set( PinConfig::FSMC_D1,  mode );  
      PinConfig::set( PinConfig::FSMC_D2,  mode );  
      PinConfig::set( PinConfig::FSMC_D3,  mode );  
      PinConfig::set( PinConfig::FSMC_D4,  mode );  
      PinConfig::set( PinConfig::FSMC_D5,  mode );  
      PinConfig::set( PinConfig::FSMC_D6,  mode );  
      PinConfig::set( PinConfig::FSMC_D7,  mode );  
      PinConfig::set( PinConfig::FSMC_D8,  mode );  
      PinConfig::set( PinConfig::FSMC_D9,  mode );  
      PinConfig::set( PinConfig::FSMC_D10, mode );  
      PinConfig::set( PinConfig::FSMC_D11, mode );  
      PinConfig::set( PinConfig::FSMC_D12, mode );  
      PinConfig::set( PinConfig::FSMC_D13, mode );  
      PinConfig::set( PinConfig::FSMC_D14, mode );  
      PinConfig::set( PinConfig::FSMC_D15, mode );  

      PinConfig::set( PinConfig::FSMC_NOE, mode ); 
      PinConfig::set( PinConfig::FSMC_NWE, mode ); 
      PinConfig::set( PinConfig::FSMC_A19, mode ); 
      PinConfig::set( PinConfig::FSMC_NCE2, mode ); 
      
      bankId = bankId << 1;
      
      RCC->AHB3ENR |= RCC_AHB3ENR_FSMCEN; // Enable FSMC clock
      
      // Bank1 NOR/SRAM control register configuration
      FSMC_Bank1->BTCR[bankId] 
        = (uint32_t) ( 0 <<  1 ) // data adress multiplex: disable
                    |( 0 <<  2 ) // memory type: SRAM
                    |( 1 <<  4 ) // data width: 16 bit
                    |( 0 <<  8 ) // burst access mode: disable
                    |( 0 <<  9 ) // wait signal priority: low
                    |( 0 << 10 ) // wrap mode: disable
                    |( 0 << 11 ) // wait signal active: before wait state
                    |( 1 << 12 ) // write operation: 
                    |( 0 << 13 ) // wait signal: disable
                    |( 0 << 14 ) // extended mode: disable
                    |( 0 << 15 ) // asynchronous wait: disable
                    |( 0 << 19 );// write burst: disable
      
      // Bank1 NOR/SRAM timing register configuration
      FSMC_Bank1->BTCR[bankId+1] 
        = (uint32_t) ( 1 <<  0 ) // AddressSetupTime 
                    |( 0 <<  4 ) // AddressHoldTime
                    |( 9 <<  8 ) // DataSetupTime 
                    |( 0 << 16 ) // BusTurnAroundDuration
                    |( 0 << 20 ) // CLKDivision
                    |( 0 << 24 ) // DataLatency
                    |( 0 << 28 );// AccessMode: A
                
      FSMC_Bank1E->BWTR[bankId] = 0x0FFFFFFF; // no extended mode
      
      FSMC_Bank1->BTCR[bankId] |= ( 1 << 0 ); // memory bank enable: on
    }
    
    //---------------------------------------------------------------
    /*! Get base address
        \return base address
    */
    BYTE * getPtr( void )
    {
      return( (BYTE*)0x60000000 ); // Base address Bank1, NOR/PSRAM
    }

}; //class Fsmc_Mcu

}  } //namespace

#endif
