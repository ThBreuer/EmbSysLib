//*******************************************************************
/*!
\file   Timer_Mcu.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   14.02.2011

License: See file "LICENSE"
*/

//*******************************************************************
#include "Timer_Mcu.h"
#include "Hardware/ReportID_Hw.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
// 
// Timer_Mcu
//
//*******************************************************************
//-------------------------------------------------------------------
Timer_Mcu::Timer_Mcu( PWM_MODE modeIn )

: Timer( ReportID_Hw::Module::TIMER_MCU )

{
  mode = modeIn;
}

//-------------------------------------------------------------------
BYTE Timer_Mcu::getClockSelect( const WORD cycleTimeIn, 
                                const BYTE prescale[], 
                                const BYTE size,
                                const WORD maxTics)
{
  // prescale setting in TCCR
  BYTE  cs = 0;

  // tics per cycle, raw value
  DWORD tx = (DWORD)cycleTimeIn * (OSC_FREQ/1000000L);

  // calculate tics per cycle with increasing prescale factor
  // finish, when tics <= maxTics
  while(    (tx >> prescale[cs])-1 > maxTics 
         && cs < size-1)
  {
    cs++; 
  }

  tics = (tx >> prescale[cs])-1;

  // limit tics to maxTics in PWM mode or if prescale doesn't fit
  if(mode != INTERRUPT || tics > maxTics)
  {
    tics = maxTics;
  }

  // calculate real cycle time
  cycleTime = (((DWORD)tics+1) << prescale[cs])/(OSC_FREQ/1000000L);

  // return prescale setting (cs==0 lets stop the timer) 
  return(cs+1);
}

//*******************************************************************
// 
// Timer_0
//
//*******************************************************************
//-------------------------------------------------------------------
Timer_Mcu *Timer_0::hwTimerPtr = 0;

//-------------------------------------------------------------------
Timer_0::Timer_0( WORD cycleTimeIn, PWM_MODE modeIn ) 

: Timer_Mcu( modeIn )

{
  // Calculate prescaler setting
  const BYTE ps[] = {0,3,6,8,10}; // prescale = 1,8,64,256,1024
  BYTE cs = getClockSelect( cycleTimeIn, ps, sizeof(ps), 0xFF );

  hwTimerPtr = this;

  // Timer/Counter Register
  TCNT0 = 0;

  if( mode == INTERRUPT ) // Interrupt only, no PWM 
                          // Waveform Generation Mode = 2 (CTC)
                          // TOP = OCRA, Interrupt on Output Compare Match
  {
    // Output Compare Register
    OCR0A = tics;

    // Timer/Counter Control Register
    TCCR0A =  (2<<WGM00 ) // Waveform Generation Mode: CTC
             |(0<<COM0A0) // Compare Match Output Mode:OC0A disconnected
             |(0<<COM0B0);// Compare Match Output Mode:OC0B disconnected
             
    // Timer/Counter Control Register B
    TCCR0B =  (0<<FOC0A ) // Force Output Compare A:   OFF
             |(0<<FOC0B ) // Force Output Compare B:   OFF
             |(0<<WGM02 ) // Waveform Generation Mode: CTC
             |cs;         // Clock Select: see above

    // Interrupt Mask Register (shared register)
    TIMSK0  =  (1<<OCIE0A) // Output A Compare Match Inter. Enable: ON
              |(0<<OCIE0B) // Output B Compare Match Inter. Enable:OFF
              |(0<<TOIE0 );// Overflow Interrupt Enable:           OFF
  }
  else // PWM
       // Waveform Generation Mode = 3 (Fast PWM)
       // TOP = 0xFF, Overflow Interrupt on MAX = 0xFF
  {
    // Timer/Counter Control Register A
    TCCR0A =  (3<<WGM00 ) // Waveform Generation Mode: Fast PWM
             |(0<<COM0A0) // Compare Match Output Mode: OC0A disconnected
             |(0<<COM0B0);// Compare Match Output Mode: OC0B disconnected

    // Timer/Counter Control Register B
    TCCR0B =  (0<<FOC0A ) // Force Output Compare A:   OFF
             |(0<<FOC0B ) // Force Output Compare B:   OFF
             |(0<<WGM02 ) // Waveform Generation Mode: Fast PWM
             |cs;         // Clock Select: see above

    if( mode == PWM_INTERRUPT )
    {
      // Interrupt Mask Register (shared register)
      TIMSK0 =  (0<<OCIE0A) // Output Compare Match Inter. Enable:OFF
               |(0<<OCIE0B) // Output Compare Match Inter. Enable:OFF
               |(1<<TOIE0 );// Overflow Interrupt Enable: ON
    }
    else
    {
      // Interrupt Mask Register (shared register)
      TIMSK0 =  (0<<OCIE0A) // Output Compare Match Inter. Enable:OFF
               |(0<<OCIE0B) // Output Compare Match Inter. Enable:OFF
               |(0<<TOIE0 );// Overflow Interrupt Enable: OFF
    }
  }
}

