//*******************************************************************
/*!
\file   SPImaster_Mcu.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   02.03.2011

License: See file "LICENSE"
*/

//*******************************************************************
#include "SPImaster_Mcu.h"
#include "Hardware/ReportID_Hw.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// SPImaster_0
//
//*******************************************************************
//-------------------------------------------------------------------
SPImaster_0::SPImaster_0( SPI_Baudrate  baud,
                          ClockPolPha clockPolPhaIn )

: SPImaster()

{
  // SPI clock rate calculation
  //
  // The calculated values are valid for fosc = 16 MHz
  //
  //    SPI2X | SPR1 | SPR0 || clock rate         | T/bit   | T/byte
  //    ------+------+------++--------------------+---------+-------
  //      0   |  0   |  0   ||  fosc/4     4 MHz  | 0,25µs  | 2µs
  //      0   |  0   |  1   ||  fosc/16    1      | 1       | 8
  //      0   |  1   |  0   ||  fosc/64    0,25   | 4       | 32
  //      0   |  1   |  1   ||  fosc/128   0,125  | 8       | 64
  //      1   |  0   |  0   ||  fosc/2     8      | 0,125   | 1
  //      1   |  0   |  1   ||  fosc/8     2      | 0,5     | 4
  //      1   |  1   |  0   ||  fosc/32    0,5    | 2       | 16
  //      1   |  1   |  1   ||  fosc/64    0,25   | 4       | 32
  //

  BYTE spr;

  switch( baud ) // SPI2X = 1 only
  {
    default:
    case CR_250kHz:  spr = 0x03; break;
    case CR_500kHz:  spr = 0x02; break;
    case CR_2000kHz: spr = 0x01; break;
    case CR_8000kHz: spr = 0x00; break;
  }

    DDRB |= (1<<PB2)  // MOSI: output
           |(1<<PB1); // SCK:  output         

  // SPI Status Register
  SPSR = (1<<SPI2X); // Double SPI Speed Bit:ON (see above)

  // SPI Control Register
  SPCR =  (0<<SPIE)  // Interrupt enable:    OFF
         |(1<<SPE )  // SPI enable:          ON
         |(1<<DORD)  // Data Order:          MSB first
         |(1<<MSTR)  // Master/Slave Select: Master
         |(((clockPolPhaIn&0x10)?1:0)<<CPOL) // Clock Polarity
         |(((clockPolPhaIn&0x01)?1:0)<<CPHA) // Clock Phase
         |spr;       // Clock Rate Select:   see above
}

//-------------------------------------------------------------------
BYTE SPImaster_0::transceiveByte( BYTE data )
{
  // start transmission
  SPDR = data;

  // wait until transmission completed
  while( !(SPSR & (1<<SPIF)) );

  return( SPDR );
}

//-------------------------------------------------------------------
BYTE SPImaster_0::transceiveByteExt( BYTE bit, BYTE data )
{
  if( bit )
    PORTB = PORTB | (1<<5);    //set MOSI 
  else
    PORTB = PORTB & (~(1<<5)); //clr MOSI
  
  PORTB = PORTB & (~(1<<7));   // SCK low
  SPCR &= ~(1<<SPE);           // disable SPI
  PORTB = PORTB | (1<<7);      // SCK high
  SPCR |= (1<<SPE);            // enable SPI  

  return( transceiveByte( data ) );
}

}  } //namespace

//EOF
