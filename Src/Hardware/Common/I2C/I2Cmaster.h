//*******************************************************************
/*!
\file   I2Cmaster.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_I2C_MASTER_H
#define _HW_I2C_MASTER_H

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class I2Cmaster

\brief Abstract class supporting the I2C hardware in master mode

I2C is a serial communication interface.
This class base supports an I2C bus and handles data transmit and 
receive actions \n

Devices connected to the I2C bus can be represented by an instance of 
\see I2Cmaster::Device

Derived classes should at least:
 - initialize the I2C hardware
 - implement \a start() and \a stop() to control transmission
 - implement \a sendAddr(), \a writeByte() and \a readByteNack()
   to read/write a data byte from/to hardware

\example HwI2Cmaster.cpp
*/
class I2Cmaster
{
  protected:
    //---------------------------------------------------------------
    // Standard constructor
    //
    I2Cmaster( WORD moduleId );

  public:
    //---------------------------------------------------------------
    /*! \class Device

        \brief This class represents a single device connected to 
               the I2C bus
        \remark Due to the template functions an explicit type cast
                might be necassary, e.g. \code read( (BYTE)0x02 ) \endcode
    */
    class Device
    {
      public:
        //-----------------------------------------------------------
        /*! Initialize and connect the device to the bus
            \param i2c    Reference to I2C hardware (I2C bus interface)
            \param hwAddr Device's hardware address
        */
        Device( I2Cmaster &i2c,
                BYTE       hwAddr )
        : i2c( i2c )
        {
          this->hwAddr = hwAddr;
          isErrorFlag  = false;
        }

        //---------------------------------------------------------------
        /*! Read a byte from the device
            \return Received data byte
        */
        BYTE read( void )
        {
          BYTE dat = 0;
          isErrorFlag |= i2c.read( hwAddr, NULL, 0, &dat, 1 );
          return( dat );
        }

        //---------------------------------------------------------------
        /*! Write an address to the device and get back a data byte.
            \param  addr Memory or register adress of the device
            \return Received data byte
        */
        template <class A> 
        BYTE read( A addr )
        {
          BYTE dat = 0;
          isErrorFlag |= i2c.read( hwAddr, (BYTE*)&addr, sizeof(A), &dat, 1 );
          return( dat );
        }

        //---------------------------------------------------------------
        /*! Read a data structure from the device
            \param data Pointer to data structure
        */
        template <class T> 
        void read( T *data )
        {
          isErrorFlag |= i2c.read( hwAddr, NULL, 0, (BYTE*)data, sizeof(T) );
        }

        //---------------------------------------------------------------
        /*! Write an adress to the device and get back a data structure.
            \param addr Memory or register adress of the device
            \param data Pointer to data structure
        */
        template <class A, class T>
        void read( A addr, T *data )
        {
          isErrorFlag |= i2c.read( hwAddr, (BYTE *)&addr, sizeof(A), (BYTE *)data, sizeof(T) );
        }

        //---------------------------------------------------------------
        /*! Write a byte to the device
            \param  addr Transmitted data byte
        */
        template <class A>
        void write( A addr )
        {
          isErrorFlag |= i2c.write( hwAddr, (BYTE *)&addr, sizeof(A), NULL, 0 );
        }

        //---------------------------------------------------------------
        /*! Write a data structure to the device
            \param data Pointer to data structure
        */
        template <class T>
        void write( T *data )
        {
          isErrorFlag |= i2c.write( hwAddr, NULL, 0, (BYTE *)data, sizeof(T) );
        }

        //---------------------------------------------------------------
        /*! Write first an address and second a data structure
            \param addr Memory or register adress of the device
            \param data Reference to data structure
        */
        template <class A, class T>
        void write( A addr, T data  )
        {
          isErrorFlag |= i2c.write( hwAddr, (BYTE *)&addr, sizeof(A), (BYTE *)&data, sizeof(T) );
        }

        //---------------------------------------------------------------
        /*! Write first an address and second a data structure
            \param addr Memory or register adress of the device
            \param data Pointer to data structure
        */
        template <class A, class T> 
        void write( A addr, T *data )
        {
          isErrorFlag |= i2c.write( hwAddr, (BYTE *)&addr, sizeof(A), (BYTE *)data, sizeof(T) );
        }

        //---------------------------------------------------------------
        /*! Check, if an error occurred
            Returns internal error flag. This flag is reseted by next
            call of \a read() or \a write()
            \return
                    - true:  Error, previous transfer was aborted
                    - false: No error
        */
        virtual BYTE isError( void )
        {
          BYTE ret = isErrorFlag;
          isErrorFlag = false;
          return( ret );
        }

      private:
        //-----------------------------------------------------------
        I2Cmaster &i2c;
        BYTE       hwAddr;
        BYTE       isErrorFlag;
    };

  protected:
    //---------------------------------------------------------------
    BYTE isHardwareOK;

  protected:
    //---------------------------------------------------------------
    virtual BYTE write( BYTE  hwAddr,
                        BYTE *adr,
                        BYTE  sizeAdr,
                        BYTE *data,
                        BYTE  sizeData );

    //---------------------------------------------------------------
    virtual BYTE read( BYTE  hwAddr,
                       BYTE *adr,
                       BYTE  sizeAdr,
                       BYTE *data,
                       BYTE  sizeData );
  private:
    //---------------------------------------------------------------
    // Start a I2C transfer.
    // Send Start-Condition and wait until ready
    //
    virtual void start( BYTE size ) = 0;

    //---------------------------------------------------------------
    // Stop I2C transfer
    // Send Stop-Condition
    //
    virtual void stop( void ) = 0;

    //---------------------------------------------------------------
    // Send hwAddr (slave) with added R/W flag, wait for acknowledge
    // hwAddr:  Device's hardware adress
    //          (master write: LSB = 0, master read: LSB = 1)
    //
    virtual void sendAddr( BYTE hwAddr ) = 0;

    //---------------------------------------------------------------
    // Send data byte and wait for acknowledge
    // data:  Transmitted data byte
    //
    virtual void writeByte( BYTE data ) = 0;

    //---------------------------------------------------------------
    // Wait for received data byte and return it
    // return: Received data byte
    //
    virtual BYTE readByteAck( void ) = 0;

    //---------------------------------------------------------------
    // Wait for received data byte and return it
    // return: Received data byte
    //
    virtual BYTE readByteNack( void ) = 0;

  public:
    //---------------------------------------------------------------
    Std::Report report;

}; //class I2Cmaster

} } //namespace

#endif
