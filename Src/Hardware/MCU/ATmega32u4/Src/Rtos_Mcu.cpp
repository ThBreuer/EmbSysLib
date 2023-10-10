//*******************************************************************
/*!
\file   Rtos_Mcu.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   07.02.2023

License: See file "LICENSE"

\warning Don't use TWI in slave mode together with RTOS, interrupt mismatch
\warning RTOS reconfigures Timer!

*/

//*******************************************************************
#include "Rtos_Mcu.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// Rtos_Mcu
//
//*******************************************************************
//-------------------------------------------------------------------
Rtos_Mcu::TaskHandle (*Rtos_Mcu::schedule)( Rtos_Mcu::TaskHandle sp, BYTE flag ) = 0;

//-------------------------------------------------------------------
volatile unsigned long  Rtos_Mcu::sysTic = 0;
unsigned                Rtos_Mcu::sysTicTime = 0;

//-------------------------------------------------------------------
extern "C"
{
//  void     load_context(unsigned ptr);
//  unsigned save_context(void);
}

#define save_context() \
asm volatile ( \
"push r0 \n\t" \
"in r0, __SREG__ \n\t" \
"cli \n\t" \
"push r0 \n\t" \
"push r1 \n\t" \
"clr r1 \n\t" \
"push r2 \n\t" \
"push r3 \n\t" \
"push r4 \n\t" \
"push r5 \n\t" \
"push r6 \n\t" \
"push r7 \n\t" \
"push r8 \n\t" \
"push r9 \n\t" \
"push r10 \n\t" \
"push r11 \n\t" \
"push r12 \n\t" \
"push r13 \n\t" \
"push r14 \n\t" \
"push r15 \n\t" \
"push r16 \n\t" \
"push r17 \n\t" \
"push r18 \n\t" \
"push r19 \n\t" \
"push r20 \n\t" \
"push r21 \n\t" \
"push r22 \n\t" \
"push r23 \n\t" \
"push r24 \n\t" \
"push r25 \n\t" \
"push r26 \n\t" \
"push r27 \n\t" \
"push r28 \n\t" \
"push r29 \n\t" \
"push r30 \n\t" \
"push r31 \n\t" \
);


#define load_context() \
asm volatile (\
"pop r31 \n\t" \
"pop r30 \n\t" \
"pop r29 \n\t" \
"pop r28 \n\t" \
"pop r27 \n\t" \
"pop r26 \n\t" \
"pop r25 \n\t" \
"pop r24 \n\t" \
"pop r23 \n\t" \
"pop r22 \n\t" \
"pop r21 \n\t" \
"pop r20 \n\t" \
"pop r19 \n\t" \
"pop r18 \n\t" \
"pop r17 \n\t" \
"pop r16 \n\t" \
"pop r15 \n\t" \
"pop r14 \n\t" \
"pop r13 \n\t" \
"pop r12 \n\t" \
"pop r11 \n\t" \
"pop r10 \n\t" \
"pop r9 \n\t" \
"pop r8 \n\t" \
"pop r7 \n\t" \
"pop r6 \n\t" \
"pop r5 \n\t" \
"pop r4 \n\t" \
"pop r3 \n\t" \
"pop r2 \n\t" \
"pop r1 \n\t" \
"pop r0 \n\t" \
"out __SREG__, r0 \n\t" \
"pop r0 \n\t" \
);

//-------------------------------------------------------------------
#if defined _USE_RTOS && _USE_RTOS==true
extern "C"
{
  void TIMER3_COMPA_vect() __attribute__ ( ( signal,naked ) );
  ISR(TIMER3_COMPA_vect) 
  {
    save_context()
    Rtos_Mcu::sysTic++;
    SP = Rtos_Mcu::schedule( SP, true );
    load_context()   
    asm volatile("reti");
  } 
}
#endif

//-------------------------------------------------------------------
extern "C"
{
  void RTOS_PENDING_vect() __attribute__ ( ( signal,naked ) );
  ISR(RTOS_PENDING_vect) 
  {
    save_context()
    SP = Rtos_Mcu::schedule( SP, false );
    load_context()   
    asm volatile("reti");
  }
}

//-------------------------------------------------------------------
unsigned Rtos_Mcu::start( BYTE     *stackAddrIn,
                             unsigned  stackSizeIn,
                             void     *arg,
                              void    (*startFunc)(void *) )  
{
  void *stackaddr = (void*)(  ((unsigned)stackAddrIn)); 
  int stack_size = stackSizeIn;

  stack_frame_t *process_frame = (stack_frame_t *)((unsigned)stackaddr + stack_size - sizeof(stack_frame_t)-1);

  process_frame->r0   = 0;
  process_frame->sreg = 0;
  for(int i=0;i<31;i++)
    process_frame->r[i] = 0; // 31-i

  process_frame->r[31-(24)] = (uint16_t)arg & 0xFF;
  process_frame->r[31-(25)] = (uint16_t)arg >> 8;
  process_frame->pc_high    = (uint16_t)startFunc >> 8;
  process_frame->pc_low     = (uint16_t)startFunc & 0xFF;
 
  return( (unsigned) stackaddr + stack_size - sizeof(stack_frame_t)-2 );
}

//-------------------------------------------------------------------
void Rtos_Mcu::stop( TaskHandle id )
{
  RTOS_PENDING_vect();
}

//-------------------------------------------------------------------
void Rtos_Mcu::pause()
{
  RTOS_PENDING_vect();
}

}  } //namespace

//EOF
