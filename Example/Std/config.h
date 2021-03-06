//*******************************************************************
/*!
\file   Lib/Example/Std/config.h
\author Thomas Breuer
\date   19.03.2016
\brief  Configuration file
*/

//*******************************************************************
#include "lib.h"

//*******************************************************************
cSystem sys;

//*******************************************************************
//===================================================================
#if defined _MCU_TYPE_STM32L1XX
//===================================================================
  cHwPinConfig::MAP cHwPinConfig::table[] =
  {
    // UART
    USART1_TX_PA_9,
    USART1_RX_PA_10,

    END_OF_TABLE
  };

  cHwUART_N    uart ( cHwUART_N::USART_1, 
                      cHwUART::BR_9600, 
                      100, 
                      100 );

  cHwTimer_N   timer( cHwTimer_N::TIM_2, 1000 );

//===================================================================
#elif defined _MCU_TYPE_STM32F4XX
//===================================================================
  cHwPinConfig::MAP cHwPinConfig::table[] =
  {
    // UART
    USART6_RX_PC_7,
    USART6_TX_PC_6,

    END_OF_TABLE
  };

  cHwUART_N      uart ( cHwUART_N::USART_6, 
                         cHwUART::BR_9600, 
                         100, 
                         100 );

  cHwTimer_N     timer( cHwTimer_N::TIM_4,  100L/*us*/ );

//===================================================================
#else
  #error "config.h: device type not defined"
#endif
//===================================================================

cDevTextIO_UART com ( uart, 100, 100 ); 

//EOF
