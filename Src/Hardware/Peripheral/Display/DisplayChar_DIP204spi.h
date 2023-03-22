//*******************************************************************
/*!
\file   DisplayChar_DIP204spi.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   20.03.2023

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_DISPLAY_CHAR_DIP204_SPI_H
#define _HW_DISPLAY_CHAR_DIP204_SPI_H

//*******************************************************************
#include "Hardware/Common/Display/DisplayChar.h"
#include "Hardware/Common/SPI/SPImaster.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*! 
\class DisplayChar_DIP204spi

\brief LCD character display DIP204 with SPI interface

\example HwDisplayChar_DIP204spi.cpp
*/
class DisplayChar_DIP204spi : public DisplayChar
{
  public:
    //---------------------------------------------------------------
    /*! Initialize display
       \param spi Reference to a SPI device object
    */
    DisplayChar_DIP204spi( SPImaster::Device &spi );

    //---------------------------------------------------------------
    virtual void clear( void );

    //---------------------------------------------------------------
    virtual void refresh() { /* nothing to do */ }

    //---------------------------------------------------------------
    virtual void gotoTextPos( BYTE lineIn, 
                              BYTE columnIn );

    //---------------------------------------------------------------
    virtual void putChar( char c );

  private:
    //---------------------------------------------------------------
    void init( void );

    //---------------------------------------------------------------
    void waitBusy( void );

    //---------------------------------------------------------------
    void writeCmd( BYTE cmd );

    //---------------------------------------------------------------
    void writeDat( BYTE dat ); 

  private:
    //---------------------------------------------------------------      
    SPImaster::Device &spi;
 
    BYTE  line;
    BYTE  column;

    //---------------------------------------------------------------
    static const WORD NUM_OF_LINE   =  4;
    static const WORD NUM_OF_COLUMN = 20;

}; //class DisplayChar_DIP204spi

}  } //namespace

#endif
