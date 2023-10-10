//*******************************************************************
/*!
\file   Uart_Mcu.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   14.02.2011

License: See file "LICENSE"
*/

//*******************************************************************
#include "Uart_Mcu.h"
#include "Std/Report.h"
#include "Hardware/ReportID_Hw.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// Uart_1
//
//*******************************************************************
//-------------------------------------------------------------------
Uart *Uart_1::uartPtr = 0;

//-------------------------------------------------------------------
Uart_1::Uart_1( DWORD baudRate, WORD sizeRxBuf, WORD sizeTxBuf )

: Uart( ReportID_Hw::Module::UART_MCU, sizeRxBuf, sizeTxBuf)

{
  WORD ubrr;
  BYTE u2x;

  uartPtr = this;

  if( baudRate <= 57600 )
  {
    ubrr = ((DWORD)OSC_FREQ/(16l*  baudRate)-1); 
    u2x = 0;
  }
  else
  {
	  ubrr = ((DWORD)OSC_FREQ/(16l* baudRate/2)-1); 
    u2x = 1;
  }

  // Baud Rate Registers
  UBRR1H = (uint8_t) (ubrr>>8);
  UBRR1L = (uint8_t) (ubrr);

  // Control and Status Register A
  UCSR1A =  (  0<<TXC1)  // Transmit Complete: OFF
           |(u2x<<U2X1 ) // Double the USART TX Speed: see above
           |(  0<<MPCM1);// Multi-processor Communication Mode: OFF

  // Control and Status Register B
  UCSR1B =  (1<<RXCIE1) // RX Complete Interrupt Enable: ON
           |(1<<TXCIE1) // TX Complete Interrupt Enable: ON
           |(0<<UDRIE1) // Data Register Empty Interrupt Enable: OFF
           |(1<<RXEN1 ) // Receiver Enable: ON
           |(1<<TXEN1 ) // Transmitter Enable: ON
           |(0<<UCSZ12) // Character Size: 8-bit
           |(0<<TXB81 );// Transmit Data Bit 8: OFF

  // Control and Status Register C
  UCSR1C =  (0<<UMSEL11) // Mode Select:
           |(0<<UMSEL10) //   Asynchronous Operation
           |(0<<UPM11  ) // Parity Mode:
           |(0<<UPM10  ) //   Disabled
           |(0<<USBS1  ) // Stop Bit Select: 1-bit
           |(1<<UCSZ11 ) // Character Size:
           |(1<<UCSZ10 ) //   8-bit
           |(0<<UCPOL1 );// Clock Polarity: (synchronous only)

  // Flush receive buffer
  do
  {
    UDR1;
  }
  while( UCSR1A & (1 << RXC1) );
}

//-------------------------------------------------------------------
inline BYTE Uart_1::getByte( void )
{
  return( UDR1 );
}

//-------------------------------------------------------------------
inline void Uart_1::setByte( BYTE c )
{
  UDR1 = c;
}

//-------------------------------------------------------------------
ISR( USART1_RX_vect )
{
  System::enterISR();
  Uart_1::uartPtr->isrRx();
  System::leaveISR();
}

//-------------------------------------------------------------------
ISR( USART1_TX_vect )
{
  System::enterISR();
  Uart_1::uartPtr->isrTx();
  System::leaveISR();
}

}  } //namespace

//EOF
