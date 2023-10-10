//*******************************************************************
/*!
\file   System.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   14.03.2011

License: See file "LICENSE"

\brief  Controller specific implementation of class cSystem
*/

//*******************************************************************
#include "Hardware/MCU/System.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//#include "system_ATmega.cpp"

//*******************************************************************
// 
// cSystem
//
//*******************************************************************
//-------------------------------------------------------------------
unsigned char System::cntInterrupt = 0;

//-------------------------------------------------------------------
System::System( bool disableInterrupts )
{
  disableWatchdog();
  if( disableInterrupts )
  {
    disableInterrupt();
  }
  else
  {
    sei();
  }
}

//-------------------------------------------------------------------
void System::start( void )
{
  enableInterrupt();
}

//-------------------------------------------------------------------
void System::disableInterrupt( void )
{
  if( !cntInterrupt )
  {
    cli();
  
  }  
  cntInterrupt++;
}

//-------------------------------------------------------------------
void System::enableInterrupt( void )
{
  if( cntInterrupt > 0 )
  {
    cntInterrupt--;
  }
  if( cntInterrupt == 0 )
  {
    sei();
  }
}

//-------------------------------------------------------------------
void System::enterISR( void )
{
  cntInterrupt++;
}

//-------------------------------------------------------------------
void System::leaveISR( void )
{
  if( cntInterrupt > 0 )
  {
    cntInterrupt--;
  }
}

////-------------------------------------------------------------------
//void  System::run( void )
//{
  //// nothinng to do
//}

//-------------------------------------------------------------------
void __attribute__((optimize("Os"))) System::enableWatchdog( WORD resetTime )
{
  BYTE prescale = 0;

  //switch( mode )
  //{
    //default:
    //case WD_TIMEOUT_16ms:   prescale = 0x00; break;
    //case WD_TIMEOUT_32ms:   prescale = 0x01; break;
    //case WD_TIMEOUT_65ms:   prescale = 0x02; break;
    //case WD_TIMEOUT_130ms:  prescale = 0x03; break;
    //case WD_TIMEOUT_260ms:  prescale = 0x04; break;
    //case WD_TIMEOUT_520ms:  prescale = 0x05; break;
    //case WD_TIMEOUT_1000ms: prescale = 0x06; break;
    //case WD_TIMEOUT_2000ms: prescale = 0x07; break;
  //}

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  #if defined (_MCU_DEV_ATMEGA32)  \
   || defined (_MCU_DEV_AT90CAN128)

     /////WDTCR = (1<<WDE) | prescale;

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  #elif defined (_MCU_DEV_ATMEGA328)  \
     || defined (_MCU_DEV_ATMEGA644)  \
     || defined (_MCU_DEV_ATMEGA644A) \
     || defined (_MCU_DEV_ATMEGA1284P)\
     || defined (_MCU_DEV_AT90USB1287)

   MCUSR &= ~(1<<WDRF);

   WDTCSR |= (1<<WDCE)|(1<<WDE);
   WDTCSR  = (1<<WDE)|prescale;

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  #else
    #error "device type not defined"
  #endif  
}

//-------------------------------------------------------------------
void __attribute__((optimize("Os"))) System::disableWatchdog( void )
{
#if defined (_MCU_DEV_ATMEGA32)

  ////WDTCR |= (1<<WDTOE)|(0<<WDE);
  ////WDTCR &= ~(1<<WDE);

#elif defined (_MCU_DEV_AT90CAN128)

   WDTCR |= (1<<WDCE)|(1<<WDE);
   WDTCR  = 0;

#else

   MCUSR &= ~(1<<WDRF);

   WDTCSR |= (1<<WDCE)|(1<<WDE);
   WDTCSR  = 0;

#endif
}

//-------------------------------------------------------------------
void System::feedWatchdog( void )
{
  asm volatile( "wdr" );
}

//-------------------------------------------------------------------
void System::reset( void )
{
  cli();
  enableWatchdog( 0 ); //WD_TIMEOUT_16ms );

  while( 1 );  
}

//-------------------------------------------------------------------
void __attribute__((optimize("O0"))) System::delayMicroSec( unsigned short delay )
{
  // Calibration:
  delay *= 0.7;
  for(;delay>0;delay--)
  {
//    asm volatile("nop");
  }
}

//-------------------------------------------------------------------
void __attribute__((optimize("O0"))) System::delayMilliSec( unsigned short delay )
{
  for(;delay>0;delay--)
    delayMicroSec(1000);
}


}  } //namespace

// EOF