//-------------------------------------------------------------------
void Timer_0::enablePWM( BYTE ch, Mode pol )
{
  if( mode != INTERRUPT )
  {
    com = ( pol == NORMAL )? 2/*clear*/ : 3/*set*/;

    setPWM( ch, 0 );

    switch( ch )
    {
      case OC0A: // PB7
        if( pol == NORMAL ) { PORTB &= ~(1<<7); }
        else                { PORTB |=  (1<<7); }
        DDRB |=  (1<<7);
        break;

      case OC0B: // PD0
        if( pol == NORMAL ) { PORTD &= ~(1<<0); }
        else                { PORTD |=  (1<<0); }
        DDRD |=  (1<<0);
        break;

      default:
        report.error( ReportID_Hw::Event::WRONG_CHANNEL );
        break;
    }
  }
  else
  {
    report.error( ReportID_Hw::Event::FUNCTION_NOT_SUPPORTED );
  }
}

//-------------------------------------------------------------------
void Timer_0::setPWM( BYTE ch, WORD value )
{
  if( mode == PWM )
  {

    switch( ch )
    {
      case OC0A: // PB7
        OCR0A = value >> 8;
        if( value == 0 ) { TCCR0A &= ~(0x3<<COM0A0); }
        else             { TCCR0A |=  (com<<COM0A0); } // Clear or set OC0A on compare match
        break;

      case OC0B: // PD0
        OCR0B = value >> 8;
        if( value == 0 ) { TCCR0A &= ~(0x3<<COM0B0); }
        else             { TCCR0A |=  (com<<COM0B0); } // Clear or set OC0B on compare match
        break;

      default:
        report.error( ReportID_Hw::Event::WRONG_CHANNEL );
        break;
    }
  }
  else
  {
    report.error( ReportID_Hw::Event::FUNCTION_NOT_SUPPORTED );
  }
}

//-------------------------------------------------------------------
ISR(TIMER0_COMPA_vect)
{
  // allow other interrupts
  //SREG |= (1<<7); // global interrupt enable

  System::enterISR();
  Timer_0::hwTimerPtr->isr();
  System::leaveISR();
}

//-------------------------------------------------------------------
ISR(TIMER0_OVF_vect)
{
  // allow other interrupts
  SREG |= (1<<7); // global interrupt enable

  System::enterISR();
  Timer_0::hwTimerPtr->isr();
  System::leaveISR();
}

//*******************************************************************
// 
// Timer_1
//
//*******************************************************************
//-------------------------------------------------------------------
Timer_Mcu *Timer_1::hwTimerPtr = 0;

//-------------------------------------------------------------------
Timer_1::Timer_1( WORD cycleTimeIn, PWM_MODE modeIn ) 

: Timer_Mcu( modeIn )

