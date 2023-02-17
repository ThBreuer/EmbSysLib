//*******************************************************************
/*!
\file   DisplayChar_Terminal.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   21.11.2022

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_DISPLAY_CHAR_TERMINAL_H
#define _HW_DISPLAY_CHAR_TERMINAL_H

//*******************************************************************
#include "Hardware/Common/Display/DisplayChar.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class DisplayChar_Terminal

\brief Terminal display

This class emulates a character display on an uart/tty terminal.

\example HwDisplayChar.cpp
*/
class DisplayChar_Terminal : public DisplayChar
{
  public:
    //---------------------------------------------------------------
    /*! Initialize display
       \param uart Reference to an UART interface
       \param numOfLines Number of lines (default: 10)
       \param numOfColumns Number of coumns (default: 20)
    */
    DisplayChar_Terminal( EmbSysLib::Hw::Uart &uart, WORD numOfLines = 10, WORD numOfColumns = 20 );

    //---------------------------------------------------------------
    virtual void clear( void );

    //---------------------------------------------------------------
    virtual void refresh( void );

    //---------------------------------------------------------------
    virtual void gotoTextPos( BYTE line,
                              BYTE colum );

    //---------------------------------------------------------------
    virtual void putChar( char c );

  private:
    //---------------------------------------------------------------
    void sendESC( const char *seq );

    //---------------------------------------------------------------
    EmbSysLib::Hw::Uart &uart;
    BYTE     line;
    BYTE     colum;

}; //class DisplayChar_Terminal


}  } //namespace

#endif
