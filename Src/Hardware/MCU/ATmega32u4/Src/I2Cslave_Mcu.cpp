//*******************************************************************
/*!
\file   I2Cslave_MCU.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   02.03.2011

License: See file "LICENSE"
*/

//*******************************************************************
#include "I2Cslave_Mcu.h"
#include "Hardware/ReportID_Hw.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// I2Cslave_0
//
//*******************************************************************
//-------------------------------------------------------------------
I2Cslave_0 *I2Cslave_0::i2cPtr = 0;

//-------------------------------------------------------------------
I2Cslave_0::I2Cslave_0( DataHandler &handlerIn,
                        BYTE         hwAddrIn )

: I2Cslave( handlerIn, hwAddrIn )

{
  i2cPtr = this;

  cnt = 0;

  TWAR = hwAddr; // TWI (Slave) Address Register

  SetControlReg();
}

//-------------------------------------------------------------------
void I2Cslave_0::SetControlReg( void )
{
  // TWI Control Register
  TWCR =  (1<<TWINT)  // Interrupt Flag
         |(1<<TWEA )  // Enable Acknowledge Bit
         |(0<<TWSTA)  // START Condition Bit
         |(0<<TWSTO)  // STOP Condition Bit
         |(0<<TWWC )  // Write Collision Flag (read only)
         |(1<<TWEN )  // Enable Bit
         |(1<<TWIE ); // Interrupt Enable
}

//-------------------------------------------------------------------
void I2Cslave_0::isr( void )
{
  switch( TWSR )
  {
    //---------------------------------------------------------------
    case 0xA8: // STX: Own SLA+R has been received;
               //      ACK has been returned
               //----------------------------------------------------

    case 0xB0: // STX: Arbitration lost in SLA+R/W as Master;
               //      Own SLA+R has been received;
               //      ACK has been returned
               //----------------------------------------------------

      // Enter slave transmitter mode
      cnt  = 0;                       // Set counter to first data byte
      TWDR = handler.transmit( cnt ); // Get data byte from callback
                                      // function and transmit
      SetControlReg();                // Prepare for next call
      break;

    //---------------------------------------------------------------
    case 0xB8: // STX: Data byte in TWDR has been transmitted;
               //      ACK has been received
               //----------------------------------------------------

      // Next data byte in slave transmitter mode is requested
      TWDR = handler.transmit( ++cnt ); // Get data byte
      SetControlReg();                  // Prepare for next call
      break;

    //---------------------------------------------------------------
    case 0xC0: // STX: Data byte in TWDR has been transmitted;
               //      NACK has been received
               //----------------------------------------------------

      // Leave slave transmitter mode
      SetControlReg();                // Prepare for next call
      break;

    //---------------------------------------------------------------
    case 0x70: // SRX: General call address has been received;
               //      ACK has been returned
               //----------------------------------------------------

    case 0x78: // SRX: Arbitration lost in SLA+R/W as Master;
               //      General call address has been received;
               //      ACK has been returned
               //----------------------------------------------------

    case 0x60: // SRX: Own SLA+W has been received;
               //      ACK has been returned
               //----------------------------------------------------

    case 0x68: // SRX: Arbitration lost in SLA+R/W as Master;
               //      Own SLA+W has been received;
               //      ACK has been returned
               //----------------------------------------------------

      // Enter slave reveicer mode
      cnt = 0;         // Set counter to first data byte
      SetControlReg(); // Prepare for next call
      break;

    //---------------------------------------------------------------
    case 0x80: // SRX: Previously addressed with own SLA+W;
               //      Data has been received;
               //      ACK has been returned
               //----------------------------------------------------

    case 0x90: // SRX: Previously addressed with general call;
               //      Data has been received;
               //      ACK has been returned
               //----------------------------------------------------

      // Data received in slave receiver mode
      handler.receive( TWDR, cnt++ );  // Handle received data
      SetControlReg();                 // Prepare for next call
      break;

    //---------------------------------------------------------------
    case 0xA0: // SRX: A STOP condition or repeated START condition
               //      has been received while still addressed as slave
               //----------------------------------------------------

      // Leave slave receiver mode
      SetControlReg();                 // Prepare for next call
      break;

    //---------------------------------------------------------------
    case 0x88:  // SRX: Previously addressed with own SLA+W;
                //      Data has been received;
                //      NOT ACK has been returned
                //---------------------------------------------------

    case 0x98:  // SRX: Previously addressed with general call;
                //      Data has been received;
                //      NOT ACK has been returned
                //---------------------------------------------------

    case 0xC8:  // STX: Last data byte in TWDR has been transmitted
                //      (TWEA = "0");
                //      ACK has been received
                //---------------------------------------------------

    case 0xF8:  //      No relevant state information available;
                //      TWINT = "0"
                //---------------------------------------------------

      // End of transaction
      SetControlReg();

    //---------------------------------------------------------------
    case 0x00: // Bus error due to an illegal START or STOP condition
               //----------------------------------------------------

      // Bus error
      SetControlReg();
      
      //!< \todo how to recover from bus error?
 
      System::reset();
      break;

    //---------------------------------------------------------------
    default:
      // Unknown state
      SetControlReg();  // Prepare for next call
      break;
  }
}

//-------------------------------------------------------------------
ISR( TWI_vect )
{
  System::disableInterrupt();

  I2Cslave_0::i2cPtr->isr();

  System::enableInterrupt();
}


}  } //namespace

//EOF
