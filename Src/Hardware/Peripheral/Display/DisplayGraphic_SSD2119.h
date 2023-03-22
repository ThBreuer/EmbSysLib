//*******************************************************************
/*!
\file   DisplayGraphic_SSD2119.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   10.03.2011

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_DISPLAY_GRAPHIC_SSD2119_H
#define _HW_DISPLAY_GRAPHIC_SSD2119_H

//*******************************************************************
#include "Hardware/Common/Port/Port.h"
#include "Hardware/Common/Display/DisplayGraphic.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*! 
\class DisplayGraphic_SSD2119

\brief LCD display SSD2119

\example  HwDisplayGraphic.cpp
*/
class DisplayGraphic_SSD2119 : public DisplayGraphic
{
  public:

    //---------------------------------------------------------------
    /*! Initialize display        
        \param baseAddr Memory base address
        \param pinReset Reset pin
        \param pinBL    Backlight pin
        \param font     Display font 
        \param zoom     Display font zoom size (1,2,3,...)
    */
    DisplayGraphic_SSD2119( BYTE       *baseAddr,
                            Port::Pin  &pinReset,
                            Port::Pin  &pinBL,
                            Font        font, 
                            BYTE        zoom = 1 );
   
    //---------------------------------------------------------------
    virtual void clear( void );

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
    // Initialize LCD
    void init( void );
      
    //---------------------------------------------------------------
    // Write a command with value
    void writeCmd( BYTE reg, WORD value );

    //---------------------------------------------------------------
    // Write to a command
    void writeCmd( BYTE reg );

    //---------------------------------------------------------------
    // Write data
    void writeDat( WORD dat );

  private:
    //---------------------------------------------------------------
    typedef enum
    {
       OSC_START          = 0x00,
       OUTPUT_CTRL        = 0x01,
       LCD_DRIVE_AC_CTRL  = 0x02,
       DISPLAY_CTRL       = 0x07,
       PWR_CTRL_2         = 0x0C,
       PWR_CTRL_3         = 0x0D,
       PWR_CTRL_4         = 0x0E,
       SLEEP_MODE_1       = 0x10,
       ENTRY_MODE         = 0x11,
       SLEEP_MODE_2       = 0x12,
       PWR_CTRL_5         = 0x1E,
       RAM_DATA           = 0x22,
       ANALOG_SET         = 0x26,
       VCOM_OTP_1         = 0x28,
       GAMMA_CTRL_1       = 0x30,
       GAMMA_CTRL_2       = 0x31,
       GAMMA_CTRL_3       = 0x32,
       GAMMA_CTRL_4       = 0x33,
       GAMMA_CTRL_5       = 0x34,
       GAMMA_CTRL_6       = 0x35,
       GAMMA_CTRL_7       = 0x36,
       GAMMA_CTRL_8       = 0x37,
       GAMMA_CTRL_9       = 0x3A,
       GAMMA_CTRL_10      = 0x3B,
       V_RAM_POS          = 0x44,
       H_RAM_START        = 0x45,
       H_RAM_END          = 0x46,
       X_RAM_ADDR         = 0x4E,
       Y_RAM_ADDR         = 0x4F

    } Reg;  

  private:
    //---------------------------------------------------------------
    // 
    // configuration:
    //
    static const WORD   WIDTH  = 320;
    static const WORD   HEIGHT = 240;

    volatile WORD      *ptrCmd;
    volatile WORD      *ptrData;
    Port::Pin          &reset;

}; //class DisplayGraphic_SSD2119


}  } //namespace

#endif
