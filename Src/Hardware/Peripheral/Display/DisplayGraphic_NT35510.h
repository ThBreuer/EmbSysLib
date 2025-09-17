//*******************************************************************
/*!
\file   DisplayGraphic_NT35510.h
\author Thomas Breuer
\date   11.09.2025
*/

//*******************************************************************
#ifndef _HW_DISPLAY_GRAPHIC_NT35510_H
#define _HW_DISPLAY_GRAPHIC_NT35510_H

//*******************************************************************
#include "Hardware/Common/Port/Port.h"
#include "Hardware/Common/Display/DisplayChar.h"
#include "Hardware/Common/Display/DisplayGraphic.h"
#include "Src/Dsi_Mcu.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class DisplayGraphic_NT35510

\brief LCD display NT35510

\example  HwDisplayGraphic.cpp
*/
class DisplayGraphic_NT35510 : public DisplayGraphic
{
  public:
    //---------------------------------------------------------------
    typedef enum
    {
      PORTRAIT     = 0x00,
      LANDSCAPE    = 0x01,
      NORMAL       = 0x00,
      ROTATED      = 0x02,
      PORTRAIT_0   = PORTRAIT  | NORMAL,
      LANDSCAPE_90 = LANDSCAPE | NORMAL,
      PORTRAIT_180 = PORTRAIT  | ROTATED,
      LANDSCAPE_270= LANDSCAPE | ROTATED
    } Orientation;

  protected:
    //---------------------------------------------------------------
    typedef enum
    {
    /* List of NT35510 used commands                                  */
    /* Details in NT35510 Data Sheet 'NT35510 - General Specification'*/
    /* Version of 11/8/2010                                           */
      CMD_NOP         = 0x00,  // NOP command
      CMD_SLPOUT      = 0x11,  // Sleep Out command
      CMD_DISPOFF     = 0x28,  // Display Off command
      CMD_DISPON      = 0x29,  // Display On command
      CMD_CASET       = 0x2A,  // Column address set command
      CMD_PASET       = 0x2B,  // Page address set command
      CMD_RAMWR       = 0x2C,  // Memory (GRAM) write command
      
      CMD_TEOFF       = 0x34,  // Tearing effect line off
      CMD_TEEON       = 0x35,  // Tearing effect line on

      CMD_MADCTR      = 0x36,  // Memory Access write control command
      CMD_COLMOD      = 0x3A,  // Interface Pixel Format
      CMD_RAMWRCNT    = 0x3C,  // Memory (GRAM) write continue
      CMD_WRDISBV     = 0x51,  // Write Display Brightness command
      CMD_WRCTRLD     = 0x53,  // Write CTRL Display command
      CMD_WRCABC      = 0x55,  // Write Content Adaptive Brightness command
      CMD_WRCABCMB    = 0x5E,  // Write CABC Minimum Brightness command

      PANSET          = 0xB3A6,
      SD_CTRL         = 0xC0A2,
      P_DRV_M         = 0xC0B4,
      OSC_ADJ         = 0xC181,
      RGB_VIDEO_SET   = 0xC1A1,
      SD_PCH_CTRL     = 0xC480,
      NO_DOC1         = 0xC48A,
      PWR_CTRL1       = 0xC580,
      PWR_CTRL2       = 0xC590,
      PWR_CTRL4       = 0xC5B0,
      PWM_PARA3       = 0xC6B1,
      PANCTRLSET1     = 0xCB80,
      PANCTRLSET2     = 0xCB90,
      PANCTRLSET3     = 0xCBA0,
      PANCTRLSET4     = 0xCBB0,
      PANCTRLSET5     = 0xCBC0,
      PANCTRLSET6     = 0xCBD0,
      PANCTRLSET7     = 0xCBE0,
      PANCTRLSET8     = 0xCBF0,
      PANU2D1         = 0xCC80,
      PANU2D2         = 0xCC90,
      PANU2D3         = 0xCCA0,
      PAND2U1         = 0xCCB0,
      PAND2U2         = 0xCCC0,
      PAND2U3         = 0xCCD0,
      GOAVST          = 0xCE80,
      GOACLKA1        = 0xCEA0,
      GOACLKA3        = 0xCEB0,
      GOAECLK         = 0xCFC0,
      NO_DOC2         = 0xCFD0,
      GVDDSET         = 0xD800,
      VCOMDC          = 0xD900,
      GMCT2_2P        = 0xE100,
      GMCT2_2N        = 0xE200,
      NO_DOC3         = 0xF5B6,
      CMD2_ENA1       = 0xFF00,
      CMD2_ENA2       = 0xFF80,

    } Cmd_type;

  protected:
    //---------------------------------------------------------------
    DisplayGraphic_NT35510( Dsi_Mcu     &hwDSI,
                            Port::Pin   &resetPin,
                            Orientation  orientation,
                            Font         font,
                            BYTE         zoom = 1 )

    : DisplayGraphic( ReportID_Hw::Module::DISPLAY_GRAPHIC_NT35510,
                      (orientation & LANDSCAPE)?lcd_x_size:lcd_y_size,
                      (orientation & LANDSCAPE)?lcd_y_size:lcd_x_size,
                      font,
                      zoom )
    , hwDSI( hwDSI )
    {
      this->orientation = orientation;

      if( orientation & LANDSCAPE )
      {
        Hsize = lcd_x_size;
        Vsize = lcd_y_size;
      }
      else
      {
        Hsize = lcd_y_size;
        Vsize = lcd_x_size;
      }

      ax = 0;
      ay = 0;
      aw = 0;
      ah = 0;
      xpos = 0;
      ypos = 0;

      // Reset LCD
      resetPin.setMode( Port::OutPP );
      resetPin.clr();
      System::delayMilliSec(20);
      resetPin.set();
      System::delayMilliSec(20);
    }

    //---------------------------------------------------------------
    void init();

  protected:
    //---------------------------------------------------------------
    Dsi_Mcu     &hwDSI;
    Orientation  orientation;

    //---------------------------------------------------------------
    WORD ax;
    WORD ay;
    WORD aw;
    WORD ah;
    WORD xpos;
    WORD ypos;

    //---------------------------------------------------------------
    static const uint32_t lcd_x_size = 800;
    static const uint32_t lcd_y_size = 480;

}; //class DisplayGraphic_NT35510

//*******************************************************************
/*!
\class DisplayGraphic_NT35510cmd

\brief LCD display NT35510

\example  HwDisplayGraphic.cpp
*/
class DisplayGraphic_NT35510cmd : public DisplayGraphic_NT35510
{
  public:
    //---------------------------------------------------------------
    DisplayGraphic_NT35510cmd( Dsi_Mcu      &hwDSI,
                               Port::Pin    &resetPin,
                               Orientation   orientation,
                               Font          font,
                               BYTE          zoom = 1 );

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
                           WORD y      );

    //---------------------------------------------------------------
    virtual void setPixel( WORD color );

  private:
    BYTE flag;

}; //class DisplayGraphic_NT35510cmd

//*******************************************************************
/*!
\class DisplayGraphic_NT35510ram

\brief LCD display NT35510

\example  HwDisplayGraphic.cpp
*/
class DisplayGraphic_NT35510ram : public DisplayGraphic_NT35510
{
  public:
    //---------------------------------------------------------------
    DisplayGraphic_NT35510ram( Dsi_Mcu      &hwDSI,
                               Port::Pin    &resetPin,
                               Orientation   orientation,
                               Font          font,
                               BYTE          zoom = 1 );

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

}; //class DisplayGraphic_NT35510ram

}  } //namespace

#endif
