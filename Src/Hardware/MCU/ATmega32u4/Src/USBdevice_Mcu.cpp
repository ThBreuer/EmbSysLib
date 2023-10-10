//*******************************************************************
/*!
\file   USBdevice_Mcu.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   18.03.2015

License: See file "LICENSE"
*/

//*******************************************************************
#include "USBdevice_Mcu.h"
#include "Hardware/MCU/System.h"
#include "Hardware/ReportID_Hw.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

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

  // Set some registers to their initial (reset) value.
  // Reason: Atmels bootloader activates some interrupts.
  // This function deactivates it, so we have clean start
  // conditions if our program is started from bootloader.
  USBCON	= (1<<FRZCLK);
  UDIEN		= 0;
  UEIENX	= 0;
  UHWCON	= 0;
}

//-------------------------------------------------------------------
inline void USBdevice_Mcu::isrGEN(void)
{
  BYTE udint = UDINT;
  UDINT = 0;
  
  if( udint & (1<<UPRSMI) )
  {
  }

  if( udint & (1<<EORSTI) ) // End Of Reset Interr.
  {
    UENUM = 0; 
    
    if(	UECONX & (1<<EPEN) ) // EP0 already enabled ?
    {
    }
    else
    {
      USBCON &= ~(1<<FRZCLK);
      UECONX |= (1<<EPEN); 

      UECFG0X = (  (0x00 << 6) // Endpoint Type = Control
                 | (0x00));    // Direction = OUT

      UECFG1X = (  (0x03 << 4) // Endpoint Size = 64 bytes
                 | (0x00 << 2)); // Endpoint Bank = One bank

      UECFG1X |= (1 << ALLOC); // Enpoint Allocation = allocate

      if( !(UESTA0X & (1 << CFGOK)) ) // Check Configuration
      {
        // Error
        UECONX &= ~(1<<EPEN);
      }
    }

    UEIENX |= (1<<RXSTPE);
    UEIENX |= (1<<RXOUTE);
    //UEIENX |= (1<<TXINE);
  }

  if( udint & (1<<WAKEUPI) ) // Wake-up CPU Interr.
  {
    UDIEN &= ~(1<<WAKEUPE); // disable Wake-up-Interr.

    // Set PLL
    // PLL clock for USB interface, section 6.11, page 50
    // PLLCSR (PLL Control and Status Register)
    // set PLL prescaler according to XTAL crystal frequency
    
    #if (OSC_FREQ == 16000000)
    #else
      #error "XTAL-Frequency has to be 16 MHz for USB devices!"
    #endif
    
    PLLCSR = (5 << 2);
    (PLLCSR |= (1<<PLLE));
    while (!(PLLCSR & (1<<PLOCK))); 
    
    USBCON &= ~(1<<FRZCLK);
  }

  if( udint & (1<<SUSPI) )
  {
    USBCON &= ~(1<<FRZCLK);
    suspend();
  }
}

//-------------------------------------------------------------------
inline void USBdevice_Mcu::isrCOM(void)
{
  BYTE epTmp = UENUM;

  for(BYTE epNr = 0; epNr < USB_EP_NUM ; epNr++)
  {
    if( UEINT & (1<<epNr) )
    {
      UEINT &= ~(1<<epNr);
      UENUM = epNr;
  
      BYTE epInterruptReason = UEINTX;
           
      if( (epInterruptReason & (1<<RXSTPI)) )
      {
        // Process SETUP
        if(epNr == 0 )
        {
          eventHandler( epNr|0x00, true );
        }
        UEINTX &= ~(1<<RXSTPI);               
      }

      if(    (epInterruptReason & (1<<RXOUTI)) ) 
      {
        // Process OUT
        UEINTX &= ~(1<<RXOUTI);
        eventHandler( epNr|0x00, false );
      }
      
      if(    (epInterruptReason & (1<<NAKOUTI)) ) 
      {
        UEINTX &= ~(1<<NAKOUTI);
      }

      if( (epInterruptReason & (1<<TXINI)) )
      {
        UEINTX &= ~(1<<TXINI);
        eventHandler( epNr|0x80, false );
      }
      
      if( (epInterruptReason & (1<<NAKINI)) )
      {
        if( epNr>0 )
        {
           UEINTX &= ~(1<<NAKINI);
        }
        nak[epNr] = false;
      }
    }
  }
  UENUM = epTmp;
}

//-------------------------------------------------------------------
void USBdevice_Mcu::start(void)
{
  // Basic USB activation necessary to trigger a wakeup interrupt
  // Wakeup ISR will start PLL clock, then USB-END-OF-RESET interrupts are recognized
  UHWCON = ( (1<<UVREGE) );

  USBCON = (  (1<<USBE) 
            | (1<<OTGPADE) 
            | (1<<FRZCLK) );

  asm volatile ("nop"); // nop may be are necessary if mpu is clocked with 16 MHz (deactivated prescaler)
  asm volatile ("nop");
  
  if (0) // 1:lowspeed) 
    UDCON = (1<<LSM);

  USBCON = (  (1<<USBE) 
            | (1<<OTGPADE));

  UDIEN = (  (1<<WAKEUPE) 
           | (1<<EORSTE )
           | (1<<SUSPE  ));

  UDCON &= ~(1<<DETACH); // USB attach
}

//-------------------------------------------------------------------
void USBdevice_Mcu::reset (void) 
{
  // no implementation? 
}

//---------------------------------------------------------------
void USBdevice_Mcu::configure (BYTE flag) 
{
  // no implementation? 
}

