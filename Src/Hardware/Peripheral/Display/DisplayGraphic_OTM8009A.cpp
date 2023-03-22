//*******************************************************************
/*!
\file   DisplayGraphic_OTM8009A.cpp
\author Thomas Breuer
\date   27.11.2018

\see GitHub: linux/drivers/gpu/drm/panel/panel-orisetech-otm8009a.c
     https://www.keil.com/boards2/stmicroelectronics/stm32f746g_discovery, Keil.STM32F7xx_DFP.2.9.0.pack
*/

//*******************************************************************
#include "DisplayGraphic_OTM8009A.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// DisplayGraphic_OTM8009A
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
void DisplayGraphic_OTM8009A::init( )
{
                                                       // Enable CMD2 to access vendor specific commands
  WRITE_CUSTOMER_ARRAY( CMD2_ENA1,   0x80,0x09,0x01 ); // Enter in command 2 mode and set EXTC to enable address shift function (0x00)
  WRITE_CUSTOMER_ARRAY( CMD2_ENA2,   0x80,0x09      ); // Enter ORISE Command 2: Shift address to 0x80
  WRITE_CUSTOMER_ARRAY( SD_PCH_CTRL, 0x30           ); // SD_PT: Source output level during porch and non-display area to GND
  WRITE_CUSTOMER_ARRAY( NO_DOC1,     0x40           ); // undocumented!

  System::delayMilliSec(10);

  WRITE_CUSTOMER_ARRAY( PWR_CTRL4 + 1, 0xA9); // gvdd_en_test: enable GVDD test mode
  WRITE_CUSTOMER_ARRAY( PWR_CTRL2 + 1, 0x34); // pump 4 vgh voltage: 15.0v down to 13.0v
                                              // pump 5 vgh voltage: -12.0v downto -9.0v
  WRITE_CUSTOMER_ARRAY( P_DRV_M,       0x50); // Column inversion
  WRITE_CUSTOMER_ARRAY( VCOMDC,        0x4E); // VCOM Voltage: -1.0000v downto -1.2625v
  WRITE_CUSTOMER_ARRAY( OSC_ADJ,       0x66); // Oscillator adjustment for Idle/Normal mode (LPDT only) set to 65Hz (default is 60Hz)
  WRITE_CUSTOMER_ARRAY( RGB_VIDEO_SET, 0x08); // Video mode internal
  WRITE_CUSTOMER_ARRAY( PWR_CTRL2 + 2, 0x01); // pump 4&5 x6: ONLY VALID when PUMP4_EN_ASDM_HV = "0"
  WRITE_CUSTOMER_ARRAY( PWR_CTRL2 + 5, 0x36); // pump4 clock ratio: 1/2 line
  WRITE_CUSTOMER_ARRAY( GVDDSET,       0x79,  // GVDD/NGVDD settings
                                       0x79);
  WRITE_CUSTOMER_ARRAY( PWR_CTRL2 + 4, 0x33); // Rewrite the default value !
  WRITE_CUSTOMER_ARRAY( SD_CTRL + 1,   0x1B); // Panel display timing Setting 3
  WRITE_CUSTOMER_ARRAY( PWR_CTRL1 + 2, 0x83); // Power control 1
  WRITE_CUSTOMER_ARRAY( SD_PCH_CTRL +1,0x83); // Source driver precharge
  WRITE_CUSTOMER_ARRAY( RGB_VIDEO_SET, 0x0E);
  WRITE_CUSTOMER_ARRAY( PANSET,        0x00,0x01);

  WRITE_CUSTOMER_ARRAY( GOAVST,        0x85,0x01,0x00,0x84,0x01,0x00);
  WRITE_CUSTOMER_ARRAY( GOACLKA1,      0x18,0x04,0x03,0x39,0x00,0x00,0x00,0x18,0x03,0x03,0x3A,0x00,0x00,0x00);
  WRITE_CUSTOMER_ARRAY( GOACLKA3,      0x18,0x02,0x03,0x3B,0x00,0x00,0x00,0x18,0x01,0x03,0x3C,0x00,0x00,0x00);
  WRITE_CUSTOMER_ARRAY( GOAECLK,       0x01,0x01,0x20,0x20,0x00,0x00,0x01,0x02,0x00,0x00);

  WRITE_CUSTOMER_ARRAY( NO_DOC2,       0x00); // undocumented!

  WRITE_CUSTOMER_ARRAY( PANCTRLSET1,   0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);
  WRITE_CUSTOMER_ARRAY( PANCTRLSET2,   0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);
  WRITE_CUSTOMER_ARRAY( PANCTRLSET3,   0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);
  WRITE_CUSTOMER_ARRAY( PANCTRLSET4,   0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);
  WRITE_CUSTOMER_ARRAY( PANCTRLSET5,   0x00,0x04,0x04,0x04,0x04,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);
  WRITE_CUSTOMER_ARRAY( PANCTRLSET6,   0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x04,0x04,0x04,0x04,0x00,0x00,0x00,0x00);
  WRITE_CUSTOMER_ARRAY( PANCTRLSET7,   0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);
  WRITE_CUSTOMER_ARRAY( PANCTRLSET8,   0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF);
  WRITE_CUSTOMER_ARRAY( PANU2D1,       0x00,0x26,0x09,0x0B,0x01,0x25,0x00,0x00,0x00,0x00);
  WRITE_CUSTOMER_ARRAY( PANU2D2,       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x26,0x0A,0x0C,0x02);
  WRITE_CUSTOMER_ARRAY( PANU2D3,       0x25,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);
  WRITE_CUSTOMER_ARRAY( PAND2U1,       0x00,0x25,0x0C,0x0A,0x02,0x26,0x00,0x00,0x00,0x00);
  WRITE_CUSTOMER_ARRAY( PAND2U2,       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x25,0x0B,0x09,0x01);
  WRITE_CUSTOMER_ARRAY( PAND2U3,       0x26,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);

  WRITE_CUSTOMER_ARRAY( PWR_CTRL1 + 1, 0x66); // Pump 1 min and max DM

  WRITE_CUSTOMER_ARRAY( PWM_PARA3,       32); // PWM-Freq, DBF=4,141 kHz (if PWM_FREQ_SEL[1:0]=01)

  WRITE_CUSTOMER_ARRAY( NO_DOC3,       0x06); // undocumented, but important
  WRITE_CUSTOMER_ARRAY( CMD2_ENA1,     0xFF,  // Exit CMD2 mode
                                       0xFF,0xFF);

  WRITE_SYS_CMD( CMD_NOP, 0x00 ); // NOP - goes back to DCS std command ?

  WRITE_CUSTOMER_ARRAY( GMCT2_2P,  0x00,0x09,0x0F,0x0E,0x07,0x10,0x0B,0x0A,0x04,0x07,0x0B,0x08,0x0F,0x10,0x0A,0x01);
  WRITE_CUSTOMER_ARRAY( GMCT2_2N,  0x00,0x09,0x0F,0x0E,0x07,0x10,0x0B,0x0A,0x04,0x07,0x0B,0x08,0x0F,0x10,0x0A,0x01);

  WRITE_SYS_CMD( CMD_SLPOUT, 0x00 ); // Send Sleep Out command to display : no parameter

  System::delayMilliSec(120);

  WRITE_SYS_CMD( CMD_COLMOD, 0x77 ); // RGB888, MCU: 24bit/pixel, RGB:24bit/pixel,1-times


  switch( orientation )
  {
    default:                                      //MY     MX     MV
    case PORTRAIT_0:    WRITE_SYS_CMD( CMD_MADCTR, (0<<7)|(0<<6)|(0<<5) ); break;
    case PORTRAIT_180:  WRITE_SYS_CMD( CMD_MADCTR, (1<<7)|(1<<6)|(0<<5) ); break;
    case LANDSCAPE_90:  WRITE_SYS_CMD( CMD_MADCTR, (0<<7)|(1<<6)|(1<<5) ); break;
    case LANDSCAPE_270:  WRITE_SYS_CMD( CMD_MADCTR, (1<<7)|(0<<6)|(1<<5) ); break;
  }
                           // Start-Addr,  End-Addr
  WRITE_SYS_ARRAY( CMD_CASET, 0x00, 0x00,  (BYTE)((Hsize-1)>>8), (BYTE)((Hsize-1)&0xFF) );
  WRITE_SYS_ARRAY( CMD_PASET, 0x00, 0x00,  (BYTE)((Vsize-1)>>8), (BYTE)((Vsize-1)&0xFF) );

  WRITE_SYS_CMD( CMD_WRDISBV,  0x7F ); // Display Brightness: medium
  WRITE_SYS_CMD( CMD_WRCTRLD,  0x2C ); // Control Display: Brightness Control Block: On,
                                       //                  Display Dimming: on
                                       //                  BackLight: on
  WRITE_SYS_CMD( CMD_WRCABC,   0x02 ); // Content Adaptive Brightness: Still Picture
  WRITE_SYS_CMD( CMD_WRCABCMB, 0x7F ); // CABC Minimum Brightness: maximum

  WRITE_SYS_CMD( CMD_DISPOFF, 0x00 ); // Display Off
  WRITE_SYS_CMD( CMD_NOP,    0x00 ); // NOP command
}

