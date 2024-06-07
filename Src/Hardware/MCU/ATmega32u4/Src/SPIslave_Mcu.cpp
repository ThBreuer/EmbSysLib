//*******************************************************************
/*!
\file   SPIslave_Mcu.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   02.03.2011

License: See file "LICENSE"
*/

//*******************************************************************
#include "SPIslave_Mcu.h"
#include "Hardware/ReportID_Hw.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// SPIslave_0
//
//*******************************************************************
//-------------------------------------------------------------------
SPIslave_0 *SPIslave_0::spiPtr = 0;

//-------------------------------------------------------------------
SPIslave_0::SPIslave_0( SPIslave::DataHandler &handlerIn )

: SPIslave( handlerIn )

{
  spiPtr = this;

  // Port B Data Direction Register
  DDRB |=   (1<<3); // MISO: output
  DDRB &=  ~(1<<2); // MOSI: input
  DDRB &=  ~(1<<1); // SCK: input
  DDRB &=  ~(1<<0); // SS: input

  // SPI Status Register
  SPSR = (1<<SPI2X); // Double SPI Speed Bit:ON

  // SPI Control Register
  SPCR =  (1<<SPIE)  // Interrupt enable:    ON
         |(1<<SPE )  // SPI enable:          ON
         |(1<<DORD)  // Data Order:          MSB first
         |(0<<MSTR)  // Master/Slave Select: Slave
         |(1<<CPOL)  // Clock Polarity:      SCK=1 when idle
         |(1<<CPHA)  // Clock Phase:         Sample at leading edge
         |0;         // Clock Rate Select:   no effect on Slave

  SPDR = 0x00; // set first transmitted data byte
}

//-------------------------------------------------------------------
void SPIslave_0::isr( void )
{
  SPDR = handler.transceive( SPDR );
}

//-------------------------------------------------------------------
ISR( SPI_STC_vect )
{
  SPIslave_0::spiPtr->isr();
}

}  } //namespace

//EOF
