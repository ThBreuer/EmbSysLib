//*******************************************************************
/*!
\file   USBdevice_Mcu.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   16.10.2022

License: See file "LICENSE"

This implementaion is partially derived from STM32CubeF4
(see: http://www.st.com/web/en/catalog/tools/PF259243)

\todo Bulk IN transfer not working
*/

//*******************************************************************
#include "USBdevice_Mcu.h"

//*******************************************************************
#define USB_PCGCCTL    *(__IO DWORD             *)((DWORD)USB_OTG_FS + USB_OTG_PCGCCTL_BASE)
#define USB_DEVICE      ((USB_OTG_DeviceTypeDef *)((DWORD)USB_OTG_FS + USB_OTG_DEVICE_BASE ))

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// USBdevice_Mcu::EndpointIN
//
//*******************************************************************
//-------------------------------------------------------------------
USBdevice_Mcu::EndpointIN::EndpointIN( void )
{
}

//-------------------------------------------------------------------
void USBdevice_Mcu::EndpointIN::init( BYTE numIn )
{
  num  = numIn;
  inep = ((USB_OTG_INEndpointTypeDef *)((DWORD)USB_OTG_FS + USB_OTG_IN_ENDPOINT_BASE  + (num)*USB_OTG_EP_REG_SIZE));
  fifo = (__IO DWORD                 *)((DWORD)USB_OTG_FS + USB_OTG_FIFO_BASE         + (num)*USB_OTG_FIFO_SIZE   );
}

//-------------------------------------------------------------------
void USBdevice_Mcu::EndpointIN::config( WORD epMPS, BYTE epType )
{
  txBuf.maxpacket = epMPS;

  if( num == 0 )
  {
    epMPS = 0; // MPS has to be 64 for endpoint 0
  }
  USB_DEVICE->DAINTMSK |= USB_OTG_DAINTMSK_IEPM & ((1 << (num)));

  if( !(inep->DIEPCTL & USB_OTG_DIEPCTL_USBAEP) )
  {
    inep->DIEPCTL |= (  (epMPS & USB_OTG_DIEPCTL_MPSIZ )
                      | (epType << 18 )
                      | (num    << 22 )
                      | USB_OTG_DIEPCTL_SD0PID_SEVNFRM
                      | USB_OTG_DIEPCTL_USBAEP );
  }
}

//-------------------------------------------------------------------
void USBdevice_Mcu::EndpointIN::flush( void )
{
  DWORD count = 200000;

  USB_OTG_FS->GRSTCTL = USB_OTG_GRSTCTL_TXFFLSH | (num << 5);

  while(    (USB_OTG_FS->GRSTCTL & USB_OTG_GRSTCTL_TXFFLSH)
         && (count--) );
}

//-------------------------------------------------------------------
void USBdevice_Mcu::EndpointIN::reset()
{
  inep->DIEPINT = 0xFF;
}

//-------------------------------------------------------------------
void USBdevice_Mcu::EndpointIN::start()
{
  if( inep->DIEPCTL & USB_OTG_DIEPCTL_EPENA )
  {
    inep->DIEPCTL = (  USB_OTG_DIEPCTL_EPDIS
                     | USB_OTG_DIEPCTL_SNAK  );
  }
  else
  {
    inep->DIEPCTL = 0;
  }
  inep->DIEPTSIZ = 0;
  inep->DIEPINT  = 0xFF;
}

//-------------------------------------------------------------------
void USBdevice_Mcu::EndpointIN::setStall( void )
{
  if( !(inep->DIEPCTL & USB_OTG_DIEPCTL_EPENA) )
  {
    inep->DIEPCTL &= ~(USB_OTG_DIEPCTL_EPDIS);
  }
  inep->DIEPCTL |= USB_OTG_DIEPCTL_STALL;
}

//---------------------------------------------------------------
void USBdevice_Mcu::EndpointIN::clrStall( void )
{
  inep->DIEPCTL &= ~USB_OTG_DIEPCTL_STALL;
  inep->DIEPCTL |=  USB_OTG_DIEPCTL_SD0PID_SEVNFRM;
}

//-------------------------------------------------------------------
void USBdevice_Mcu::EndpointIN::setNAK( void )
{
  inep->DIEPCTL |= USB_OTG_DIEPCTL_SNAK;
}

//-------------------------------------------------------------------
void USBdevice_Mcu::EndpointIN::clrNAK( void )
{
  inep->DIEPCTL |= USB_OTG_DIEPCTL_CNAK;
}

