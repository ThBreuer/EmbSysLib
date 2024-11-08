//*******************************************************************
/*!
\file   SPImaster.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   20.03.2023

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_SPI_MASTER_H
#define _HW_SPI_MASTER_H

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
#include "Hardware/Common/Port/Port.h"

//*******************************************************************
/*!
\class SPImaster

\brief Abstract class supporting the SPI hardware in master mode

SPI is a serial communication interface.
This base class handles data transmit/receive actions \n

\remark Interrupts are disabled during data transfer

Derived classes should at least:
 - initialize the SPI hardware
 - implement \a start() and \a stop() to control transmission
 - implement \a transceiveByte() to read/write a data byte from/to
   the hardware

\example HwSPImaster.cpp
*/
class SPImaster
{
  public:
    //---------------------------------------------------------------
    /*! SPI clock polarioty and phase
    */
    typedef enum
    {
      CPOL_L_CPHA_L = 0x00, //!< SCK low in idle state  & sample data at SCK rising edge
      CPOL_L_CPHA_H = 0x01, //!< SCK low in idle state  & sample data at SCK falling edge
      CPOL_H_CPHA_L = 0x10, //!< SCK high in idle state & sample data at SCK falling edge
      CPOL_H_CPHA_H = 0x11  //!< SCK high in idle state & sample data at SCK rising edge

    } ClockPolPha;

    //---------------------------------------------------------------
    /*! \class Device
        \todo comment missing ...
    */
    class Device
    {
      public:
        //-----------------------------------------------------------
        /*! \todo comment missing ...
            \param spi     Reference to SPI hardware
            \param cs_port Reference to port of SPI chip select pin
            \param cs_pin  Chip select pin number
        */
        Device( SPImaster &spi,
                Port      &cs_port,
                BYTE       cs_pin )

        : spi( spi )
        , cs( cs_port, cs_pin )

        {
          cs.setMode( Port::Out );
          cs.set();
        }

        //-----------------------------------------------------------
        /*! \todo comment missing ...
            \todo make virtual, so that SPImaster_Mcu can overload this
        */
        void start()
        {
          cs.clr();
        }

        //-----------------------------------------------------------
        /*! \todo comment missing ...
            \todo make virtual, so that SPImaster_Mcu can overload this
        */
        void stop()
        {
          cs.set();
        }

        //---------------------------------------------------------------
        /*! Transmit and receive one data byte, NO START/STOP
            \param data Transmitted data byte
            \return Received data byte
        */
        BYTE transceiveByte( BYTE data = 0 );

        //---------------------------------------------------------------
        /*! Transmit and receive one data byte
            \param data Transmitted data byte
            \return Received data byte
        */
        BYTE transceive( BYTE data = 0 );

        //---------------------------------------------------------------
        /*! Transmit and receive multiple data bytes
            \param data Pointer to transmitted data byte
            \param size Number of data bytes
        */
        void transceive( BYTE *data, WORD size );

        //---------------------------------------------------------------
        /*! Get one data byte from slave
            \return Received data byte
        */
        BYTE read( void );

        //---------------------------------------------------------------
        /*! Get \a size data byte from slave
            \param data Pointer to received data structure
        */
        template <class T>
        void read( T *data );

        //---------------------------------------------------------------
        /*! Write one data byte to slave
            \param data Transmitted data byte
        */
        void write( BYTE data );

        //---------------------------------------------------------------
        /*! Write one data word (16 bit) to slave, MSB first
            \param data Transmitted data word
        */
        void write( WORD data );

        //---------------------------------------------------------------
        /*! Write \a size data byte to slave
            \param data Pointer to transmitted data structure
        */
        template <class T>
        void write( T *data );

      public:
        //-----------------------------------------------------------
        SPImaster &spi;
        Port::Pin  cs;
    };

  protected:
    //---------------------------------------------------------------
    // Standard constructor
    //
    SPImaster( void );

  private:
    //---------------------------------------------------------------
    // Write data into transmission buffer, wait for transmission
    // complete and return received data
    // data:   Transmitted data byte
    // return: Received data byte
    //
    virtual BYTE transceiveByte( BYTE data = 0 ) = 0;

}; //class SPImaster

} } //namespace

#endif
