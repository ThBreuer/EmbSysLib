//*******************************************************************
/*!
\file   SPIslave.h
\author Thomas Breuer
\date   20.03.2023

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_SPI_SLAVE_H
#define _HW_SPI_SLAVE_H

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*! 
\class SPIslave

\brief Abstract class supporting the SPI hardware in slave mode

SPI is a serial communication interface in master/slave mode. 
This base class handles data transmit/receive actions \n

Derived classes should at least:
 - initialize the SPI hardware
 - call \a isr() in the appropriate ISR
                     
The application has to derive SPIslave::DataHandler. The instance of 
the derived class will then respond to the SPI data transfer 

\example SPIslave.cpp
*/
class SPIslave
{
  public:
    //*******************************************************************
    /*! 
    \class DataHandler

    \brief Abstract class as interface to SPIslave

    The application has to derive a own class from this interface. 
    The derived class must then referred to the SPIslave object, which 
    will call back the transceive() method. 

    \remark Because there is no handshake to the SPI master, the 
    transceive() method is time critical

    \example SPIslave.cpp
    */
    class DataHandler
    {
      public:
        //---------------------------------------------------------------
        /*! Call back method, called by SPIslave object.  
            Due to the SPI transfer protocol (shift register), \a data 
            is transmitted to the master at next transfer only
            \param data  Transmitted data byte (next time)
            \return      Received data byte
            \remark This method is time critical, because there is no 
            handshake with the master
        */
        virtual BYTE transceive( BYTE data ) = 0;

    }; //DataHandler
     
protected:
    //---------------------------------------------------------------
    // Initialize SPI hardware in slave mode and install the handler
    // handler:  Reference to the aplication specific 
    //           SPIslaveDataHandler
    //
    SPIslave( DataHandler &handler );
    
  protected:
    //---------------------------------------------------------------
    DataHandler &handler;
   
}; //class SPIslave

} } //namespace

#endif