//-------------------------------------------------------------------
WORD USBdevice_Mcu::EndpointIN::write( BYTE *dataPtr, WORD len )
{
  len = MIN( len, (WORD)128 );

  for(WORD i = 0; i < len; i++ )
  {
    txBuf.data[i] = dataPtr[i];
  }
  txBuf.size = len;
  txBuf.flag = true;

  return( len );
}

//-------------------------------------------------------------------
void USBdevice_Mcu::EndpointIN::writeToFifo( void )
{
  WORD len = MIN( txBuf.size, txBuf.maxpacket );
  WORD cnt = (len + 3) / 4;

  if( (inep->DTXFSTS & USB_OTG_DTXFSTS_INEPTFSAV) >= cnt )
  {
    DWORD *src = (DWORD *)&txBuf.data[0];

    txBuf.size -= len;
    txBuf.flag = txBuf.size?true:false;

    for( WORD i = 0; i < cnt; i++, src ++)
    {
      *fifo = *src;
    }
  }

  if( len <= 0 )
  {
    USB_DEVICE->DIEPEMPMSK &= ~(0x1 << num);
  }
}

//-------------------------------------------------------------------
void USBdevice_Mcu::EndpointIN::transmit( bool flag )
{
  WORD mps  = txBuf.maxpacket;
  WORD len  = MIN( txBuf.size, txBuf.maxpacket );

  if( !txBuf.flag )
  {
    transmitZLP();
    return;
  }

  if( len == 0 ) // Zero Length Packet?
  {
    if( flag )
    {
      transmitZLP();
    }
  }
  else
  {
    BYTE pktCnt =  ((len + mps -1)/ mps);

    inep->DIEPTSIZ =  (  inep->DIEPTSIZ & ~(   USB_OTG_DIEPTSIZ_PKTCNT
                                             | USB_OTG_DIEPTSIZ_XFRSIZ ) )
                      | ((   USB_OTG_DIEPTSIZ_PKTCNT & (pktCnt << 19))
                           |(USB_OTG_DIEPTSIZ_XFRSIZ & len           ));

    USB_DEVICE->DIEPEMPMSK |= 1 << num; // Enable Tx FIFO Empty Interrupt

    inep->DIEPCTL  |=  (   USB_OTG_DIEPCTL_CNAK
                         | USB_OTG_DIEPCTL_EPENA ); // Enable EP
  }
}

//-------------------------------------------------------------------
void USBdevice_Mcu::EndpointIN::transmitZLP( void )
{
    inep->DIEPTSIZ =  ( inep->DIEPTSIZ & ~(   USB_OTG_DIEPTSIZ_PKTCNT
                                            | USB_OTG_DIEPTSIZ_XFRSIZ ) )
                      | (   USB_OTG_DIEPTSIZ_PKTCNT & (1 << 19) );

 //   USB_DEVICE->DIEPEMPMSK |= 1 << num; // Enable Tx FIFO Empty Interrupt

    inep->DIEPCTL  |=  (   USB_OTG_DIEPCTL_CNAK
                         | USB_OTG_DIEPCTL_EPENA ); // Enable EP
}

//-------------------------------------------------------------------
DWORD USBdevice_Mcu::EndpointIN::getInterrupt( void )
{
  DWORD msk = USB_DEVICE->DIEPMSK | (((USB_DEVICE->DIEPEMPMSK >> num) & 0x1) << 7);

  return( inep->DIEPINT & msk );
}

//-------------------------------------------------------------------
void USBdevice_Mcu::EndpointIN::clrInterrupt( DWORD interruptID )
{
  inep->DIEPINT |= interruptID; 
}

//*******************************************************************
//
// USBdevice_Mcu::EndpointOUT
//
//*******************************************************************
//-------------------------------------------------------------------
USBdevice_Mcu::EndpointOUT::EndpointOUT( void )
{
}

//-------------------------------------------------------------------
void USBdevice_Mcu::EndpointOUT::init( BYTE numIn )
{
  num   = numIn;
  outep = ((USB_OTG_OUTEndpointTypeDef *)((DWORD)USB_OTG_FS + USB_OTG_OUT_ENDPOINT_BASE + (num)*USB_OTG_EP_REG_SIZE));
  fifo  = (__IO DWORD                  *)((DWORD)USB_OTG_FS + USB_OTG_FIFO_BASE                                     );
}

