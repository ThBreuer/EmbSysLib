//*******************************************************************
/*!
\file   Isc_Uart.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   03.01.2024

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _MOD_ISC_UART_H
#define _MOD_ISC_UART_H

//*******************************************************************
#include "Module/Isc/Isc.h"

//*******************************************************************
namespace EmbSysLib {
namespace Mod {

using namespace EmbSysLib::Hw;

//*******************************************************************
/*!
\class Isc_Uart

\brief Isc transport class with UART hardware

\remark The length of the payload is limited to 64 bytes

\example Isc.cpp
*/
template <WORD maxLength> 
class Isc_Uart : public Isc
{
  private:
    //---------------------------------------------------------------
    #pragma pack(1)
    class DATA
    {
      public:
        Header header;
        BYTE data[ maxLength ];
    };
    #pragma pack()
    //---------------------------------------------------------------

  public:
    //---------------------------------------------------------------
    /*! Instantiate an Isc transport object with UART hardware

        \param uart Reference to UART hardware
        \param crcMode Select CRC calculation mode (FAST or SMALL)
    */
    Isc_Uart( Uart       &uart,
              Crc::MODE  crcMode = Crc::SMALL )
  
    : Isc( crcMode )
    , uart(uart)

    {
      recLen = 0;
      error  = false;
      ready  = false;
    }

    //---------------------------------------------------------------
    /*! ...
    */
    virtual bool isConnected()
    {
      return( true );
    }

    //---------------------------------------------------------------
    /*! Update the Isc transport object

        Checks, if new data are arrived. In that case Isc copies the
        data to identified data object, sets the new data flag and
        calls the related \b update() method.
    */
    virtual void update( void )
    {
      BYTE c;

      while( uart.get(c) )
      {
        switch( c )
        {
          case 0xA1: // START
            recLen = 0;
            flag = false;
            break;

          case 0xA2: // STOP
            ready = true;
            flag = false;
            break;

         case 0xA0: //
             flag = true;
             break;

         default:
             if( flag )
             {
                c += 0xA0;
             }
             ((BYTE*)(&rec))[recLen++] = c;
             flag = false;
             break;
        }

        if( recLen > sizeof(DATA) )
        {
          error = true;
          flag = false;
        }

        if( error )
        {
          error  = false;
          ready  = false;
          recLen = 0;
          errorCnt++;
        }

        if( ready )
        {
          ready = false;
          flag = false;

          if( !process( rec.header, rec.data, recLen-sizeof(Header) ) )
          {
            errorCnt++;
          }
          recLen = 0;
          break;
        }
      }
    }

  private:
    //---------------------------------------------------------------
    void concatByte( BYTE data )
    {
      switch( data )
      {
        case  0xA0: uart.set( (BYTE)0xA0 );
                    uart.set( (BYTE)0x00 );
                    break;
        case  0xA1: uart.set( (BYTE)0xA0 );
                    uart.set( (BYTE)0x01 );
                    break;
        case  0xA2: uart.set( (BYTE)0xA0 );
                    uart.set( (BYTE)0x02 );
                    break;
        default:    uart.set( data );
                    break;
      }
    }

    //---------------------------------------------------------------
    virtual bool writeStream( Header &header,
                             BYTE   *data,
                             WORD    size )
    {
      if( 2*(size+sizeof( Header )) + 2 > uart.getFifoRemainingSize() )
      {
        return( false );
      }
      uart.set( 0xA1 );
      for( BYTE i=0; i<sizeof( Header ); i++ )
      {
        concatByte( *((BYTE*)&header + i) );
      }
      for( BYTE i=0; i<size; i++ )
      {
        concatByte( data[i] ); // data[],
      }
      uart.set( 0xA2 );
      return( true );
    }

  private:
    //---------------------------------------------------------------
    Uart &uart;

    DATA  rec;

    bool  error;
    WORD  errorCnt;
    BYTE  recLen;
    BYTE  ready;

    bool  flag;

}; //class Isc_Uart

} } //namespace

#endif
