//*******************************************************************
/*!
\file   USBdevice_Mcu.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"

\remark: see also STM32CubeF4 (see: http://www.st.com/web/en/catalog/tools/PF259243)

\todo   Review implementation
*/

//*******************************************************************
#include "USBdevice_Mcu.h"
#include "Hardware/MCU/System.h"
#include "Hardware/ReportID_Hw.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//!<\cond
typedef struct
{
  __IO uint32_t EP[8];        // Endpoint register
  __IO uint32_t reserved[8];  // Reserved Address
  __IO uint32_t CNTR;         // Control register
  __IO uint32_t ISTR;         // Interrupt status register
  __IO uint32_t FNR;          // Frame number register
  __IO uint32_t DADDR;        // Device address register
  __IO uint32_t BTABLE;       // Buffer table register

} USB_TypeDef;
//!<\endcond

//*******************************************************************
//!<\cond
#pragma pack(1)
typedef struct
{
    struct
    {
      volatile WORD addr;
      volatile WORD reserved2;
      volatile WORD count;
      volatile WORD reserved1;
    } bufDesc[16];

} USB_BufferDescriptionTable;
#pragma pack()
//!<\endcond

//*******************************************************************
#define USB_BASE             (APB1PERIPH_BASE + 0x5C00)
#define USB                  ((USB_TypeDef *) USB_BASE)

#define USB_BUF_BASE         (APB1PERIPH_BASE + 0x6000)
#define USB_BUF             ((USB_BufferDescriptionTable *) USB_BUF_BASE)

//*******************************************************************
//
// USBdevice_Mcu
//
//*******************************************************************
//-------------------------------------------------------------------
USBdevice_Mcu *USBdevice_Mcu::usbPtr = 0;
volatile WORD           USBdevice_Mcu::EPconfig::offset = sizeof(USB_BufferDescriptionTable);

//-------------------------------------------------------------------
USBdevice_Mcu::USBdevice_Mcu( USBdeviceDescriptor &desc )
: USBdevice( ReportID_Hw::Module::USB_DEVICE_MCU, desc )
{
//<! \todo PinConfig needed?
  usbPtr = this;
}

//-------------------------------------------------------------------
inline void USBdevice_Mcu::isr(void)
{
  volatile DWORD istr = USB->ISTR;

  if( istr & USB_ISTR_RESET )
  {
    reset();
    USB->ISTR = ~USB_ISTR_RESET;
  }

  if( istr & USB_ISTR_SUSP )
  {
    suspend();
    USB->CNTR |= USB_CNTR_FSUSP;
    USB->ISTR = ~USB_ISTR_SUSP;
  }

  if( istr & USB_ISTR_CTR )
  {
    for( BYTE num=0; num<8;num++) //<! \todo: use ISTR, EP_ID bits?
    {
      if( USB->EP[num]&USB_EP0R_CTR_RX )
      {
        EP[num].clrCTR_RX();
        eventHandler( num|0x00,                                   // calc EP address
                      (USB->EP[num]&USB_EP0R_SETUP)?true:false ); // Setup?
     }

      if( USB->EP[num]&USB_EP0R_CTR_TX )
      {
        EP[num].clrCTR_TX();
        eventHandler( num|0x80,  // calc EP address
                      false );   // no Setup
       }
    }
    USB->ISTR = ~USB_ISTR_CTR;
  }
}

//-------------------------------------------------------------------
void USBdevice_Mcu::start(void)
{
  reset();

  setAddress(0);

  NVIC_EnableIRQ(USB_LP_IRQn);      // enable USB interrupt
  SYSCFG->PMC |= SYSCFG_PMC_USB_PU; // switch on PU
}