//-------------------------------------------------------------------
void USBdevice_Mcu::EndpointOUT::config( WORD epMPS, BYTE epType )
{
  rxBuf.maxpacket = epMPS;

  if( num == 0 )
  {
    epMPS = 0; // MPS has to be 64 for endpoint 0
  }

  USB_DEVICE->DAINTMSK |= USB_OTG_DAINTMSK_OEPM & ((1 << (num)) << 16);

  if( !(outep->DOEPCTL & USB_OTG_DOEPCTL_USBAEP) )
  {
    outep->DOEPCTL |= (  (epMPS & USB_OTG_DOEPCTL_MPSIZ )
                       | (epType << 18 )
                       | (USB_OTG_DIEPCTL_SD0PID_SEVNFRM)
                       | (USB_OTG_DOEPCTL_USBAEP));
  }

  if( num > 0 )
  {
    receive();
  }
}

//-------------------------------------------------------------------
void USBdevice_Mcu::EndpointOUT::flush( void )
{
  DWORD count = 200000;

  USB_OTG_FS->GRSTCTL = USB_OTG_GRSTCTL_RXFFLSH;

  while(    (USB_OTG_FS->GRSTCTL & USB_OTG_GRSTCTL_RXFFLSH)
         && (count--) );
}

//-------------------------------------------------------------------
void USBdevice_Mcu::EndpointOUT::reset()
{
  outep->DOEPINT = 0xFF;
}

//-------------------------------------------------------------------
void USBdevice_Mcu::EndpointOUT::start()
{
  if( outep->DOEPCTL & USB_OTG_DOEPCTL_EPENA )
  {
    outep->DOEPCTL = (   USB_OTG_DOEPCTL_EPDIS
                       | USB_OTG_DOEPCTL_SNAK  );
  }
  else
  {
    outep->DOEPCTL = 0;
  }

  if( num == 0 )  // Setup control EP
  {
    outep->DOEPTSIZ =   (USB_OTG_DOEPTSIZ_PKTCNT & (1 << 19))
                      | MAX_EP0_SIZE
                      | USB_OTG_DOEPTSIZ_STUPCNT;
  }
  else
  {
    outep->DOEPTSIZ = 0;
    outep->DOEPINT  = 0xFF;
  }
}

//-------------------------------------------------------------------
void USBdevice_Mcu::EndpointOUT::setStall( void )
{

  if( !(outep->DOEPCTL & USB_OTG_DOEPCTL_EPENA)  )
  {
    outep->DOEPCTL &= ~(USB_OTG_DOEPCTL_EPDIS);
  }
  outep->DOEPCTL |= USB_OTG_DOEPCTL_STALL;

  if( num == 0 )
  {
    outep->DOEPTSIZ = ( (USB_OTG_DOEPTSIZ_PKTCNT & (1 << 19))
                       | MAX_EP0_SIZE
                       | USB_OTG_DOEPTSIZ_STUPCNT );
  }
}

//---------------------------------------------------------------
void USBdevice_Mcu::EndpointOUT::clrStall( void )
{
  outep->DOEPCTL &= ~USB_OTG_DOEPCTL_STALL;
  outep->DOEPCTL |=  USB_OTG_DOEPCTL_SD0PID_SEVNFRM;
}

//-------------------------------------------------------------------
void USBdevice_Mcu::EndpointOUT::setNAK( void )
{
  outep->DOEPCTL |= USB_OTG_DOEPCTL_SNAK;
}

//-------------------------------------------------------------------
void USBdevice_Mcu::EndpointOUT::clrNAK( void )
{
  outep->DOEPCTL |= USB_OTG_DOEPCTL_CNAK;
}

//-------------------------------------------------------------------
WORD USBdevice_Mcu::EndpointOUT::read( BYTE *dataPtr, WORD len )
{
  len = MIN( len, (WORD)rxBuf.size );

  for( WORD i = 0; i < len; i++ )
  {
    dataPtr[i] = rxBuf.data[i];
  }
  rxBuf.size = 0;
  rxBuf.flag = false;

  return( len );
}

//-------------------------------------------------------------------
void USBdevice_Mcu::EndpointOUT::readFromFifo( WORD len )
{
  WORD cnt = (len + 3) / 4;

  DWORD *dest = (DWORD *)&rxBuf.data[0];

  rxBuf.size = len;
  rxBuf.flag = true;

  for( WORD  i = 0; i < cnt; i++, dest++ )
  {
    *dest = *fifo;
  }
}

