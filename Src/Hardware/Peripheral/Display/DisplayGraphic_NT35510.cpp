//*******************************************************************
/*!
\file   DisplayGraphic_NT35510.cpp
\author Thomas Breuer
\date   11.09.2025

\see GitHub: linux/drivers/gpu/drm/panel/panel-orisetech-otm8009a.c
     https://www.keil.com/boards2/stmicroelectronics/stm32f746g_discovery, Keil.STM32F7xx_DFP.2.9.0.pack
*/

//*******************************************************************
#include "DisplayGraphic_NT35510.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// DisplayGraphic_NT35510
//
//*******************************************************************
//---------------------------------------------------------------
#define WRITE_SYS_CMD( cmd, para ) \
  { hwDSI.writeShort(cmd,para);    }

//---------------------------------------------------------------
#define WRITE_SYS_ARRAY( cmd,... )                \
  { const BYTE data[] = {cmd, __VA_ARGS__};       \
    hwDSI.writeLong( (BYTE*)data, sizeof(data) ); }

//---------------------------------------------------------------
#define WRITE_CUSTOMER_ARRAY( cmd,... )            \
  { const BYTE data[] = { (cmd)>>8,__VA_ARGS__};   \
    hwDSI.writeShort( CMD_NOP, (cmd)&0xFF);        \
    hwDSI.writeLong( (BYTE*)data, sizeof(data)  ); }

//---------------------------------------------------------------
void DisplayGraphic_NT35510::init( )
{
//  WRITE_SYS_ARRAY( 0xF0, 0x55, 0xAA, 0x52, 0x08, 0x01);/* LV2:  Page 1 enable */
//  WRITE_SYS_ARRAY( 0xB0, 0x03, 0x03, 0x03);/* AVDD: 5.2V */
//  WRITE_SYS_ARRAY( 0xB6, 0x46, 0x46, 0x46);/* AVDD: Ratio */
//  WRITE_SYS_ARRAY( 0xB1, 0x03, 0x03, 0x03);/* AVEE: -5.2V */
//  WRITE_SYS_ARRAY( 0xB7, 0x36, 0x36, 0x36);/* AVEE: Ratio */
//  WRITE_SYS_ARRAY( 0xB2, 0x00, 0x00, 0x02);/* VCL: -2.5V */
//  WRITE_SYS_ARRAY( 0xB8, 0x26, 0x26, 0x26);/* VCL: Ratio */
//  WRITE_SYS_CMD  ( 0xBF, 0x01);/* VGH: 15V (Free Pump) */
//  WRITE_SYS_ARRAY( 0xB3, 0x09, 0x09, 0x09);
//  WRITE_SYS_ARRAY( 0xB9, 0x36, 0x36, 0x36);/* VGH: Ratio */
//  WRITE_SYS_ARRAY( 0xB5, 0x08, 0x08, 0x08);/* VGL_REG: -10V */
//  WRITE_SYS_ARRAY( 0xBA, 0x26, 0x26, 0x26);/* VGLX: Ratio */
//  WRITE_SYS_ARRAY( 0xBC, 0x00, 0x80, 0x00);/* VGMP/VGSP: 4.5V/0V */
//  WRITE_SYS_ARRAY( 0xBD, 0x00, 0x80, 0x00);/* VGMN/VGSN:-4.5V/0V */
//  WRITE_SYS_ARRAY( 0xBE, 0x00, 0x50);/* VCOM: -1.325V */
//
///* ************************************************************************** */
///* Proprietary DCS Initialization                                             */
///* ************************************************************************** */
//  WRITE_SYS_ARRAY( 0xF0, 0x55, 0xAA, 0x52, 0x08, 0x00);/* LV2: Page 0 enable */
//  WRITE_SYS_ARRAY( 0xB1, 0xFC, 0x00);/* Display control */
//  WRITE_SYS_CMD  ( 0xB6, 0x03);/* Src hold time */
//  WRITE_SYS_CMD  ( 0xB5, 0x50);
//  WRITE_SYS_ARRAY( 0xB7, 0x00, 0x00);/* Gate EQ control */
//  WRITE_SYS_ARRAY( 0xB8, 0x01, 0x02, 0x02, 0x02);/* Src EQ control(Mode2) */
//  WRITE_SYS_ARRAY( 0xBC, 0x00, 0x00, 0x00);/* Inv. mode(2-dot) */
//  WRITE_SYS_ARRAY( 0xCC, 0x03, 0x00, 0x00);
//  WRITE_SYS_CMD  ( 0xBA, 0x01);

  System::delayMilliSec(20);

  WRITE_SYS_ARRAY( CMD_SLPOUT, 0x00); // Sleep out
  System::delayMilliSec(12);          // Wait for sleep out exit

  WRITE_SYS_ARRAY( CMD_TEOFF, 0x00);  // Tear off

  WRITE_SYS_CMD( CMD_COLMOD, 0x55 );  // RGB565

  switch( orientation )
  {
    default:                                      //MY     MX     MV
    case PORTRAIT_0:    WRITE_SYS_CMD( CMD_MADCTR, (0<<7)|(0<<6)|(0<<5) ); break;
    case PORTRAIT_180:  WRITE_SYS_CMD( CMD_MADCTR, (1<<7)|(1<<6)|(0<<5) ); break;
    case LANDSCAPE_90:  WRITE_SYS_CMD( CMD_MADCTR, (0<<7)|(1<<6)|(1<<5) ); break;
    case LANDSCAPE_270: WRITE_SYS_CMD( CMD_MADCTR, (1<<7)|(0<<6)|(1<<5) ); break;
  }

  //                                       Start-Addr          ,  End-Addr
  WRITE_SYS_ARRAY( CMD_CASET, 0x00, 0x00,  (BYTE)((Hsize-1)>>8), (BYTE)((Hsize-1)&0xFF) );
  WRITE_SYS_ARRAY( CMD_PASET, 0x00, 0x00,  (BYTE)((Vsize-1)>>8), (BYTE)((Vsize-1)&0xFF) );
  
  WRITE_SYS_CMD( CMD_WRDISBV,  0x7F ); // Display Brightness: medium
  WRITE_SYS_CMD( CMD_WRCTRLD,  0x2C ); // Control Display: Brightness Control Block: On,
                                       //                  Display Dimming: on
                                       //                  BackLight: on
  WRITE_SYS_CMD( CMD_WRCABC,   0x02 ); // Content Adaptive Brightness: Still Picture
  WRITE_SYS_CMD( CMD_WRCABCMB, 0x7F ); // CABC Minimum Brightness: maximum

  WRITE_SYS_CMD( CMD_DISPOFF, 0x00 );  // Display Off
  WRITE_SYS_CMD( CMD_NOP,    0x00 );   // NOP command
}

