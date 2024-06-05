//*******************************************************************
/*!
\file   Uart_Mcu.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   16.10.2022

License: See file "LICENSE"
*/

//*******************************************************************
#include "Uart_Mcu.h"
#include "PinConfig.h"
#include "Std/Report.h"
#include "Hardware/ReportID_Hw.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// Uart_Mcu
//
//*******************************************************************
//-------------------------------------------------------------------
#if ( system_PCLK_UART != 50000000 )
  #error "system_PCLK_UART not defined or wrong value"
#endif

//-------------------------------------------------------------------
Uart_Mcu *Uart_Mcu::uartPtr_1 = 0;
Uart_Mcu *Uart_Mcu::uartPtr_2 = 0;
Uart_Mcu *Uart_Mcu::uartPtr_3 = 0;
Uart_Mcu *Uart_Mcu::uartPtr_4 = 0;
Uart_Mcu *Uart_Mcu::uartPtr_5 = 0;
Uart_Mcu *Uart_Mcu::uartPtr_6 = 0;
Uart_Mcu *Uart_Mcu::uartPtr_7 = 0;

//-------------------------------------------------------------------
Uart_Mcu::Uart_Mcu( USART_Id id,
                    DWORD    baud,
                    BYTE     mode,
                    WORD     sizeRxBuf,
                    WORD     sizeTxBuf )

: Uart( ReportID_Hw::Module::UART_MCU, sizeRxBuf, sizeTxBuf )
{
  switch( id )
  {
    case USART_1:
      ptr           = (USART_TypeDef*)USART1_BASE;
      uartPtr_1     = this;
      RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
      PinConfig::set( PinConfig::USART1_RX, PinConfig::ALTERNATE_FUNC );
      PinConfig::set( PinConfig::USART1_TX, PinConfig::ALTERNATE_FUNC );
      if( mode & RTS )
      {
        PinConfig::set( PinConfig::USART1_RTS, PinConfig::ALTERNATE_FUNC );
      }
      if( mode & CTS )
      {
        PinConfig::set( PinConfig::USART1_CTS, PinConfig::ALTERNATE_FUNC );
      }
      NVIC_EnableIRQ( USART1_IRQn );
      break;

  case USART_2:
      ptr           = (USART_TypeDef*)USART2_BASE;
      uartPtr_2     = this;
      RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
      PinConfig::set( PinConfig::USART2_RX, PinConfig::ALTERNATE_FUNC );
      PinConfig::set( PinConfig::USART2_TX, PinConfig::ALTERNATE_FUNC );
      if( mode & RTS )
      {
        PinConfig::set( PinConfig::USART2_RTS, PinConfig::ALTERNATE_FUNC );
      }
      if( mode & CTS )
      {
        PinConfig::set( PinConfig::USART2_CTS, PinConfig::ALTERNATE_FUNC );
      }
      NVIC_EnableIRQ( USART2_IRQn );
      break;

    case USART_3:
      ptr           = (USART_TypeDef*)USART3_BASE;
      uartPtr_3     = this;
      RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
      PinConfig::set( PinConfig::USART3_RX, PinConfig::ALTERNATE_FUNC );
      PinConfig::set( PinConfig::USART3_TX, PinConfig::ALTERNATE_FUNC );
      if( mode & RTS )
      {
        PinConfig::set( PinConfig::USART3_RTS, PinConfig::ALTERNATE_FUNC );
      }
      if( mode & CTS )
      {
        PinConfig::set( PinConfig::USART3_CTS, PinConfig::ALTERNATE_FUNC );
      }
      NVIC_EnableIRQ( USART3_IRQn );
      break;

    case UART_4:
      ptr           = (USART_TypeDef*)UART4_BASE;
      uartPtr_4     = this;
      RCC->APB1ENR |= RCC_APB1ENR_UART4EN;
      PinConfig::set( PinConfig::UART4_RX, PinConfig::ALTERNATE_FUNC );
      PinConfig::set( PinConfig::UART4_TX, PinConfig::ALTERNATE_FUNC );
      // no RTS/CTS
      NVIC_EnableIRQ( UART4_IRQn );
      break;

    case UART_5:
      ptr           = (USART_TypeDef*)UART5_BASE;
      uartPtr_5     = this;
      RCC->APB1ENR |= RCC_APB1ENR_UART5EN;
      PinConfig::set( PinConfig::UART5_RX, PinConfig::ALTERNATE_FUNC );
      PinConfig::set( PinConfig::UART5_TX, PinConfig::ALTERNATE_FUNC );
      // no RTS/CTS
      NVIC_EnableIRQ( UART5_IRQn );
      break;

    case USART_6:
      ptr           = (USART_TypeDef*)USART6_BASE;
      uartPtr_6     = this;
      RCC->APB2ENR |= RCC_APB2ENR_USART6EN;
      PinConfig::set( PinConfig::USART6_RX, PinConfig::ALTERNATE_FUNC );
      PinConfig::set( PinConfig::USART6_TX, PinConfig::ALTERNATE_FUNC );
      if( mode & RTS )
      {
        PinConfig::set( PinConfig::USART6_RTS, PinConfig::ALTERNATE_FUNC );
      }
      if( mode & CTS )
      {
        PinConfig::set( PinConfig::USART6_CTS, PinConfig::ALTERNATE_FUNC );
      }
      NVIC_EnableIRQ( USART6_IRQn );
      break;

    case UART_7:
          ptr           = (USART_TypeDef*)UART7_BASE;
          uartPtr_7     = this;
          RCC->APB1ENR |= RCC_APB1ENR_UART7EN;
          PinConfig::set( PinConfig::UART7_RX, PinConfig::ALTERNATE_FUNC );
          PinConfig::set( PinConfig::UART7_TX, PinConfig::ALTERNATE_FUNC );
          // no RTS/CTS
          NVIC_EnableIRQ( UART7_IRQn );
          break;

    default:
      report.error( ReportID_Hw::Event::WRONG_ID );
      break;

  }

  ptr->CR2 |=  ((MTYPE)0<<12); // 0: 1 stop bit

  ptr->CR1 |=  (        ((mode&(EVEN|ODD))?USART_CR1_M  :0)  // 8+paritry = 9 Bit
                     |  ((mode&(EVEN|ODD))?USART_CR1_PCE:0)  //  parity
                     |  ((mode&      ODD )?USART_CR1_PS :0)  // 0: even
                     |  USART_CR1_RE   // RX mode
                     |  USART_CR1_TE );// TX mode

  ptr->CR3 |=  (  ((mode&RTS)?USART_CR3_RTSE:0)
                | ((mode&CTS)?USART_CR3_CTSE:0) ); // set hardware flow control

  setBaudrate( baud );

  ptr->CR1 |= (  USART_CR1_UE       // Enable USART
               | USART_CR1_TCIE     // Transm. complete interr. enable
               | USART_CR1_RXNEIE );// Receive reg. not empty interr. enable
}

