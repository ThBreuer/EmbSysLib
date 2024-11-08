//*******************************************************************
/*!
\file   DisplayGraphic_GRAM.h
\author Thomas Breuer
\date   13.03.2024
*/

//*******************************************************************
#ifndef _HW_DISPLAY_GRAPHIC_GRAM_H
#define _HW_DISPLAY_GRAPHIC_GRAM_H

//*******************************************************************
#include "Hardware/Common/Display/DisplayChar.h"
#include "Hardware/Common/Display/DisplayGraphic.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class DisplayGraphic_GRAM

\brief LCD display GRAM

\example  HwDisplayGraphic.cpp
*/
class DisplayGraphic_GRAM : public DisplayGraphic
{
  public:
    //---------------------------------------------------------------
    DisplayGraphic_GRAM( WORD *ptr,
                             WORD width,
                             WORD height,
                             Font         font,
                             BYTE         zoom = 1 );

    //---------------------------------------------------------------
    void init();

  
    //---------------------------------------------------------------
    virtual void clear( void );

    //---------------------------------------------------------------
    virtual void refresh( void );

    //---------------------------------------------------------------
    virtual void setArea( WORD x,
                          WORD y,
                          WORD w,
                          WORD h );

    //---------------------------------------------------------------
    virtual void setPixel( WORD color,
                           WORD x,
                           WORD y );

    //---------------------------------------------------------------
    virtual void setPixel( WORD color );

  private:
    //---------------------------------------------------------------
    __IO uint16_t *ptr;
    __IO uint16_t *ptrStart;
    DWORD          offset;

  protected:
    //---------------------------------------------------------------

    //---------------------------------------------------------------
    WORD ax;
    WORD ay;
    WORD aw;
    WORD ah;
    WORD xpos;
    WORD ypos;

}; //class DisplayGraphic_GRAM

}  } //namespace

#endif