{
  // Calculate prescaler setting
  const BYTE ps[] = {0,3,6,8,10}; // prescale = 1,8,64,256,1024
  BYTE cs = getClockSelect( cycleTimeIn, ps, sizeof(ps), 0xFFFF );

  hwTimerPtr = this;

  // Timer/Counter Register
  TCNT1 = 0;

  if( mode == INTERRUPT )  // Interrupt only, no PWM 
                           // Waveform Generation Mode = 4 (CTC)
                           // TOP = OCR1A, Interrupt on Output Compare Match
  {
    // Output Compare Register A
    OCR1A = tics;

    // Timer/Counter Control Register A
    TCCR1A =  (0<<COM1A0) // Compare Match Output A Mode: OC1A disconnected
             |(0<<COM1B0) // Compare Match Output B Mode: OC1B disconnected
             |(0<<COM1C0) // Compare Match Output C Mode: OC1C disconnected
             |(0<<WGM10 );// Waveform Generation Mode: CTC
             
    // Timer/Counter Control Register B  
    TCCR1B =  (1<<WGM12) // Waveform Generation Mode = 4 (CTC)
             |cs;        // Clock Select: see above

    // Timer/Counter Control Register C
    TCCR1C =  (0<<FOC1A ) // Force Output Compare A:   OFF
             |(0<<FOC1B ) // Force Output Compare B:   OFF
             |(0<<FOC1C );// Force Output Compare C:   OFF

    // Interrupt Mask Register
    TIMSK1 |=  (1<<OCIE1A) // Output Compare Match A Interrupt Enable: ON
              |(0<<OCIE1B) // Output Compare Match B Interrupt Enable: OFF
              |(0<<OCIE1C) // Output Compare Match C Interrupt Enable: OFF
              |(0<<ICIE1 ) // Input Capture Interrupt Enable: OFF
              |(0<<TOIE1 );// Overflow Interrupt Enable: OFF
  }
  else // PWM
       // Waveform Generation Mode = 7 (Fast PWM 10 Bit)
       // TOP = 0x3FF, Overflow Interrupt on BOTTOM = 0
  {
    // Timer/Counter Control Register A
    TCCR1A =  (0<<COM1A0) // Compare Match Output A Mode: OC1A disconnected
             |(0<<COM1B0) // Compare Match Output B Mode: OC1B disconnected
             |(0<<COM1C0) // Compare Match Output C Mode: OC1C disconnected
             |(3<<WGM10 ); // Waveform Generation Mode: Fast PWM (10-Bit)        

    // Timer/Counter Control Register B
    TCCR1B =  (1<<WGM12) // Waveform Generation Mode: Fast PWM (10-Bit)
             |cs;        // Clock Select: see above

    // Timer/Counter Control Register C
    TCCR1C =  (0<<FOC1A ) // Force Output Compare A:   OFF
             |(0<<FOC1B ) // Force Output Compare B:   OFF
             |(0<<FOC1C );// Force Output Compare C:   OFF

    if( mode == PWM_INTERRUPT )
    {
      // Interrupt Mask Register
      TIMSK1 |=  (0<<OCIE1A) // Output Compare Match A Interrupt Enable: ON
                |(0<<OCIE1B) // Output Compare Match B Interrupt Enable: OFF
                |(0<<OCIE1C) // Output Compare Match C Interrupt Enable: OFF
                |(0<<ICIE1 ) // Input Capture Interrupt Enable: OFF
                |(1<<TOIE1 );// Overflow Interrupt Enable: ON
    }
    else
    {
      // Interrupt Mask Register
      TIMSK1 |=  (0<<OCIE1A) // Output Compare Match A Interrupt Enable: ON
                |(0<<OCIE1B) // Output Compare Match B Interrupt Enable: OFF
                |(0<<OCIE1C) // Output Compare Match C Interrupt Enable: OFF
                |(0<<ICIE1 ) // Input Capture Interrupt Enable: OFF
                |(0<<TOIE1 );// Overflow Interrupt Enable: OFF
    }
  }
}

