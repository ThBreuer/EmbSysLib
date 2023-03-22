//*******************************************************************
/*!
\file   Terminal.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   06.02.2023

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _DEV_TERMINAL_H
#define _DEV_TERMINAL_H

//*******************************************************************
#include "Hardware/Common/Uart/Uart.h"

//*******************************************************************
namespace EmbSysLib {
namespace Dev {

//*******************************************************************
/*!
\class Terminal

\brief Uart based string I/O

This class uses an \b Uart object to handle character in- and
output. The received data stream is buffered until a query is made by
get() or getString().

ESC sequences are repaced by a 2 Byte sequence

\example DevTerminal.cpp
*/
class Terminal
{
  public:
    //---------------------------------------------------------------
    /*! Initialize a Terminal object
        \param uart Reference to hardware object
        \param inBufSize Size of input buffer
        \param outBufSize Size of output buffer
        \param key Defines single key characters (default: no characters)
        \param eos Defines end-of-string characters (default: "\r\n")
    */
    Terminal( Uart       &uart,
              WORD        inBufSize,
              WORD        outBufSize,
              const char *key = "",
              const char *eos = "\r\n" );

  public:
    //---------------------------------------------------------------
    /*! \enum KeyCode
        \brief Selected key codes of an ESC sequence
    */
    typedef enum
    {
      UP     = 0x141, //!< cursor key 'up'
      DOWN   = 0x142, //!< cursor key 'down'
      RIGHT  = 0x143, //!< cursor key 'right'
      LEFT   = 0x144, //!< cursor key 'left'
      PGUP   = 0x105, //!< page up key
      PGDOWN = 0x106, //!< page down key
    } KeyCode;

    //---------------------------------------------------------------
    /*! \enum KeyAttribute
        \brief Control bits marking an ESC sequence
    */
    typedef enum
    {
      ESC_SEQUENCE = 0x0100, //!< Next byte is the key code of an ESC sequence
      SHIFT_CTRL   = 0x0200, //!< Shift or ctrl key pressed simultaneous with cursor key
      ALT          = 0x0400  //!< Alt key pressed simultaneous with cursor key
    } KeyAttribute;

  public:
    //---------------------------------------------------------------
    /*! Read an input string

        The input is read from Uart until
        - \b inBufSize is reached
        - OR an ESC sequence is detected
        - OR one of the 'single keys' defined by \b key is received
        - OR one of the 'end of string' characters defined by \b eos is received.

        The first call to \b getString() after an input is complete
        will return a pointer to the input string.

        Each call to this function will initiate a reading from the
        hardware as far as new data is available. ESC sequences or
        function key codes are converted.

        \return Pointer to last received string or NULL
    */
    char *getString( void );

    //---------------------------------------------------------------
    /*! Reads a character from Uart and filters ESC sequences.

        If the received data stream contains a ESC sequence, the ESC
        sequence is mapped to a 2-byte sequence. The first byte
        contains a control information (KeyAttribute), the second one
        contains the KeyCode.
        An ESC sequence is interpreted by getString() as end of line.

        Format of the ESC sequence:
        \code
          [esc]  esc  "[" | "O"  number{number}  "~"
          [esc]  esc  "[" | "O"  char
        \endcode

        \return Received character or key code
    */
    /*
      Tested ESC-Sequences:
      =====================
        Key              PuTTY           Windows-Shell   remark
        -----------------------------------------------------------------
        UP               1b 5b 41        1b 5b 41
        DOWN             1b 5b 42        1b 5b 42
        RIGHT            1b 5b 43        1b 5b 43
        LEFT             1b 5b 44        1b 5b 44

        PGUP             1b 5b 35 7e     1b 5b 49
        PGDOWN           1b 5b 36 7e     1b 5b 51

        Shift/Ctrl UP    1b 4f 41        ---
        Shift/Ctrl DOWN  1b 4f 42        ---
        Shift/Ctrl RIGHT 1b 4f 43        1b 5b 74
        Shift/Ctrl LEFT  1b 4f 44        1b 5b 73
        Ctrl PGUP        ---             1b 5b 86
        Ctrl PGDOWN      ---             1b 5b 76

        F1               1b 5b 31 31 7e  ---             '11'
        F2               1b 5b 31 32 7e  ---             '12'
        F3               1b 5b 31 33 7e  ---             '13'
        F4               1b 5b 31 34 7e  ---             '14'
        F5               1b 5b 31 35 7e  ---             '15'
        F6               1b 5b 31 37 7e  ---             '17'
        F7               1b 5b 31 38 7e  ---             '18'
        F8               1b 5b 31 39 7e  ---             '19'
        F9               1b 5b 32 30 7e  ---             '20'
        F10              1b 5b 32 31 7e  ---             '21'
        F11              1b 5b 32 33 7e  ---             '23'
        F12              1b 5b 32 34 7e  ---             '24'
        Shift F1         1b 5b 32 33 7e  ---             '23' same as 'F11'
        Shift F2         1b 5b 32 34 7e  ---             '24' same as 'F12'
        Shift F3         1b 5b 32 35 7e  ---             '25'
        Shift F4         1b 5b 32 36 7e  ---             '26'
        Shift F5         1b 5b 32 38 7e  ---             '28'
        Shift F6         1b 5b 32 39 7e  ---             '29'
        Shift F7         1b 5b 33 31 7e  ---             '31'
        Shift F8         1b 5b 33 32 7e  ---             '32'
        Shift F9         1b 5b 33 33 7e  ---             '33'
        Shift F10        1b 5b 33 34 7e  ---             '34'
        Shift F11        1b 5b 32 33 7e  ---             '23' same as 'F11'
        Shift F12        1b 5b 32 34 7e  ---             '24' same as 'F12'

        +ALT-Key:        leading 1b      ---
        -----------------------------------------------------------------
        note "---": no ESC sequence or not testable

    */
    WORD get();

    //---------------------------------------------------------------
    /*! Print a string

        The syntax and usage of this method is compatible to the same named
        ANSI-C function.
        The length of the output string is limited to \b outBufSize.
        \param format Format string as defined in ANSI-C
        \param ... Additional arguments
    */
    virtual void printf( const char *format, ... );

  private:
    //---------------------------------------------------------------
    WORD remapESC( WORD num )
    {
      switch( num )
      {
        case 0x49: return(            PGUP   );
        case 0x51: return(            PGDOWN );
        case 0x86: return( SHIFT_CTRL|PGUP   );
        case 0x76: return( SHIFT_CTRL|PGDOWN );
        case 0x73: return( SHIFT_CTRL|LEFT   );
        case 0x74: return( SHIFT_CTRL|RIGHT  );
        default:   return( num );
      }
    }

    //---------------------------------------------------------------
    bool isNumber( BYTE c )
    {
      return( 0x30 <= c && c <= 0x39 );
    }

  private:
    //---------------------------------------------------------------
    Uart       &uart;
    WORD        inBufSize;
    WORD        outBufSize;
    const char *key;
    const char *eos;
    char       *strIn;
    char       *strOut;
    BYTE        idx;
    BYTE        escape;
    BYTE        last;
    WORD        ctrl;
    BYTE        num;
    WORD        pending;

}; //class Terminal

} } //namespace

#endif
