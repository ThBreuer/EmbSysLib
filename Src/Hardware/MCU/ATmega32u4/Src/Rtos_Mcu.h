//*******************************************************************
/*!
\file   Rtos_Mcu.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_RTOS_MCU_H
#define _HW_RTOS_MCU_H

//*******************************************************************
namespace EmbSysLib {
namespace Hw {


//*******************************************************************
/*!
\class Rtos_Mcu

\brief Interupt and context switching used by RTOS

\example HwRtos.cpp
*/
class Rtos_Mcu
{
  public:
    //---------------------------------------------------------------
    typedef  unsigned int TaskHandle;

  private:
    //---------------------------------------------------------------
    typedef struct
    {
      uint8_t r[31];
      uint8_t sreg;
      uint8_t r0;
      uint8_t pc_high;
      uint8_t pc_low;
    } stack_frame_t;

  public:
    //---------------------------------------------------------------
    static void init( WORD us, TaskHandle (*scheduleIn)( TaskHandle sp, BYTE flag ))
    {
      schedule = scheduleIn;

      //***************************************
      /*! /warning TIM0 configuration is overwritten
      */

      //***************************************
      // fixed prescaler = 64
      // cycle time = N * (prescale factor)/fosc  with N=1,..,65536 \endcode
      //
      // fosc  prescale/fosc  cycle time max
      //  1     64us           4194ms
      //  2     32us
      //  4     16us
      //  8      8us
      // 16      4us           262ms
      
      // Timer/Counter Register
      TCNT3  = 0;
        
      // Interrupt only, no PWM
      // Waveform Generation Mode = 4 (CTC)
      // TOP = OCR3A, Interrupt on Output Compare Match
        
      // Output Compare Register A
      DWORD tics = (DWORD)us * (OSC_FREQ/1000000L) / 64;
      tics = (tics <= 0xFFFF)?tics:0xFFFF;
      OCR3A = tics;

      // Timer/Counter Control Register A
      TCCR3A =  (0<<COM3A0) // Compare Match Output A Mode: OC3A disconnected
               |(0<<COM3B0) // Compare Match Output B Mode: OC3B disconnected
               |(0<<COM3C0) // Compare Match Output C Mode: OC3C disconnected
               |(0<<WGM30 );// Waveform Generation Mode: CTC

      // Timer/Counter Control Register B
      TCCR3B =  (1<<WGM32) // Waveform Generation Mode = 4 (CTC)
               |0x03;        // Clock Select: prescaler = 64
 
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
        
      sysTicTime = tics*64000/(OSC_FREQ/1000);
    }

    //---------------------------------------------------------------
    /*! Start a RTOS task
 
        Initialize stack frame and start a task by calling the 
        function \b starter
        
        \param stackAddrIn Address of stack memory
        \param stackSizeIn Size of stack memory
        \param arg         Pointer to arguments of the \b starter 
                           function
        \param startFunc   Function pointer
        \return Current stack pointer
        \param startFunc   Function pointer
        \return Current stack pointer
    */
    static unsigned start( BYTE     *stackAddrIn,
                            unsigned  stackSizeIn,
                            void     *arg,
                            void    (*startFunc)(void *) );

    //---------------------------------------------------------------
    /*! Stop a task
    
        Here we can't stop a task, but we can switch the context
        \para id Task ID, reserved for future use
    */
    static void stop( TaskHandle id );

    //---------------------------------------------------------------
    /*! Pause a task
    
        Here we can't pause a task, but we can switch the context
    */
    static void pause( void  );

		//---------------------------------------------------------------
    static unsigned long getSysTime( void )
		{
			return( (unsigned long)((unsigned long long)sysTic*sysTicTime/1000) );
		}

    //---------------------------------------------------------------
    /*! This is called when the task returns
    */
    static void deleteFunc( void )
    {
      while(1);
    }

  public:    
    //---------------------------------------------------------------
    static TaskHandle (*schedule)( TaskHandle sp, BYTE flag );

    //---------------------------------------------------------------
    static DWORD getTics()
    {
      return( sysTic );
    }

    //---------------------------------------------------------------
    static WORD getTimeOfTic( void )
    {
      return( sysTicTime );
    }

  public:
    //---------------------------------------------------------------
    static volatile unsigned long  sysTic;
    static unsigned                sysTicTime;
    
    static bool flag;

}; //class Rtos_Mcu

}  } //namespace

#endif