//-------------------------------------------------------------------
void Timer_1::enablePWM(BYTE ch, Mode pol )
{
  if(mode != INTERRUPT )
  {
    com = ( pol == NORMAL )? 2/*clear*/ : 3/*set*/;
 
	setPWM( ch, 0 );
    
    switch(ch)
    {
      case OC1A: // PB5
        if( pol == NORMAL ) { PORTB &= ~(1<<5); }
        else                { PORTB |=  (1<<5); }              
        DDRB |= (1<<5);
        break;

      case OC1B: // PB6
        if( pol == NORMAL ) { PORTB &= ~(1<<6); }
        else                { PORTB |=  (1<<6); }
        DDRB |= (1<<6);
        break;
    
      case OC1C: // PB7
        if( pol == NORMAL ) { PORTB &= ~(1<<7); }
        else                { PORTB |=  (1<<7); }
        DDRB |= (1<<7);
        break;

      default:
        report.error( ReportID_Hw::Event::WRONG_CHANNEL );
        break;
    }
  }
  else
  {
    report.error( ReportID_Hw::Event::FUNCTION_NOT_SUPPORTED );
  }
}

//-------------------------------------------------------------------
void Timer_1::setPWM( BYTE ch, WORD value )
{
  if( mode == PWM )
  {
    switch(ch)
    {
      case OC1A:
        OCR1A = value;
        if( value == 0 ) { TCCR1A &= ~(0x3<<COM1A0); }
        else             { TCCR1A |=  (com<<COM1A0); } // Clear or set OC1A on compare match
        break;

      case OC1B:
        OCR1B = value;
        if( value == 0 ) { TCCR1A &= ~(0x3<<COM1B0);  }
        else             { TCCR1A |=  (com<<COM1B0); } // Clear or set OC1B on compare match
      break;
    
      case OC1C:
        OCR1C = value;
        if( value == 0 ) { TCCR1A &= ~(0x3<<COM1C0); }
        else             { TCCR1A |=  (com<<COM1C0); } // Clear or set OC1C on compare match

      default:
        report.error( ReportID_Hw::Event::WRONG_CHANNEL );
        break;
    }
  }    
  else
  {
    report.error( ReportID_Hw::Event::FUNCTION_NOT_SUPPORTED );
  }
}

//-------------------------------------------------------------------
ISR(TIMER1_OVF_vect)
{
  // allow other interrupts
  //SREG |= (1<<7); // global interrupt enable

  System::enterISR();
  Timer_1::hwTimerPtr->isr();
  System::leaveISR();
}

//-------------------------------------------------------------------
ISR(TIMER1_COMPA_vect)
{
  // allow other interrupts
  //SREG |= (1<<7); // global interrupt enable

  System::enterISR();
  Timer_1::hwTimerPtr->isr();
  System::leaveISR();
}

//-------------------------------------------------------------------
ISR(TIMER1_COMPB_vect)
{
  // nothing to do
}

//-------------------------------------------------------------------
ISR(TIMER1_CAPT_vect)
{
  // nothing to do
}

//*******************************************************************
// 
// Timer_3
//
//*******************************************************************
#if defined _USE_RTOS && _USE_RTOS==true
  #pragma warning "Timer 3 not available"
#else
//-------------------------------------------------------------------
Timer_Mcu *Timer_3::hwTimerPtr = 0;

//-------------------------------------------------------------------
Timer_3::Timer_3( WORD cycleTimeIn, PWM_MODE modeIn )

: Timer_Mcu( modeIn )