//*******************************************************************
//
// DisplayGraphic_NT35510cmd
//
//*******************************************************************
//---------------------------------------------------------------
DisplayGraphic_NT35510cmd::DisplayGraphic_NT35510cmd( Dsi_Mcu     &hwDSI,
                                                        Port::Pin   &resetPin,
                                                        Orientation  orientation,
                                                        Font         font,
                                                        BYTE         zoom )

: DisplayGraphic_NT35510( hwDSI, resetPin, orientation, font, zoom )
{
  flag = 0;

  this->hwDSI.Init(Hsize, Vsize, false);

  DisplayGraphic_NT35510::init( );

  WRITE_SYS_CMD( CMD_DISPON, 0x00);
  this->hwDSI.DisplayRefresh();
}

//---------------------------------------------------------------
void DisplayGraphic_NT35510cmd::refresh( void )
{
}

//---------------------------------------------------------------
void DisplayGraphic_NT35510cmd::clear( void )
{
  setArea(0,0,Hsize,Vsize);
  for(int x=0;x<Vsize;x++)
    for(int y=0;y<Hsize;y++)
      setPixel(BackColor);
}

//---------------------------------------------------------------
void DisplayGraphic_NT35510cmd::setArea( WORD x,
                                          WORD y,
                                          WORD w,
                                          WORD h )
{
  ax = x;
  ay = y;
  aw = w;
  ah = h;
  xpos = ax;
  ypos = ay;
  flag = 1;
}