//-------------------------------------------------------------------
inline BYTE Uart_Mcu::getByte( void )
{
  return( ptr->RDR );
}

//-------------------------------------------------------------------
inline void Uart_Mcu::setByte( BYTE c )
{
  ptr->TDR = c;
}

//-------------------------------------------------------------------
// Configure the USART Baud Rate:
//
// OVER8==0:
//   BRR: mmmm mmmm mmmm ffff
//   div: Fixpoint number = M+F/16
//
//            fck                         fck
//   div = ---------  =>  BRR = 16*div = ------
//         16 x baud                      baud
//
void Uart_Mcu::setBaudrate( DWORD baud )
{
  if( baud > 1500)
  {
    DWORD brr = (WORD)(2.0*system_PCLK_UART / baud);
    ptr->BRR  = (brr&0XFFF0) | ((brr>>1)&0x0007);
    ptr->CR1 |= USART_CR1_OVER8;
  }
  else
  {
    DWORD brr = (WORD)(system_PCLK_UART / baud);
    ptr->BRR  = brr;
    ptr->CR1 &= ~USART_CR1_OVER8;
  }
}

//-------------------------------------------------------------------
void Uart_Mcu::isr( void )
{
  if( ptr->ISR & USART_ISR_RXNE ) // Receive data register not empty?
  {
    isrRx();
  }
  if( ptr->ISR & USART_ISR_TC ) // Transmission complete?
  {
    ptr->ICR |= USART_ICR_TCCF;
    isrTx();
  }
}

//-------------------------------------------------------------------
extern "C"
{
  void USART1_IRQHandler(void)
  {
    Uart_Mcu::uartPtr_1->isr();
  }
}

//-------------------------------------------------------------------
extern "C"
{
  void USART2_IRQHandler(void)
  {
    Uart_Mcu::uartPtr_2->isr();
  }
}

//-------------------------------------------------------------------
extern "C"
{
  void USART3_IRQHandler(void)
  {
    Uart_Mcu::uartPtr_3->isr();
  }
}

//-------------------------------------------------------------------
extern "C"
{
  void UART4_IRQHandler(void)
  {
    Uart_Mcu::uartPtr_4->isr();
  }
}

//-------------------------------------------------------------------
extern "C"
{
  void UART5_IRQHandler(void)
  {
    Uart_Mcu::uartPtr_5->isr();
  }
}

//-------------------------------------------------------------------
extern "C"
{
  void USART6_IRQHandler(void)
  {
    Uart_Mcu::uartPtr_6->isr();
  }
}

//-------------------------------------------------------------------
extern "C"
{
  void UART7_IRQHandler(void)
  {
    Uart_Mcu::uartPtr_7->isr();
  }
}

}  } //namespace

//EOF
