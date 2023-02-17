//*******************************************************************
/*!
\file   Color.h
\author Thomas Breuer
\date   25.11.2021
*/

//*******************************************************************

//*******************************************************************
#include "MCU_Types.h"

//*******************************************************************
/*!
\class hwColor

\brief Color

\see HwDisplayGraphic
*/

//-------------------------------------------------------------------
#define RGB2COLOR(red, green, blue ) \
    ( (((blue )& 0xF8) >> 3)  /* 5 bit,  0.. 4 */\
     |(((green)& 0xFC) << 3)  /* 6 bit,  5..10 */\
     |(((red  )& 0xF8) << 8)) /* 5 bit, 11..15 */
//-------------------------------------------------------------------

//*******************************************************************
class Color
{
  public:
    //---------------------------------------------------------------
    /*! Predefined color map
    */
    //*******************************************************************
    //
    // color coding: rrrrr gggggg bbbbb
    //
    //*******************************************************************

    /*! Predefined colors
    */
    typedef enum
    {
      //                       Red Green Blue
      Black       = RGB2COLOR(   0,    0,   0 ),  //!< Black
      DarkGrey    = RGB2COLOR(  64,   64,  64 ),  //!< DarkGrey
      Grey        = RGB2COLOR( 128,  128, 128 ),  //!< Grey
      LightGrey   = RGB2COLOR( 192,  192, 192 ),  //!< LightGrey / Silver
      White       = RGB2COLOR( 255,  255, 255 ),  //!< White

      Red         = RGB2COLOR( 255,    0,   0 ),  //!< Red
      Green       = RGB2COLOR(   0,  255,   0 ),  //!< Green
      Blue        = RGB2COLOR(   0,    0, 255 ),  //!< Blue

      Yellow      = RGB2COLOR( 255,  255,   0 ),  //!< Yellow
      Cyan        = RGB2COLOR(   0,  255, 255 ),  //!< Cyan
      Magenta     = RGB2COLOR( 255,    0, 255 ),  //!< Magenta


      DarkRed     = RGB2COLOR( 128,    0,   0 ),  //!< DarkRed / Maroon
      DarkGreen   = RGB2COLOR(   0,  128,   0 ),  //!< DarkGreen
      DarkBlue    = RGB2COLOR(   0,    0, 128 ),  //!< DarkBlue / Navy

      DarkYellow  = RGB2COLOR( 128,  128,   0 ),  //!< DarkYellow / Olive
      DarkCyan    = RGB2COLOR(   0,  128, 128 ),  //!< DarkCyan
      DarkMagenta = RGB2COLOR( 128,    0, 128 )   //!< Purple

   } colorPredefined;

//-------------------------------------------------------------------
#undef RGB2COLOR
//-------------------------------------------------------------------

}; //Color