//-------------------------------------------------------------------
void USBdevice_Mcu::EndpointOUT::receive( void )
{
  WORD mps = rxBuf.maxpacket;
  WORD len = MIN( rxBuf.size, rxBuf.maxpacket );

  if( len == 0 )
  {
    receiveZLP();
  }
  else
  {
   BYTE pktcnt = (rxBuf.size + mps -1)/ mps;

   outep->DOEPTSIZ  &= ~(   USB_OTG_DOEPTSIZ_PKTCNT
                          | USB_OTG_DOEPTSIZ_XFRSIZ );

    outep->DOEPTSIZ |=  (  (USB_OTG_DOEPTSIZ_PKTCNT & (pktcnt << 19))
                          |(USB_OTG_DOEPTSIZ_XFRSIZ & (mps * pktcnt)) );

    outep->DOEPCTL  |=  (   USB_OTG_DOEPCTL_CNAK
                          | USB_OTG_DOEPCTL_EPENA );  // Enable EP
  }
}

//-------------------------------------------------------------------
void USBdevice_Mcu::EndpointOUT::receiveZLP()
{
  outep->DOEPTSIZ &= ~(   USB_OTG_DOEPTSIZ_PKTCNT
                        | USB_OTG_DOEPTSIZ_XFRSIZ );

  outep->DOEPTSIZ |=  (  (USB_OTG_DOEPTSIZ_PKTCNT & (1 << 19))
                      |  (USB_OTG_DOEPTSIZ_XFRSIZ & rxBuf.maxpacket) );

  outep->DOEPCTL  |=  (   USB_OTG_DOEPCTL_CNAK
                        | USB_OTG_DOEPCTL_EPENA );  // Enable EP
}

//-------------------------------------------------------------------
DWORD USBdevice_Mcu::EndpointOUT::getInterrupt( void )
{
  return( outep->DOEPINT & USB_DEVICE->DOEPMSK );
}

//-------------------------------------------------------------------
void USBdevice_Mcu::EndpointOUT::clrInterrupt( DWORD interruptID )
{
  outep->DOEPINT |= interruptID;
}

//*******************************************************************
//
// USBdevice_Mcu
//
//*******************************************************************
//-------------------------------------------------------------------
USBdevice_Mcu *USBdevice_Mcu::usbPtr = 0;

//-------------------------------------------------------------------
USBdevice_Mcu::USBdevice_Mcu( USBdeviceDescriptor &desc )

: USBdevice( ReportID_Hw::Module::USB_DEVICE_MCU, desc )

{
  usbPtr = this;

  for( WORD i = 0; i < NUM_OF_ENDPOINTS; i++ )
  {
    epIN [i].init( i );
    epOUT[i].init( i );
  }
}