//---------------------------------------------------------------
void DisplayGraphic_NT35510cmd::setPixel( WORD color,
                                           WORD x,
                                           WORD y  )
{
  WRITE_SYS_ARRAY( CMD_CASET, (BYTE)(x>>8), (BYTE)(x&0xFF), (BYTE)(x>>8), (BYTE)(x&0xFF) );
  WRITE_SYS_ARRAY( CMD_PASET, (BYTE)(y>>8), (BYTE)(y&0xFF), (BYTE)(y>>8), (BYTE)(y&0xFF) );

  WRITE_SYS_ARRAY( CMD_RAMWR, (BYTE)(color>>8), (BYTE)(color>>3), (BYTE)(color<<3) );
}

//---------------------------------------------------------------
void DisplayGraphic_NT35510cmd::setPixel( WORD color )
{
  WORD x = xpos;
  WORD y = ypos;

  if( flag == 1 )
  {
    WRITE_SYS_ARRAY( CMD_CASET, (BYTE)(x>>8), (BYTE)(x&0xFF), (BYTE)((x+aw-1)>>8), (BYTE)((x+aw-1)&0xFF) );
    WRITE_SYS_ARRAY( CMD_PASET, (BYTE)(y>>8), (BYTE)(y&0xFF), (BYTE)((y+ah-1)>>8), (BYTE)((y+ah-1)&0xFF) );

    WRITE_SYS_ARRAY( CMD_RAMWR, (BYTE)(color>>8), (BYTE)(color>>3), (BYTE)(color<<3) );
    flag = 0;
  }
  else
  {
    WRITE_SYS_ARRAY(CMD_RAMWRCNT, (BYTE)(color>>8), (BYTE)(color>>3), (BYTE)(color<<3) );
  }

  xpos++;
  if( xpos >= ax+aw)
  {
    xpos = ax;
    ypos++;
  }
}

//*******************************************************************
//
// DisplayGraphic_NT35510ram
//
//*******************************************************************
//---------------------------------------------------------------
DisplayGraphic_NT35510ram::DisplayGraphic_NT35510ram( Dsi_Mcu     &hwDSI,
                                                        Port::Pin   &resetPin,
                                                        Orientation  orientation,
                                                        Font         font,
                                                        BYTE         zoom )

: DisplayGraphic_NT35510( hwDSI, resetPin, orientation, font, zoom )
{
   this->hwDSI.Init( Hsize, Vsize, true );

  DisplayGraphic_NT35510::init( );

  ptrStart = (__IO uint16_t *)this->hwDSI.startLTDC( Hsize, Vsize );
  ptr      = ptrStart;

  WRITE_SYS_CMD( CMD_DISPON, 0x00);
  this->hwDSI.DisplayRefresh();
}

//---------------------------------------------------------------
void DisplayGraphic_NT35510ram::refresh( void )
{
  hwDSI.DisplayRefresh();
}

//---------------------------------------------------------------
void DisplayGraphic_NT35510ram::clear( void )
{
  ptr = ptrStart;
  __IO uint16_t *ptrEnd = ptr + Hsize*Vsize*sizeof(uint16_t);
  while( ptr < ptrEnd )
    *ptr++ = BackColor;
}

//---------------------------------------------------------------
void DisplayGraphic_NT35510ram::setArea( WORD x,
                                          WORD y,
                                          WORD w,
                                          WORD h )
{
  ax = x;
  ay = y;
  aw = w;
  ah = h;
  xpos   = ax;
  ypos   = ay*Hsize;
  offset = (DWORD)y*Hsize + x;

  ptr = ptrStart + offset;
}

//---------------------------------------------------------------
void DisplayGraphic_NT35510ram::setPixel( WORD color,
                                           WORD x,
                                           WORD y )
{
   ptr = ptrStart + (DWORD)y*Hsize + x;
  *ptr = color;
}

//---------------------------------------------------------------
void DisplayGraphic_NT35510ram::setPixel( WORD color )
{
  *ptr++ = color;
  xpos++;
  if( xpos >= ax+aw)
  {
    xpos = ax;
    ypos++;
    offset += Hsize;
    ptr = ptrStart + offset;
  }
}

}  } //namespace

//EOF
