//*******************************************************************
/*!
\file   ModUSBkeyboard.cpp
\author Thomas Breuer
\date
\brief  Sample of using USB as HID keyboard
*/

//*******************************************************************
/*
Usage:    The USB device is connected as a keyboard (HID class).
          After driver installation a sequence of keys is transmitted to the PC.
          Attention: Make sure to focus on a suitable window.

          Then the state of the keyboard LEDs is displayed.
          Try "Caps Lock" and "Num Lock" on your PC keyboard.
*/

//*******************************************************************
#define USE_USB

//*******************************************************************
#include <stdio.h>
#include "EmbSysLib.h"
#include "Module/USB/USBinterfClassHID.h"

//-------------------------------------------------------------------
#include "ReportHandler.h"
#include "ModUSBkeyboard/descriptor.cpp"
#include "config.h"

//*******************************************************************
typedef enum
{
  NONE        = 0,
  LEFT_CTRl   = 0x01,
  LEFT_SHIFT  = 0x02,
  LEFT_ALT    = 0x04,
  LEFT_GUI    = 0x08,
  RIGHT_CTRL  = 0x10,
  RIGHT_SHIFT = 0x20,
  RIGHT_ALT   = 0x40,
  RIGHT_GUI   = 0x80,
} keycodeModifier;

// This is the list of scan codes that will be sent:
BYTE scancode[][2] =
{
  {0x2C,NONE      }, // <SPC> (ASCII 32)
  {0x1e,LEFT_SHIFT}, // !
  {0x1f,LEFT_SHIFT}, // "
  {0x31,NONE      }, // #
  {0x21,LEFT_SHIFT}, // $
  {0x22,LEFT_SHIFT}, // %
  {0x23,LEFT_SHIFT}, // &
  {0x31,LEFT_SHIFT}, // '
  {0x25,LEFT_SHIFT}, // (
  {0x26,LEFT_SHIFT}, // )
  {0x30,LEFT_SHIFT}, // *
  {0x30,NONE      }, // +
  {0x36,NONE      }, // ,
  {0x38,NONE      }, // -
  {0x37,NONE      }, // .
  {0x24,LEFT_SHIFT}, // /

  {0x27,NONE      }, // 0
  {0x1e,NONE      }, // 1
  {0x1f,NONE      }, // 2
  {0x20,NONE      }, // 3
  {0x21,NONE      }, // 4
  {0x22,NONE      }, // 5
  {0x23,NONE      }, // 6
  {0x24,NONE      }, // 7
  {0x25,NONE      }, // 8
  {0x26,NONE      }, // 9

  {0x37,LEFT_SHIFT}, // :
  {0x36,LEFT_SHIFT}, // ;
  {0x64,NONE      }, // <
  {0x27,LEFT_SHIFT}, // =
  {0x64,LEFT_SHIFT}, // >
  {0x2D,LEFT_SHIFT}, // ?
  {0x14,RIGHT_ALT }, // @

  {0x04,LEFT_SHIFT}, // A
  {0x05,LEFT_SHIFT}, // B
  {0x06,LEFT_SHIFT}, // C
  {0x07,LEFT_SHIFT}, // D
  {0x08,LEFT_SHIFT}, // E
  {0x09,LEFT_SHIFT}, // F
  {0x0a,LEFT_SHIFT}, // G
  {0x0b,LEFT_SHIFT}, // H
  {0x0c,LEFT_SHIFT}, // I
  {0x0d,LEFT_SHIFT}, // J
  {0x0e,LEFT_SHIFT}, // K
  {0x0f,LEFT_SHIFT}, // L
  {0x10,LEFT_SHIFT}, // M
  {0x11,LEFT_SHIFT}, // N
  {0x12,LEFT_SHIFT}, // O
  {0x13,LEFT_SHIFT}, // P
  {0x14,LEFT_SHIFT}, // Q
  {0x15,LEFT_SHIFT}, // R
  {0x16,LEFT_SHIFT}, // S
  {0x17,LEFT_SHIFT}, // T
  {0x18,LEFT_SHIFT}, // U
  {0x19,LEFT_SHIFT}, // V
  {0x1a,LEFT_SHIFT}, // W
  {0x1b,LEFT_SHIFT}, // X
  {0x1d,LEFT_SHIFT}, // Y
  {0x1c,LEFT_SHIFT}, // Z
  {0x25,RIGHT_ALT }, // [
  {0x2d,RIGHT_ALT }, // '\'
  {0x26,RIGHT_ALT }, // ]

  {0x35,NONE      }, // ^
  {0x38,LEFT_SHIFT}, // _
  {0x2e,LEFT_SHIFT}, // `

  {0x04,NONE       }, // a
  {0x05,NONE       }, // b
  {0x06,NONE       }, // c
  {0x07,NONE       }, // d
  {0x08,NONE       }, // e
  {0x09,NONE       }, // f
  {0x0a,NONE       }, // g
  {0x0b,NONE       }, // h
  {0x0c,NONE       }, // i
  {0x0d,NONE       }, // j
  {0x0e,NONE       }, // k
  {0x0f,NONE       }, // l
  {0x10,NONE       }, // m
  {0x11,NONE       }, // n
  {0x12,NONE       }, // o
  {0x13,NONE       }, // p
  {0x14,NONE       }, // q
  {0x15,NONE       }, // r
  {0x16,NONE       }, // s
  {0x17,NONE       }, // t
  {0x18,NONE       }, // u
  {0x19,NONE       }, // v
  {0x1a,NONE       }, // w
  {0x1b,NONE       }, // x
  {0x1d,NONE       }, // y
  {0x1c,NONE       }, // z
  {0x24,RIGHT_ALT  }, // {
  {0x64,RIGHT_ALT  }, // |
  {0x27,RIGHT_ALT  }, // }
  {0x30,RIGHT_ALT  },  // ~  (ASCII 126)

  {0x28,NONE       },  // <ENT>

};

  //-----------------------------------------------------------------
