//*******************************************************************
/*!
\file   DisplayChar.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   05.04.2016

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_DISPLAY_CHAR_H
#define _HW_DISPLAY_CHAR_H

//*******************************************************************
#include "Mcu_Types.h"
#include "Std/Report.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class DisplayChar

\brief Abtract class supporting display hardware

This class provides some basic methods to clear the display and to
print characters

\example HwDisplayChar.cpp
*/

class DisplayChar
{
  protected:
    //---------------------------------------------------------------
    // Initialize the character display
    // moduleId:          Id of this module, used for report
    // numberOfLinesIn:   Number of display lines
    // numberOfColumnsIn: Number of display columns
    //
    DisplayChar( WORD moduleId,
                 BYTE numberOfLinesIn,
                 BYTE numberOfColumnsIn );

  public:
    //---------------------------------------------------------------
    /*! Clear display
    */
    virtual void clear( void ) = 0;

    //---------------------------------------------------------------
    /*! Refresh display
    */
    virtual void refresh( void ) = 0;

    //---------------------------------------------------------------
    /*! Set position of next character, relative to the upper left
        corner
        \param line Line number (0,...)
        \param column Column number (0,...)
    */
    virtual void gotoTextPos( BYTE line, BYTE column ) = 0;

    //---------------------------------------------------------------
    /*! Put character at actual position and auto increment column
        for next character
        \param c Character (ASCII code)
    */
    virtual void putChar( char c ) = 0;

    //---------------------------------------------------------------
    /*! Put string at actual position
        \param str Character-String (ASCII code)
    */
    virtual void putString( const char *str );

    //---------------------------------------------------------------
    /*! Get number of lines supported by the display
        \return Number of lines
    */
    virtual BYTE getNumberOfLines( void );

    //---------------------------------------------------------------
    /*! Get number of columns supported by the display
        \return Number of columns
    */
    virtual BYTE getNumberOfColumns( void );

  protected:
    //---------------------------------------------------------------
    WORD numberOfLines;
    WORD numberOfColumns;

  public:
    //---------------------------------------------------------------
    Std::Report report;

}; //DisplayChar

} } //namespace

#endif
