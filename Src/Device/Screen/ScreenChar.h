//*******************************************************************
/*!
\file   ScreenChar.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   01.02.2023

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _DEV_SCREEN_CHAR_H
#define _DEV_SCREEN_CHAR_H

//*******************************************************************
#include "Hardware/Common/Display/DisplayChar.h"

//*******************************************************************
namespace EmbSysLib {
namespace Dev {

//*******************************************************************
/*!
\class ScreenChar

\brief Character Screen

\example DevScreenChar.cpp
*/
class ScreenChar
{
  public:
    //---------------------------------------------------------------
    /*! Initialize Screen

        Using DisplayChar hardware
        \param display Reference to a DisplayChar object
    */
    ScreenChar( DisplayChar &display );

    //---------------------------------------------------------------
    /*! Clear Screen
    */
    virtual void clear( void );

    //---------------------------------------------------------------
    /*! Refresh Screen
    */
    virtual void refresh( void );

    //---------------------------------------------------------------
    /*! Print a formated text string

        The string is printed on position (\a line, \a column).
        Beginning with the \b format parameter, this method is
        equal to the correspondent ANSI-C function
        \param line   Line number
        \param column Column number of first character
        \param format Formated string to print
    */
    virtual void printf( BYTE        line,
                         BYTE        column,
                         const char *format,
                         ... );

  private:
    //---------------------------------------------------------------
    DisplayChar &display;
    BYTE         maxStrLength;
    char        *str;

}; //class ScreenChar

} } //namespace

#endif