//-------------------------------------------------------------------
void USBdevice_Mcu::start(void)
{
  PinConfig::set( PinConfig::USB_OTG_FS_DM, PinConfig::NO_PUPD
                                       |PinConfig::HIGH_SPEED
                                       |PinConfig::PUSH_PULL );

  PinConfig::set( PinConfig::USB_OTG_FS_DP, PinConfig::NO_PUPD
                                       |PinConfig::HIGH_SPEED
                                       |PinConfig::PUSH_PULL);

  RCC->AHB2ENR |= RCC_AHB2ENR_OTGFSEN;              // Enable USB-FS clock

  NVIC_EnableIRQ( OTG_FS_IRQn );

  USB_OTG_FS->GAHBCFG &= ~USB_OTG_GAHBCFG_GINT;     // disable USB interrupts
  USB_OTG_FS->GUSBCFG |=  USB_OTG_GUSBCFG_PHYSEL;   // Select FS Embedded PHY
  CoreReset();                                      // Reset after a PHY select
                                                    // and set Host mode
//  USB_OTG_FS->GCCFG    = USB_OTG_GCCFG_PWRDWN       // Deactivate the power down
//                         | USB_OTG_GCCFG_VBDEN;     // VBUS detection enable

//USB_OTG_FS->GOTGCTL |=   USB_OTG_GOTGCTL_BVALOEN
//                       | USB_OTG_GOTGCTL_BVALOVAL;

  USB_OTG_FS->GCCFG    = USB_OTG_GCCFG_PWRDWN;       // Deactivate the power down

  USB_OTG_FS->GOTGCTL |=   USB_OTG_GOTGCTL_BVALOEN
                         | USB_OTG_GOTGCTL_BVALOVAL;

  USB_OTG_FS->GUSBCFG &= ~( USB_OTG_GUSBCFG_FHMOD   // Force Device Mode
                           |USB_OTG_GUSBCFG_FDMOD );
  USB_OTG_FS->GUSBCFG |=  ( USB_OTG_GUSBCFG_FDMOD );

  for (uint32_t i = 0; i < 15 ; i++)
  {
    USB_OTG_FS->DIEPTXF[i] = 0;
  }

  //  USB_OTG_FS->GCCFG |= USB_OTG_GCCFG_VBUSBSEN;    // Activate VBUS Sensing B
  //  USB_OTG_FS->GCCFG |= USB_OTG_GCCFG_NOVBUSSENS;  // No Vbus sensing
  USB_PCGCCTL        = 0;                         // Restart the Phy Clock

  // Device configuration
  USB_DEVICE->DCFG  |= ( (3 <<  0)   // Device speed: Full speed
                        |(0 << 11)); // Periodic frame interval: 80%

  // Flush the FIFO
  epIN [0].flush();
  epOUT[0].flush();

  // Clear all pending device interrupts
  USB_DEVICE->DIEPMSK  = 0;
  USB_DEVICE->DOEPMSK  = 0;
  USB_DEVICE->DAINT    = 0xFFFFFFFF;
  USB_DEVICE->DAINTMSK = 0;

  for (uint32_t i = 0; i < NUM_OF_ENDPOINTS; i++)
  {
    epIN[i].start();
    epOUT[i].start();
  }

  USB_DEVICE->DIEPMSK &= ~(USB_OTG_DIEPMSK_TXFURM);  //
  USB_OTG_FS->GINTMSK  = 0;                          // Disable all interrupts
  USB_OTG_FS->GINTSTS  = 0xBFFFFFFF;                 // Clear any pending interrupts

  USB_OTG_FS->GINTMSK |= USB_OTG_GINTMSK_RXFLVLM;    // Enable the common interrupts

  // Enable interrupts matching to the Device mode ONLY
  USB_OTG_FS->GINTMSK |= (  USB_OTG_GINTMSK_USBSUSPM
                          | USB_OTG_GINTMSK_USBRST
                          | USB_OTG_GINTMSK_ENUMDNEM
                          | USB_OTG_GINTMSK_IEPINT
                          | USB_OTG_GINTMSK_OEPINT
                          | USB_OTG_GINTMSK_WUIM );
  USB_OTG_FS->GINTMSK |= (  USB_OTG_GINTMSK_SRQIM
                          | USB_OTG_GINTMSK_OTGINT);

  USB_DEVICE->DCTL |= USB_OTG_DCTL_SDIS;             // disconnect

  // STM32F7xx ReferenceManual:
  // Device RxFIFO =   (4 * number of control endpoints + 6)    10
  //                 + ((largest USB packet used / 4)           16
  //                 + 1 for status information)                 1
  //                 + (2 * number of OUT endpoints)             6
  //                 + 1 for Global NAK                          1
  // size in DWORDs (32bit) !!!
  WORD offset = 0;
  USB_OTG_FS->GRXFSIZ            = (  128               ); offset  = 128; // set RX fifo size
  USB_OTG_FS->DIEPTXF0_HNPTXFSIZ = (( 32L << 16) | offset); offset += 32; // set TX 0 fifo size
  USB_OTG_FS->DIEPTXF[0]         = (( 32L << 16) | offset); offset += 32; // set TX 1 fifo size
  USB_OTG_FS->DIEPTXF[1]         = (( 32L << 16) | offset); offset += 32; // set TX 2 fifo size
  USB_OTG_FS->DIEPTXF[2]         = (( 32L << 16) | offset);               // set TX 3 fifo size

  USB_DEVICE->DCTL    &= ~USB_OTG_DCTL_SDIS;         // connect
  USB_OTG_FS->GAHBCFG |=  USB_OTG_GAHBCFG_GINT;      // enable USB interrupts
}