//*******************************************************************
//
// DisplayGraphic_OTM8009Acmd
//
//*******************************************************************
//---------------------------------------------------------------
DisplayGraphic_OTM8009Acmd::DisplayGraphic_OTM8009Acmd( Dsi_Mcu     &hwDSI,
                                                        Port::Pin   &resetPin,
                                                        Orientation  orientation,
                                                        Font         font,
                                                        BYTE         zoom )

: DisplayGraphic_OTM8009A( hwDSI, resetPin, orientation, font, zoom )
{
  flag = 0;

  this->hwDSI.Init(Hsize, Vsize, false);

  DisplayGraphic_OTM8009A::init( );

  WRITE_SYS_CMD( CMD_DISPON, 0x00);
  this->hwDSI.DisplayRefresh();
}

//---------------------------------------------------------------
void DisplayGraphic_OTM8009Acmd::refresh( void )
{
}

//---------------------------------------------------------------
void DisplayGraphic_OTM8009Acmd::clear( void )
{
  setArea(0,0,Hsize,Vsize);
  for(int x=0;x<Vsize;x++)
    for(int y=0;y<Hsize;y++)
      setPixel(BackColor);
}

//---------------------------------------------------------------
void DisplayGraphic_OTM8009Acmd::setArea( WORD x,
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
void DisplayGraphic_OTM8009Acmd::setPixel( WORD color,
                                           WORD x,
                                           WORD y  )
{
  WRITE_SYS_ARRAY( CMD_CASET, (BYTE)(x>>8), (BYTE)(x&0xFF), (BYTE)(x>>8), (BYTE)(x&0xFF) );
  WRITE_SYS_ARRAY( CMD_PASET, (BYTE)(y>>8), (BYTE)(y&0xFF), (BYTE)(y>>8), (BYTE)(y&0xFF) );

  WRITE_SYS_ARRAY( CMD_RAMWR, (BYTE)(color>>8), (BYTE)(color>>3), (BYTE)(color<<3) );
}

//---------------------------------------------------------------
void DisplayGraphic_OTM8009Acmd::setPixel( WORD color )
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
// DisplayGraphic_OTM8009Aram
//
//*******************************************************************
//---------------------------------------------------------------
DisplayGraphic_OTM8009Aram::DisplayGraphic_OTM8009Aram( Dsi_Mcu     &hwDSI,
                                                        Port::Pin   &resetPin,
                                                        Orientation  orientation,
                                                        Font         font,
                                                        BYTE         zoom )

: DisplayGraphic_OTM8009A( hwDSI, resetPin, orientation, font, zoom )
{
   this->hwDSI.Init( Hsize, Vsize, true );

  DisplayGraphic_OTM8009A::init( );

  ptrStart = (__IO uint16_t *)this->hwDSI.startLTDC( Hsize, Vsize );
  ptr      = ptrStart;

  WRITE_SYS_CMD( CMD_DISPON, 0x00);
  this->hwDSI.DisplayRefresh();
}

//---------------------------------------------------------------
void DisplayGraphic_OTM8009Aram::refresh( void )
{
  hwDSI.DisplayRefresh();
}

//---------------------------------------------------------------
void DisplayGraphic_OTM8009Aram::clear( void )
{
  ptr = ptrStart;
  __IO uint16_t *ptrEnd = ptr + Hsize*Vsize*sizeof(uint16_t);
  while( ptr < ptrEnd )
    *ptr++ = BackColor;
}

//---------------------------------------------------------------
void DisplayGraphic_OTM8009Aram::setArea( WORD x,
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
void DisplayGraphic_OTM8009Aram::setPixel( WORD color,
                                           WORD x,
                                           WORD y )
{
   ptr = ptrStart + (DWORD)y*Hsize + x;
  *ptr = color;
}

//---------------------------------------------------------------
void DisplayGraphic_OTM8009Aram::setPixel( WORD color )
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