{
  // Calculate prescaler setting
  const BYTE ps[] = {0,3,6,8,10}; // prescale = 1,8,64,256,1024
  BYTE cs = getClockSelect(cycleTimeIn, ps, sizeof(ps), 0x3FF );

  hwTimerPtr = this;

  // Timer/Counter Register
  TCNT3  = 0;
 
  if( mode == INTERRUPT )  // Interrupt only, no PWM 
                           // Waveform Generation Mode = 4 (CTC)
                           // TOP = OCR3A, Interrupt on Output Compare Match
  {
    // Output Compare Register A
    OCR3A = tics;

    // Timer/Counter Control Register A
    TCCR3A =  (0<<COM3A0) // Compare Match Output A Mode: OC3A disconnected
             |(0<<COM3B0) // Compare Match Output B Mode: OC3B disconnected
             |(0<<COM3C0) // Compare Match Output C Mode: OC3C disconnected
             |(0<<WGM30 );// Waveform Generation Mode: CTC

    // Timer/Counter Control Register B  
    TCCR3B =  (1<<WGM32) // Waveform Generation Mode = 4 (CTC)
             |cs;        // Clock Select: see above

    // Timer/Counter Control Register C
    TCCR3C =  (0<<FOC3A ) // Force Output Compare A:   OFF
             |(0<<FOC3B ) // Force Output Compare B:   OFF
             |(0<<FOC3C );// Force Output Compare C:   OFF

    // Interrupt Mask Register
    TIMSK3 =  (1<<OCIE3A) // Output Compare Match A Interrupt Enable: ON
             |(0<<OCIE3B) // Output Compare Match B Interrupt Enable: OFF
             |(0<<OCIE3C) // Output Compare Match C Interrupt Enable: OFF
             |(0<<ICIE3 ) // Input Capture Interrupt Enable: OFF
             |(0<<TOIE3 );// Overflow Interrupt Enable: OFF
  }
  else // PWM
       // Waveform Generation Mode = 7 (Fast PWM 10 Bit)
       // TOP = 0x3FF, Overflow Interrupt on BOTTOM = 0
  {
    // Timer/Counter Control Register A
    TCCR3A =  (0<<COM3A0) // Compare Match Output A Mode: OC3A disconnected
             |(0<<COM3B0) // Compare Match Output B Mode: OC3B disconnected
             |(0<<COM3C0) // Compare Match Output C Mode: OC3C disconnected
             |(3<<WGM30 );// Waveform Generation Mode: Fast PWM (10-Bit)

    // Timer/Counter Control Register B
    TCCR3B =  (1<<WGM32) // Waveform Generation Mode: Fast PWM (10-Bit)
             |cs;        // Clock Select: see above  

    // Timer/Counter Control Register C
    TCCR3C =  (0<<FOC3A) // Force Output Compare A: OFF
             |(0<<FOC3B) // Force Output Compare B: OFF
             |(0<<FOC3C);// Force Output Compare C: OFF 

    if( mode == PWM_INTERRUPT )
    {
      // Interrupt Mask Register
      TIMSK3 =  (0<<OCIE3A) // Output Compare Match A Interrupt Enable: OFF
               |(0<<OCIE3B) // Output Compare Match B Interrupt Enable: OFF
               |(0<<OCIE3C) // Output Compare Match C Interrupt Enable: OFF
               |(0<<ICIE3 ) // Input Capture Interrupt Enable: OFF
               |(1<<TOIE3 );// Overflow Interrupt Enable: ON
    }
    else
    {
      // Interrupt Mask Register
      TIMSK3 =  (0<<OCIE3A) // Output Compare Match A Interrupt Enable: OFF
               |(0<<OCIE3B) // Output Compare Match B Interrupt Enable: OFF
               |(0<<OCIE3C) // Output Compare Match C Interrupt Enable: OFF
               |(0<<ICIE3 ) // Input Capture Interrupt Enable: OFF
               |(0<<TOIE3 );// Overflow Interrupt Enable: OFF
    }
  }
}

//-------------------------------------------------------------------
void Timer_3::enablePWM(BYTE ch, Mode pol)
{
  if( mode != INTERRUPT )
  {
    com = ( pol == NORMAL )? 2/*clear*/ : 3/*set*/;

    setPWM( ch, 0 );
    
    switch( ch )
    {
      case OC3A: // PC6
        if( pol == NORMAL ) { PORTC &= ~(1<<6); }
        else                { PORTC |=  (1<<6); }              
        DDRC |= (1<<6);
        break;

      default:
        report.error( ReportID_Hw::Event::WRONG_CHANNEL );
        break;
    }
  }
  else
  {
    report.error( ReportID_Hw::Event::FUNCTION_NOT_SUPPORTED );
  }
}

//-------------------------------------------------------------------
void Timer_3::setPWM( BYTE ch, WORD value )
{
  if( mode != INTERRUPT )
  {
    switch( ch )
    {
      case OC3A:
        OCR3A = value >> 6;
        if( value == 0 ) { TCCR3A &= ~((0x3<<COM3A0)); }
        else             { TCCR3A |=  ((com<<COM3A0)); } // Clear or set OC3A on compare match
        break;    

      default:
        report.error( ReportID_Hw::Event::WRONG_CHANNEL );
        break;
    }
  }
  else
  {
    report.error( ReportID_Hw::Event::FUNCTION_NOT_SUPPORTED );
  }
}