//-------------------------------------------------------------------
inline void USBdevice_Mcu::isr(void)
{
  // Current mode of operation = Host mode ?
  if( USB_OTG_FS->GINTSTS & USB_OTG_GINTSTS_CMOD )
  {
    return;
  }

  //-----------------------------------------------------------------
  if( isInterruptPending( USB_OTG_GINTSTS_MMIS ) )
  {
    // incorrect mode
    clrInterrupt( USB_OTG_GINTSTS_MMIS);
  }

  //-----------------------------------------------------------------
  if( isInterruptPending( USB_OTG_GINTSTS_OEPINT ) )
  {
    DWORD interruptBits = getOutInterrupt();
    DWORD interruptType = 0;
    BYTE  epNum         = 0;

    while ( interruptBits )
    {
      if( interruptBits & 0x1 )
      {
        interruptType = epOUT[epNum].getInterrupt();

        if( interruptType & USB_OTG_DOEPINT_XFRC )
        {
          // OUT-package received, non-control
          epOUT[epNum].receive();
          eventHandler( epNum, false );
          epOUT[epNum].clrInterrupt( USB_OTG_DOEPINT_XFRC );
        }

        if( interruptType & USB_OTG_DOEPINT_STUP )
        {
          // Setup-package received
          epOUT[epNum].receive();
          eventHandler( epNum, true );
          epIN[epNum].transmit(true);
          // epIN[0].writeToFifo();
          epOUT[epNum].clrInterrupt( USB_OTG_DOEPINT_STUP );
        }

        if( interruptType & USB_OTG_DOEPINT_OTEPDIS )
        {
          epOUT[epNum].clrInterrupt( USB_OTG_DOEPINT_OTEPDIS);
        }
      }

      // handle next endpoint
      epNum++;
      interruptBits >>= 1;
    }
  }

  //-----------------------------------------------------------------
  if( isInterruptPending( USB_OTG_GINTSTS_IEPINT ) )
  {
    DWORD interruptBits = getInInterrupt();
    DWORD interruptType = 0;
    BYTE  epNum         = 0;

    while( interruptBits )
    {
      if( interruptBits & 0x1 )
      {
        interruptType = epIN[epNum].getInterrupt( );

        if( interruptType & USB_OTG_DIEPINT_XFRC )
        {
          // USB_DEVICE->DIEPEMPMSK = ~(0x1 << epNum);
          // epIN[epNum].clrInterrupt( USB_OTG_DIEPINT_XFRC );
          epOUT[epNum].receiveZLP();
          eventHandler( epNum | 0x80, true );

          // todo interrupt handling not correct for interrupt or ctrl transfers
          // workaround:
          if( epNum > 0 )
          {
              epIN[epNum].transmit( false );
              epIN[epNum].writeToFifo();
          }
          epIN[epNum].clrInterrupt( USB_OTG_DIEPINT_XFRC );
        }

        if( interruptType & USB_OTG_DIEPINT_TOC )
        {
          epIN[epNum].clrInterrupt( USB_OTG_DIEPINT_TOC );
        }

        if( interruptType & USB_OTG_DIEPINT_ITTXFE )
        {
          epIN[epNum].writeToFifo();
          if( epNum > 0 )
          {
            epIN[epNum].transmit( false );
          }
          epIN[epNum].clrInterrupt( USB_OTG_DIEPINT_ITTXFE );
        }

        if( interruptType & USB_OTG_DIEPINT_INEPNE )
        {
          epIN[epNum].clrInterrupt( USB_OTG_DIEPINT_INEPNE );
        }

        if( interruptType & USB_OTG_DIEPINT_EPDISD )
        {
          epIN[epNum].clrInterrupt( USB_OTG_DIEPINT_EPDISD );
        }

        if( interruptType & USB_OTG_DIEPINT_TXFE )
        {
          epIN[epNum].writeToFifo();
          epIN[epNum].clrInterrupt( USB_OTG_DIEPINT_TXFE );
        }
      }

      // handle next endpoint
      epNum++;
      interruptBits >>= 1;
    }
  }

  //-----------------------------------------------------------------
  if( isInterruptPending( USB_OTG_GINTSTS_WKUINT ) )
  {
    USB_DEVICE->DCTL &= ~USB_OTG_DCTL_RWUSIG;
    clrInterrupt( USB_OTG_GINTSTS_WKUINT );
  }

  //-----------------------------------------------------------------
  if( isInterruptPending( USB_OTG_GINTSTS_USBSUSP ) )
  {
    clrInterrupt( USB_OTG_GINTSTS_USBSUSP );
  }

  //-----------------------------------------------------------------
  if( isInterruptPending( USB_OTG_GINTSTS_USBRST ) )
  {
    reset();
    clrInterrupt( USB_OTG_GINTSTS_USBRST );
  }

  //-----------------------------------------------------------------
  if( isInterruptPending( USB_OTG_GINTSTS_ENUMDNE ) )
  {
    USB_DEVICE->DCTL |= USB_OTG_DCTL_CGINAK;

    USB_OTG_FS->GUSBCFG &= ~(  USB_OTG_GUSBCFG_TRDT );
    USB_OTG_FS->GUSBCFG |=  (  USB_OTG_GUSBCFG_TRDT_0
                             | USB_OTG_GUSBCFG_TRDT_2 );

    epOUT[0].config( MAX_EP0_SIZE );
    epIN [0].config( MAX_EP0_SIZE );

    clrInterrupt( USB_OTG_GINTSTS_ENUMDNE );
  }

  //-----------------------------------------------------------------
  if( isInterruptPending( USB_OTG_GINTSTS_RXFLVL ) )
  {
    USB_OTG_FS->GINTMSK &= ~(USB_OTG_GINTMSK_RXFLVLM);

    DWORD temp = USB_OTG_FS->GRXSTSP;

    BYTE epnum = temp & USB_OTG_GRXSTSP_EPNUM;

    if( ((temp & USB_OTG_GRXSTSP_PKTSTS) >> 17) ==  2 ) // Packet status = OUT data packet received
    {
      if( temp & USB_OTG_GRXSTSP_BCNT )
      {
        epOUT[epnum].readFromFifo( (temp & USB_OTG_GRXSTSP_BCNT) >> 4 );
      }
    }
    else if( ((temp & USB_OTG_GRXSTSP_PKTSTS) >> 17) ==  6 ) // Packet status = Setup data packet received
    {
      epOUT[epnum].readFromFifo( 8 );
    }
    USB_OTG_FS->GINTMSK |= (USB_OTG_GINTMSK_RXFLVLM);
    clrInterrupt( USB_OTG_GINTSTS_RXFLVL );
  }

  //-----------------------------------------------------------------
  if( isInterruptPending( USB_OTG_GINTSTS_SOF ) )
  {
    clrInterrupt( USB_OTG_GINTSTS_SOF );
  }

  //-----------------------------------------------------------------
  if( isInterruptPending( USB_OTG_GINTSTS_SRQINT ) )
  {
    clrInterrupt( USB_OTG_GINTSTS_SRQINT );
  }

  //-----------------------------------------------------------------
  if( isInterruptPending( USB_OTG_GINTSTS_OTGINT ) )
  {
    DWORD temp = USB_OTG_FS->GOTGINT;
    USB_OTG_FS->GOTGINT |= temp;

    clrInterrupt( USB_OTG_GINTSTS_OTGINT );
  }
}

