//*******************************************************************
/*!
\file   UART.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"
*/

//*******************************************************************
#include "Uart.h"
#include "Hardware/ReportID_Hw.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// Uart
//
//*******************************************************************
//-------------------------------------------------------------------
Uart::Uart( WORD moduleId, WORD sizeRxBuf, WORD sizeTxBuf )

: dataRxBuf( sizeRxBuf ) // initialize both FIFOs
, dataTxBuf( sizeTxBuf )
, report( moduleId )
{
  if(   dataRxBuf.getSize() != sizeRxBuf
     || dataTxBuf.getSize() != sizeTxBuf )
  {
    report.error( ReportID_Hw::Event::MEMORY_ALLOCTION_ERROR );
  }
  txReady = true;
}

//-------------------------------------------------------------------
void Uart::isrRx( void )
{
  BYTE data = getByte(); // read data from hardware

  dataRxBuf << data;     // pipe data into FIFO
}

//-------------------------------------------------------------------
inline void Uart::isrTx( void )
{
  BYTE data = 0;

  if( !dataTxBuf.isEmpty() ) // data to been sent?
  {
    dataTxBuf >> data;       // read data from FIFO
    txReady = false;         // set flag before setByte() because
                             // some hardware doesn't support
                             // interrupts and isrTx is called recursiv
    setByte( data );         // write data to hardware
  }
  else
  {
    txReady = true;          // we are ready
  }
}

//-------------------------------------------------------------------
void Uart::transmitByte( BYTE data )
{
  while( dataTxBuf.isFull() ) // wait until FIFO is ready
  {
    if( txReady )             // this case is an error:
                              //   FIFO is full,
                              //   but isrTx is not running
    {
      isrTx();                // call isrTx explicit
    }
                              // else: Data transfer is running,
                              //       we have to wait until complete
  }

  dataTxBuf << data;          // pipe data into FIFO

  if( txReady )               // at first time, isrTx has to be
                              // called explicit without interrupt
  {
    isrTx();
  }
}

//-------------------------------------------------------------------
void Uart::set( BYTE data )
{
  transmitByte( data );
}

//-------------------------------------------------------------------
void Uart::set( const BYTE *data, WORD length )
{
  while( length-- )
  {
    transmitByte( *(data++ ) ); // put a character and set pointer to next
  }
}

//-------------------------------------------------------------------
void Uart::set( const char *str )
{
  while( *str )                // until end of null terminated string
  {
    transmitByte( *(str++ ) ); // put a character and set pointer to next
  }
}

//-------------------------------------------------------------------
inline bool Uart::isTxBufferFull( void )
{
  return( dataTxBuf.isFull() );
}

//-------------------------------------------------------------------
inline WORD Uart::getFifoRemainingSize( void )
{
  return( dataTxBuf.getFree() );
}

//-------------------------------------------------------------------
bool Uart::get( BYTE &data )
{
  BYTE tmp = 0;

  if( dataRxBuf >> tmp )   // is there a data byte in the FIFO?
  {
    data = tmp;            // copy data byte to pointer
    return( true );        // yes, we received a data byte
  }
  else
  {
    return( false );       // no data byte received
  }
}

//-------------------------------------------------------------------
BYTE Uart::get( void )
{
  BYTE tmp = 0;

  if( get(tmp) )    // is there a data byte in the FIFO?
  {
    return( tmp );  // yes, we received a data byte
  }
  else
  {
    return( 0 );    // no data byte received
  }
}

}  } //namespace