//-------------------------------------------------------------------
ISR(TIMER3_OVF_vect)
{
  // allow other interrupts
  //SREG |= (1<<7); // global interrupt enable

  System::enterISR();
  Timer_3::hwTimerPtr->isr();
  System::leaveISR();
}

//-------------------------------------------------------------------
ISR(TIMER3_COMPA_vect) 
{
  // allow other interrupts
  //SREG |= (1<<7); // global interrupt enable

  System::enterISR();
  Timer_3::hwTimerPtr->isr();
  System::leaveISR();
}

//-------------------------------------------------------------------
ISR(TIMER3_COMPB_vect)
{
  // nothing to do
}

//-------------------------------------------------------------------
ISR(TIMER3_CAPT_vect)
{
  // nothing to do
}
#endif

//*******************************************************************
// 
// Timer_4
//
//*******************************************************************
//-------------------------------------------------------------------
Timer_Mcu *Timer_4::hwTimerPtr = 0;

//-------------------------------------------------------------------
Timer_4::Timer_4( WORD cycleTimeIn, PWM_MODE modeIn )

: Timer_Mcu( modeIn )

{
  // Calculate prescaler setting
  const BYTE ps[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14}; // prescale = 1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384

  BYTE cs = getClockSelect(cycleTimeIn, ps, sizeof(ps), 0xFF );

  hwTimerPtr = this;

  // Timer/Counter Register
  TCNT4  = 0;
 
  if( mode == INTERRUPT )  // Interrupt only, no PWM
                          // Waveform Generation Mode = 4 (CTC)
                          // TOP = OCR4A, Interrupt on Output Compare Match
  {
    // Output Compare Register A
    OCR4A = tics;
    
    // Timer/Counter Control Register A
    TCCR4A =  (0<<COM4A0) // Compare Match Output A Mode: OC4A disconnected
             |(0<<COM4B0) // Compare Match Output B Mode: OC4B disconnected
             |(0<<FOC4A ) // Force Output Compare A: OFF
             |(0<<FOC4B ) // Force Output Compare B: OFF
             |(0<<PWM4A ) //
             |(0<<PWM4B );//

    // Timer/Counter Control Register B
    TCCR4B =  (0<<PWM4X ) // 
             |(0<<PSR4  ) // 
             |(0<<DTPS40) // Dead Time Prescaler Bits
             |cs;

    // Timer/Counter Control Register B
    TCCR4C =  (0<<COM4D0) // Compare Match Output D Mode: OC4D disconnected
             |(0<<FOC4A ) // Force Output Compare A: OFF
             |(0<<PWM4D );//

    // Interrupt Mask Register
    TIMSK4 =  (1<<OCIE4A) // Output Compare Match A Interrupt Enable: ON
             |(0<<OCIE4B) // Output Compare Match B Interrupt Enable: OFF
             |(0<<TOIE4 );// Overflow Interrupt Enable: OFF
  }
  else  // PWM
        // Waveform Generation Mode = ??? (Fast PWM 8 Bit)
        // ??? TOP = 0x3FF, Overflow Interrupt on BOTTOM = 0

  {
    // Output Compare Register A and B
    OCR4A = 0;
    OCR4B = 0;

    // Timer/Counter Control Register A
    TCCR4A =  (0<<COM4A0) // Compare Match Output A Mode: OC4A disconnected
             |(0<<COM4B0) // Compare Match Output B Mode: OC4B disconnected
             |(0<<FOC4A ) // Force Output Compare A: OFF
             |(0<<FOC4B ) // Force Output Compare B: OFF
             |(0<<PWM4A ) //
             |(0<<PWM4B );//

    // Timer/Counter Control Register B
    TCCR4B =  (0<<PWM4X ) // 
             |(0<<PSR4  ) // 
             |(0<<DTPS40) // Dead Time Prescaler Bits
             |cs;

    // Timer/Counter Control Register B
    TCCR4C =  (0<<COM4D0) // Compare Match Output D Mode: OC4D disconnected
             |(0<<FOC4A ) // Force Output Compare A: OFF
             |(0<<PWM4D );//

    if( mode == PWM_INTERRUPT )
    { 
      // Interrupt Mask Register
      TIMSK4 =  (0<<OCIE4A) // Output Compare Match A Interrupt Enable: OFF
               |(0<<OCIE4B) // Output Compare Match B Interrupt Enable: OFF
               |(0<<OCIE4D) // Output Compare Match D Interrupt Enable: OFF
               |(1<<TOIE4 );// Overflow Interrupt Enable: ON
    }
    else
    {
      // Interrupt Mask Register
      TIMSK4 =  (0<<OCIE4A) // Output Compare Match A Interrupt Enable: OFF
               |(0<<OCIE4B) // Output Compare Match B Interrupt Enable: OFF
               |(0<<OCIE4D) // Output Compare Match D Interrupt Enable: OFF
               |(0<<TOIE4 );// Overflow Interrupt Enable: OFF
    }
  }
}