//-------------------------------------------------------------------
void USBdevice_Mcu::reset( void )
{
  USB_DEVICE->DCTL &= ~USB_OTG_DCTL_RWUSIG;

  epIN[0].flush();

  for( WORD i = 0; i < NUM_OF_ENDPOINTS ; i++ )
  {
    epIN [i].reset();
    epOUT[i].reset();
  }

  USB_DEVICE->DAINT     = 0xFFFFFFFF;
  USB_DEVICE->DAINTMSK |= 0x10001;

  USB_DEVICE->DOEPMSK |= (  USB_OTG_DOEPMSK_STUPM
                          | USB_OTG_DOEPMSK_XFRCM
                          | USB_OTG_DOEPMSK_EPDM
                          | USB_OTG_DOEPMSK_OTEPDM );

  USB_DEVICE->DIEPMSK |= (  USB_OTG_DIEPMSK_TOM
                          | USB_OTG_DIEPMSK_XFRCM
                          | USB_OTG_DIEPMSK_ITTXFEMSK
                          | USB_OTG_DIEPMSK_EPDM );

  USB_DEVICE->DCFG    &= ~USB_OTG_DCFG_DAD; // Set Default Address to 0
}

//-------------------------------------------------------------------
void USBdevice_Mcu::configure( BYTE flag )
{
}

//-------------------------------------------------------------------
void USBdevice_Mcu::setAddress( BYTE adr )
{
  USB_DEVICE->DCFG &= ~USB_OTG_DCFG_DAD;
  USB_DEVICE->DCFG |=  USB_OTG_DCFG_DAD & (adr << 4);

  epIN[0].transmitZLP();
}