//-------------------------------------------------------------------
void USBdevice_Mcu::setAddress( BYTE adr )
{
  if( adr) 
 {
 
  UDADDR = adr & 0x7F;
  UEINTX &= ~(1<<TXINI);
  while (!(UEINTX & (1<<TXINI))); 
  UDADDR |= (1<<ADDEN);
 }
}

//-------------------------------------------------------------------
void USBdevice_Mcu::configEP (BYTE epAddr, WORD epMaxPacketSize, BYTE epType) 
{
  BYTE epTmp = UENUM;

  UENUM = epAddr & 0x0F;
  UECONX = (1<<EPEN);

  BYTE type;  

  switch( epType )
  {
    case USBdeviceDescriptor::Endpoint::BULK:       type = 2; break;
    case USBdeviceDescriptor::Endpoint::INTERRUPT:  type = 3; break;
    default:
      report.error( ReportID_Hw::Event::CONFIG_NOT_SUPPORTED );
  }

  if( epAddr & 0x7F )
  {
    nak[epAddr & 0x7F] = false; ///// in CDC: = true;  ???
  }

  if(epAddr & 0x80) // IN Endpopint
  {
    UECFG0X = (  (type << 6) // Endpoint Type = Interrupt
               | (0x01));    // Direction = IN

    UECFG1X = (  (0x02 << 4) // Endpoint Size = 64 bytes
               | (0x00 << 2)); // Endpoint Bank = One bank

    UECFG1X |= (1 << ALLOC); // Enpoint Allocation = allocate

    UEIENX  |= (1<<TXINE)|(1<<NAKINE);

  }
  else // OUT Endpoint
  {
    UECFG0X = (  (type << 6) // Endpoint Type = Interrupt
               | (0x00));    // Direction = OUT

    UECFG1X = (  (0x02 << 4) // Endpoint Size = 64 bytes
               | (0x00 << 2)); // Endpoint Bank = One bank

    UECFG1X |= (1 << ALLOC); // Enpoint Allocation = allocate

    UEIENX  |= (1<<RXOUTE);
  }

  if( !(UESTA0X & (1 << CFGOK)) ) // Check set up EP0
  {
    // Error
    UECONX &= ~(1<<EPEN); 
  }

  // Enable EP
  UECONX |= (1<<EPEN);
  UECONX |= (1<<STALLRQC);
  
  // Reset EP
  UERST |= (1<<(epAddr&0x0F));
  UERST  = 0x00; 

  UENUM = epTmp;
}

//---------------------------------------------------------------
void USBdevice_Mcu::setStallEP (BYTE epAddr) 
{
  BYTE epTmp = UENUM;
  
  UENUM = epAddr & 0x0F;
  UECONX |= (1<<STALLRQ);
  UENUM = epTmp;
}

//---------------------------------------------------------------
void USBdevice_Mcu::clrStallEP (BYTE epAddr) 
{
  BYTE epTmp = UENUM;

  UENUM = epAddr & 0x0F;
  UECONX |= (1<<STALLRQC);
  UENUM = epTmp;
}

//---------------------------------------------------------------
void USBdevice_Mcu::setNAK_EP (BYTE epAddr)
{
  nak[epAddr]=true;
}

//---------------------------------------------------------------
void USBdevice_Mcu::clrNAK_EP (BYTE epAddr)
{
  nak[epAddr]=false;
  BYTE tmp = UENUM;
  
  UENUM = epAddr & 0x7F;
  UEINTX |= (1<<TXINI);
  UEINTX &= ~(1<<FIFOCON);
  UENUM = tmp;
}

//-------------------------------------------------------------------
WORD USBdevice_Mcu::readEP( BYTE  epAddr, 
                       BYTE *dataPtr, 
                       WORD  len )
{
  WORD cnt=0, n;

  BYTE uenum = UENUM;
  BYTE dummy;

  UENUM = epAddr & 0x07;

  cnt = UEBCLX;  

  for(n=0; n<cnt; n++)
  {
    if( n<len )
    dataPtr[n] = UEDATX; // read from FIFO
    else
      dummy = UEDATX;
  }
  
  //  Acknowledge setup request
  if(epAddr & 0x7F)
  {
    UEINTX &= ~(1<<RXOUTI);
    }
  else
   {
    UEINTX &= ~(1<<RXSTPI);
  }

  UEINTX &= ~(1<<FIFOCON);
  UENUM = uenum;
  return ((cnt<len)?cnt:len);
}

//-------------------------------------------------------------------
WORD USBdevice_Mcu::writeEP( BYTE  epAddr, 
                             BYTE *pData, 
                             WORD  cnt ) 
{
  BYTE uenum = UENUM;

  uint32_t n;
  
  UENUM = epAddr&0x7F;

  if( (epAddr&0x07F) == 0 )
  {
    if( cnt>0 )
      UEIENX |= (1<<TXINE);
    else
      UEIENX &= ~(1<<TXINE);
  }

  for(n=0;n<cnt;n++)
  {
    UEDATX = pData[n];
  }
 
  //  if( (epAddr&0x07F) == 0 )
  {
    UEINTX &= ~(1<<TXINI);
  }

  if( nak[epAddr&0x07F]==false)
  {
    UEINTX &= ~(1<<FIFOCON);
  }
  UENUM = uenum;

  return(cnt);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
// General Interrupt Request
ISR(USB_GEN_vect)
{
  System::enterISR();
  USBdevice_Mcu::usbPtr->isrGEN();
  System::leaveISR();
}

//-------------------------------------------------------------------
// Endpoint/Pipe Interrupt Communication Request
ISR(USB_COM_vect)
{
  System::enterISR();
  USBdevice_Mcu::usbPtr->isrCOM();
  System::leaveISR();
}

}  } //namespace

//EOF