//-------------------------------------------------------------------
void Timer_4::enablePWM(BYTE ch, Mode pol)
{
  if( mode != INTERRUPT )
  {
    com = ( pol == NORMAL )? 2/*clear*/ : 3/*set*/;

    setPWM( ch, 0 );
    
    switch( ch )
    {
      case OC4A: // PC6
        if( pol == NORMAL ) { PORTC &= ~(1<<6); }
        else                { PORTC |=  (1<<6); }
        DDRC |= (1<<6);
        break;

      case OC4B: // PB6
        if( pol == NORMAL ) { PORTB &= ~(1<<6); }
        else                { PORTB |=  (1<<6); }
        DDRB |= (1<<6);
        break;

      case OC4D: // PD7
        if( pol == NORMAL ) { PORTD &= ~(1<<7); }
        else                { PORTD |=  (1<<7); }
        DDRD |= (1<<7);
        break;

      default:
        report.error( ReportID_Hw::Event::WRONG_CHANNEL );
        break;
    }
  }
  else
  {
    report.error( ReportID_Hw::Event::FUNCTION_NOT_SUPPORTED );
  }
}

//-------------------------------------------------------------------
void Timer_4::setPWM( BYTE ch, WORD value )
{
  if( mode != INTERRUPT )
  {
    switch( ch )
    {
      case OC4A:
        OCR4A = value >> 8;
        if( value == 0 ) { TCCR4A &= ~(0x3<<COM4A0); }
        else             { TCCR4A |=  (com<<COM4A0); } // Clear OC4A on compare match
        break;    

      case OC4B:
        OCR4B = value >> 8;
        if( value == 0 ) { TCCR4A &= ~(0x3<<COM4B0); }
        else             { TCCR4A |=  (com<<COM4B0); } // Clear OC4B on compare match
        break;    

      case OC4D:
        OCR4D = value >> 8;
        if( value == 0 ) { TCCR4C &= ~(0x3<<COM4D0); }
        else             { TCCR4C |=  (com<<COM4D0); } // Clear OC4B on compare match
        break;    

      default:
        report.error( ReportID_Hw::Event::WRONG_CHANNEL );
        break;
    }
  }
  else
  {
    report.error( ReportID_Hw::Event::FUNCTION_NOT_SUPPORTED );
  }
}

//-------------------------------------------------------------------
ISR(TIMER4_OVF_vect)
{
  // allow other interrupts
  //SREG |= (1<<7); // global interrupt enable

  System::enterISR();
  Timer_4::hwTimerPtr->isr();
  System::leaveISR();
}

//-------------------------------------------------------------------
ISR(TIMER4_COMPA_vect) 
{
  // allow other interrupts
  //SREG |= (1<<7); // global interrupt enable

  System::enterISR();
  Timer_4::hwTimerPtr->isr();
  System::leaveISR();
}

//-------------------------------------------------------------------
ISR(TIMER4_COMPB_vect) 
{
  // nothing to do
}

//-------------------------------------------------------------------
ISR(TIMER4_COMPD_vect) 
{
  // nothing to do
}

}  } //namespace

//EOF