//-------------------------------------------------------------------
void USBdevice_Mcu::reset (void)
{
  RCC->APB1ENR |= RCC_APB1ENR_USBEN;
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

  // power on
  // --------
  USB->CNTR = USB_CNTR_FRES;

  System::delayMicroSec(10);

  USB->CNTR = 0;


  USB->ISTR = 0; // Clear pending interrupts

  DWORD *ptr = (DWORD*)USB_BUF_BASE;
  for(int i=0;i<256;i++)
  {
    *(ptr++)=0;
  }

  setAddress(0);

  EPconfig::offset = sizeof(USB_BufferDescriptionTable);

  EP[0].init(  0x80/*addr*/, 1/*type*/, 0/*kind*/, 64/*size*/);
  EP[0].init(  0x00/*addr*/, 1/*type*/, 0/*kind*/, 64/*size*/);
  EP[0].setSTAT_RX( EPconfig::VALID );
  EP[0].setSTAT_TX( EPconfig::NAK );

  USB->CNTR =  // set interrupts mask
                  USB_CNTR_CTRM
               | !USB_CNTR_WKUPM
               |  USB_CNTR_SUSPM
               | !USB_CNTR_ERRM
               | !USB_CNTR_SOFM
               | !USB_CNTR_ESOFM
               |  USB_CNTR_RESETM;

  SYSCFG->PMC |= SYSCFG_PMC_USB_PU; // switch on PU
}

//-------------------------------------------------------------------
void USBdevice_Mcu::configure (BYTE flag)
{
}

//-------------------------------------------------------------------
void USBdevice_Mcu::setAddress( BYTE adr )
{
  writeEP( 0x80, NULL, 0 );

  System::delayMilliSec(1);

  USB->DADDR =   adr             // address
               | USB_DADDR_EF;

  EP[0].setSTAT_RX( EPconfig::VALID );
}

//-------------------------------------------------------------------
void USBdevice_Mcu::configEP (BYTE epAddr, WORD epMaxPacketSize, BYTE epType)
{
  BYTE num = epAddr&0x07;
  BYTE type[] = { 1/*CTRL*/, 2/*ISO*/, 0/*BULK*/, 3/*INT*/};
  if( epAddr & 0x80 ) // IN=TX
  {
    EP[num].init(  epAddr, type[epType&0x3]/*type*/, 0/*kind*/, epMaxPacketSize);

    if(num>0)
    {
      EP[num].setSTAT_TX( (epType==2)?EPconfig::NAK : EPconfig::NAK );
    }
    else
      EP[num].setSTAT_TX( (epType==2)?EPconfig::NAK : EPconfig::VALID );
      
    //                              ^^^  Bulk:2, Interrupt 3
    //<! \todo anders loesen!!!
  }
  else
  {
    EP[num].init(  epAddr, type[epType&0x3]/*type*/, 0/*kind*/, epMaxPacketSize);
    EP[num].setSTAT_RX( EPconfig::VALID );
  }
}

//---------------------------------------------------------------
void USBdevice_Mcu::setStallEP (BYTE epAddr)
{
  BYTE  num = epAddr & 0x07;
  //<! \todo: is this a mismatch:
  if( epAddr & 0x80 ) // IN, RX
    EP[num].setSTAT_RX( EPconfig::STALL );
  else
    EP[num].setSTAT_TX( EPconfig::STALL );
}

//---------------------------------------------------------------
void USBdevice_Mcu::clrStallEP (BYTE epAddr)
{
}

//---------------------------------------------------------------
void USBdevice_Mcu::setNAK_EP (BYTE epAddr)
{
BYTE  num = epAddr & 0x07;
if( epAddr & 0x80 ) // IN, RX
  EP[num].setSTAT_TX( EPconfig::NAK );
else
  EP[num].setSTAT_RX( EPconfig::NAK );
}

//---------------------------------------------------------------
void USBdevice_Mcu::clrNAK_EP (BYTE epAddr)
{
BYTE  num = epAddr & 0x07;
if( epAddr & 0x80 ) // IN, RX
  EP[num].setSTAT_TX( EPconfig::VALID );
else
  EP[num].setSTAT_RX( EPconfig::VALID);
}