class cMyInterf : public USBinterfClassHID
{
  public:

    WORD cnt;
    //-----------------------------------------------------------------
    class __attribute__((packed)) cData
    {
      public:
        cData()
        {
          modifier = 0;
          reserved = 0;
          keycode[0] = 0;
          keycode[1] = 0;
          keycode[2] = 0;
          keycode[3] = 0;
          keycode[4] = 0;
          keycode[5] = 0;
        }

      public:
        BYTE modifier;
        BYTE reserved;
        BYTE keycode[6];
    };

  public:
    BYTE led[2];
    cData dataIN;
    BYTE nr;

    Flag<BYTE> isNew;

    //---------------------------------------------------------------
    cMyInterf( USBdevice &usb,
               BYTE       configId,
               BYTE       interfId )
    : USBinterfClassHID( usb, configId, interfId, DataPointer(dataIN), DataPointer(led,1) )
    {
      cnt=0;
      led[0]=0;
      nr=0;
    }

    //---------------------------------------------------------------
    virtual bool onReportTransmit()
    {
      if( nr > sizeof(scancode)/2 || dataIN.keycode[0] != 0 )
      {
        dataIN.keycode[0]=0;
        dataIN.modifier = 0;
      }
      else
      {
        dataIN.keycode[0] = scancode[nr][0];
        dataIN.modifier   = scancode[nr][1];
        nr++;
      }
      return( true );
    }

   //---------------------------------------------------------------
    virtual bool onReportReceive()
    {
      cnt++;
      isNew = true;
      return( true );
    }

}; //class cMyInterf

//*******************************************************************
int main(void)
{
  cMyInterf               interfaceHID( usb, 1/*configId*/, 0/*interfId*/ );
  usb.start();

  uart.set( "\r\n\nModUSBkeyboard," __DATE__ "," __TIME__ "\r\n\n" );
  uart.set( "\r\nwaiting ...." );

  while( !interfaceHID.isStarted ) {}

  uart.set( "\rstarted!    \r\n\n" );
  while(1)
  {
    if( interfaceHID.isNew.getEvent() )
    {
      // Show numlock und capslock, if activated on origin keyboard
      char str[32];
      sprintf(str,"led:%02x cnt:%u \r", interfaceHID.led[0], interfaceHID.cnt );
      uart.set(str);
    }
  }
}

//EOF