//-------------------------------------------------------------------
void USBdevice_Mcu::configEP( BYTE epAddr,
                         WORD epMaxPacketSize,
                         BYTE epType )
{
  BYTE epNum = epAddr&0x7F;

  if( epNum < NUM_OF_ENDPOINTS )
  {
    if( epAddr & 0x80 )
    {
      epIN[epNum].config( epMaxPacketSize, epType );
    }
    else
    {
      epOUT[epNum].config( epMaxPacketSize, epType );
    }

    clrStallEP( epAddr );
    setNAK_EP( epAddr );
    epIN[epNum].transmitZLP();
  }
}

//---------------------------------------------------------------
void USBdevice_Mcu::setStallEP (BYTE epAddr)
{
  BYTE epNum = epAddr&0x7F;

  if( epNum < NUM_OF_ENDPOINTS )
  {
    if( epAddr & 0x80 )
    {
      epIN[epNum].setStall();
    }
    else
    {
      epOUT[epNum].setStall();
    }
  }
}

//---------------------------------------------------------------
void USBdevice_Mcu::clrStallEP (BYTE epAddr)
{
  BYTE epNum = epAddr&0x7F;

  if( epNum < NUM_OF_ENDPOINTS )
  {
    if( epAddr & 0x80 )
      epIN[epNum].clrStall();
    else
      epOUT[epNum].clrStall();
  }
}

//---------------------------------------------------------------
void USBdevice_Mcu::setNAK_EP (BYTE epAddr)
{
BYTE  num = epAddr & 0x07;
if( epAddr & 0x80 )
  epIN[num].setNAK();
else
  epOUT[num].setNAK();
}

//---------------------------------------------------------------
void USBdevice_Mcu::clrNAK_EP (BYTE epAddr)
{
BYTE  num = epAddr & 0x07;
if( epAddr & 0x80 )
  epIN[num].clrNAK();
else
  epOUT[num].clrNAK();
}

//-------------------------------------------------------------------
WORD USBdevice_Mcu::readEP( BYTE  epAddr,
                       BYTE *dataPtr,
                       WORD  len )
{
  BYTE epNum = epAddr&0x7F;

  if( epNum < NUM_OF_ENDPOINTS )
  {
    return( epOUT[epNum].read( dataPtr, len ) );
  }
  return( 0 );
}

//-------------------------------------------------------------------
WORD USBdevice_Mcu::writeEP( BYTE  epAddr,
                        BYTE *dataPtr,
                        WORD  len )
{
  BYTE epNum = epAddr&0x7F;

  if( epNum < NUM_OF_ENDPOINTS )
  {
    return( epIN[epNum].write( dataPtr, len ) );
  }
  return( 0 );
}

//-------------------------------------------------------------------
BYTE USBdevice_Mcu::CoreReset( void )
{
  DWORD count = 200000;

  // Wait for AHB master IDLE state
  while( !(USB_OTG_FS->GRSTCTL & USB_OTG_GRSTCTL_AHBIDL) )
  {
    if( !count-- )
    {
      return false;
    }
  }

  USB_OTG_FS->GRSTCTL |= USB_OTG_GRSTCTL_CSRST;  // Core Soft Reset

  count = 200000;
  while( USB_OTG_FS->GRSTCTL & USB_OTG_GRSTCTL_CSRST )
  {
    if( !count-- )
    {
      return false;
    }
  }

  return true;
}

//-------------------------------------------------------------------
DWORD USBdevice_Mcu::getInInterrupt( void )
{
  return(    USB_DEVICE->DAINT
           & USB_DEVICE->DAINTMSK
           & 0xFFFF               );
}

//-------------------------------------------------------------------
DWORD USBdevice_Mcu::getOutInterrupt( void )
{
  return( (   USB_DEVICE->DAINT
            & USB_DEVICE->DAINTMSK
            & 0xFFFF0000           ) >> 16);
}

//-------------------------------------------------------------------
BYTE USBdevice_Mcu::isInterruptPending( DWORD interuptID )
{
  return( (   USB_OTG_FS->GINTSTS
            & USB_OTG_FS->GINTMSK
            & interuptID          ) == interuptID );
}

//-------------------------------------------------------------------
void USBdevice_Mcu::clrInterrupt( DWORD interuptID )
{
  USB_OTG_FS->GINTSTS |= interuptID;
}

//-------------------------------------------------------------------
//*******************************************************************
//
// Interrupt-Handler
//
//*******************************************************************
//-------------------------------------------------------------------
extern "C"
{
  void OTG_FS_IRQHandler(void)
  {
    USBdevice_Mcu::usbPtr->isr();
  }
}

}  } //namespace

//EOF