//-------------------------------------------------------------------
WORD USBdevice_Mcu::readEP( BYTE  epAddr,
                            BYTE *dataPtr,
                            WORD  len )
{
  BYTE  num = epAddr & 0x07;
  volatile WORD *ptr = EP[num].rxPtr;

  if( !(epAddr & 0x80) && ptr )
  {
    WORD cnt = USB_BUF->bufDesc[2*num+1].count & 0x3FF; // count lesen
    cnt = (cnt<len)?cnt:len;

    for( WORD n = (cnt + 1) / 2; n>0; n-- )
    {
      *(( WORD *)dataPtr) = *ptr;
      dataPtr += 2;
      ptr     += 2;
    }
    EP[num].setSTAT_RX( EPconfig::VALID );

    return( cnt );
  }

  return( 0 );
}

//-------------------------------------------------------------------
WORD USBdevice_Mcu::writeEP( BYTE  epAddr,
                             BYTE *pData,
                             WORD  cnt )
{
  BYTE  num = epAddr & 0x07;
  volatile WORD *ptr = EP[num].txPtr;

  if( (epAddr & 0x80) && ptr)
  {
    for( WORD n = (cnt + 1)/2; n>0; n-- )
    {
      *ptr = *((WORD *)pData);
      pData += 2;
      ptr   += 2;
    }
    USB_BUF->bufDesc[2*num].count = cnt & 0x3FF; // count lesen

    EP[num].setSTAT_TX( EPconfig::VALID );
      
    return( cnt );
  }
  return( 0 );
}

//*******************************************************************
//
// USBdevice_Mcu::EPconfig
//
//*******************************************************************
//-------------------------------------------------------------------
WORD *USBdevice_Mcu::EPconfig::bdt_init( BYTE num,
                                         BYTE isRX,
                                         WORD size )
{
  WORD *ret = NULL;
  offset &= (~0x01);
  if(isRX)
  {
    if( size <= 62 )
    {
      USB_BUF->bufDesc[2*num+1].count =  (       0 <<15)
                                        |((size>>1)<<10);
    }
    else
    {
      USB_BUF->bufDesc[2*num+1].count =  (       1 <<15)
                                        |((size>>6)<<10);
    }
    USB_BUF->bufDesc[2*num+1].addr  = offset;
  }
  else
  {
    USB_BUF->bufDesc[2*num].count = 0;
    USB_BUF->bufDesc[2*num].addr  = offset;
  }
  ret = (WORD*)USB_BUF+offset;
  offset += size;

  return( ret );
}

//-------------------------------------------------------------------
void USBdevice_Mcu::EPconfig::init( DWORD addrIn,
                                    DWORD type,
                                    DWORD kind,
                                    WORD sizeIn )
{
  addr     = addrIn&0x07;

  regPtr   = (DWORD*)&(USB->EP[addr&0x07]);

  if( addrIn & 0x80 ) // IN=TX
  {
    txSize   = sizeIn;
    txPtr = bdt_init(addr,0, txSize );//TX
  }
  else
  {
    rxSize   = sizeIn;
    rxPtr = bdt_init(addr,1, rxSize );//OUT=RX

  }

  config   = (addr<<0)
            |(type<<9)
            |(kind<<8)
            |(1<<15) // CTR_RX
            |(1<< 7) // CTR_TX
            |(1<<11);// SETUP

  *regPtr = config;
}

//-------------------------------------------------------------------
void USBdevice_Mcu::EPconfig::clrCTR_RX( void )
{
  *regPtr = config & ~(1<<15);//tmp;
}

//-------------------------------------------------------------------
void USBdevice_Mcu::EPconfig::clrCTR_TX( void )
{
  *regPtr = config & ~(1<<7);
}

//-------------------------------------------------------------------
void USBdevice_Mcu::EPconfig::setSTAT_RX( DWORD stat )
{
  *regPtr = (((*regPtr)&(3<<12))^(stat<<12)) | config;
}

//-------------------------------------------------------------------
void USBdevice_Mcu::EPconfig::setSTAT_TX( DWORD stat )
{
  *regPtr = (((*regPtr)&(3<<4))^(stat<<4)) | config;
}

//*******************************************************************
//
// Interrupt-Handler
//
//*******************************************************************
//-------------------------------------------------------------------
extern "C"
{
  void USB_LP_IRQHandler( void )
  {
    USBdevice_Mcu::usbPtr->isr();
  }
}

}  } //namespace

//EOF
