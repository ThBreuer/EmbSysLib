//###################################################################
//
// DON'T EDIT!
//
// This file is auto generated by
//    >php -f PinConfig.php PinConfig_STM32L432.csv
//
//###################################################################


//*******************************************************************
#include "Mcu_Types.h"
#include "Std/Report.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class PinConfig

\brief Port pin configuration

This class provides methods to
- configure GPIO mode, type, pull-up/-down, speed and alternate
  function
- connect a GPIO to an alternate function
*/
class PinConfig
{
  public:
    //---------------------------------------------------------------
    /*! \enum MODE
        \brief Pin mode configuration
    */
    typedef enum
    {
      // MODER
      INPUT          = 0x0000, //!< Input mode
      OUTPUT         = 0x0001, //!< Output mode
      ALTERNATE_FUNC = 0x0002, //!< Alternate function
      ANALOG         = 0x0003, //!< Analog I/O

      // OTYPER
      PUSH_PULL      = 0x0000, //!< Enable push-pull
      OPEN_DRAIN     = 0x0010, //!< Disable push-pull

      //PUPDR
      NO_PUPD        = 0x0000, //!< No pull-up or pull down
      PULL_UP        = 0x0100, //!< Enable pull-up
      PULL_DOWN      = 0x0200, //!< Enable pull-down

      //OSPEEDR
      LOW_SPEED      = 0x0000, //!< Low speed mode
      MEDIUM_SPEED   = 0x1000, //!< Medium speed mode
      FAST_SPEED     = 0x2000, //!< Fast speed mode
      HIGH_SPEED     = 0x3000  //!< High speed mode

    } MODE;

    //---------------------------------------------------------------
    /*! Alternate functions of the MCU
    */
    typedef enum
    { 
      ADC1_IN10,
      ADC1_IN11,
      ADC1_IN12,
      ADC1_IN15,
      ADC1_IN16,
      ADC1_IN5,
      ADC1_IN6,
      ADC1_IN7,
      ADC1_IN8,
      ADC1_IN9,
      CAN1_RX,
      CAN1_TX,
      COMP1_OUT,
      COMP2_OUT,
      DAC1_OUT1,
      DAC1_OUT2,
      I2C1_SCL,
      I2C1_SDA,
      I2C1_SMBA,
      I2C3_SCL,
      I2C3_SDA,
      IR_OUT,
      LPTIM1_ETR,
      LPTIM1_IN1,
      LPTIM1_IN2,
      LPTIM1_OUT,
      LPTIM2_ETR,
      LPTIM2_IN1,
      LPTIM2_OUT,
      LPUART1_CTS,
      LPUART1_RTS_DE,
      LPUART1_RX,
      LPUART1_TX,
      QUADSPI_BK1_IO0,
      QUADSPI_BK1_IO1,
      QUADSPI_BK1_IO2,
      QUADSPI_BK1_IO3,
      QUADSPI_BK1_NCS,
      QUADSPI_CLK,
      RCC_OSC_IN,
      RCC_OSC_OUT,
      SAI1_EXTCLK,
      SAI1_FS_A,
      SAI1_FS_B,
      SAI1_MCLK_A,
      SAI1_MCLK_B,
      SAI1_SCK_A,
      SAI1_SCK_B,
      SAI1_SD_A,
      SAI1_SD_B,
      SPI1_MISO,
      SPI1_MOSI,
      SPI1_NSS,
      SPI1_SCK,
      SPI3_MISO,
      SPI3_MOSI,
      SPI3_NSS,
      SPI3_SCK,
      SWPMI1_IO,
      SWPMI1_RX,
      SWPMI1_SUSPEND,
      SWPMI1_TX,
      TIM15_BKIN,
      TIM15_CH1,
      TIM15_CH1N,
      TIM15_CH2,
      TIM16_BKIN,
      TIM16_CH1,
      TIM16_CH1N,
      TIM1_BKIN,
      TIM1_BKIN2,
      TIM1_BKIN2_COMP1,
      TIM1_BKIN_COMP2,
      TIM1_CH1,
      TIM1_CH1N,
      TIM1_CH2,
      TIM1_CH2N,
      TIM1_CH3,
      TIM1_CH3N,
      TIM1_CH4,
      TIM1_ETR,
      TIM2_CH1,
      TIM2_CH2,
      TIM2_CH3,
      TIM2_CH4,
      TIM2_ETR,
      TSC_G2_IO1,
      TSC_G2_IO2,
      TSC_G2_IO3,
      TSC_G2_IO4,
      TSC_G3_IO1,
      USART1_CK,
      USART1_CTS,
      USART1_RTS_DE,
      USART1_RX,
      USART1_TX,
      USART2_CK,
      USART2_CTS,
      USART2_RTS_DE,
      USART2_RX,
      USART2_TX,
      USART3_CK,
      USART3_CTS,
      USART3_RTS_DE,
      USB_CRS_SYNC,
      USB_DM,
      USB_DP,
      USB_NOE,
    } Function;

    //---------------------------------------------------------------
    /*! Pin function mapping

        Maps an alternate function to a port pin and alternate function ID.
       'ADC12_IN4_PA4' means: The function 'ADC12_IN4' is mapped to pin PA4
    */
    typedef enum
    { 
      ADC1_IN10_PA5        = (ADC1_IN10           <<16)|( 0<<12)|( 5<<4)|( 0<<0), //!<\n
      ADC1_IN11_PA6        = (ADC1_IN11           <<16)|( 0<<12)|( 6<<4)|( 0<<0), //!<\n
      ADC1_IN12_PA7        = (ADC1_IN12           <<16)|( 0<<12)|( 7<<4)|( 0<<0), //!<\n
      ADC1_IN15_PB0        = (ADC1_IN15           <<16)|( 1<<12)|( 0<<4)|( 0<<0), //!<\n
      ADC1_IN16_PB1        = (ADC1_IN16           <<16)|( 1<<12)|( 1<<4)|( 0<<0), //!<\n
      ADC1_IN5_PA0         = (ADC1_IN5            <<16)|( 0<<12)|( 0<<4)|( 0<<0), //!<\n
      ADC1_IN6_PA1         = (ADC1_IN6            <<16)|( 0<<12)|( 1<<4)|( 0<<0), //!<\n
      ADC1_IN7_PA2         = (ADC1_IN7            <<16)|( 0<<12)|( 2<<4)|( 0<<0), //!<\n
      ADC1_IN8_PA3         = (ADC1_IN8            <<16)|( 0<<12)|( 3<<4)|( 0<<0), //!<\n
      ADC1_IN9_PA4         = (ADC1_IN9            <<16)|( 0<<12)|( 4<<4)|( 0<<0), //!<\n
      CAN1_RX_PA11         = (CAN1_RX             <<16)|( 0<<12)|(11<<4)|( 9<<0), //!<\n
      CAN1_TX_PA12         = (CAN1_TX             <<16)|( 0<<12)|(12<<4)|( 9<<0), //!<\n
      COMP1_OUT_PA0        = (COMP1_OUT           <<16)|( 0<<12)|( 0<<4)|(12<<0), //!<\n
      COMP1_OUT_PA11       = (COMP1_OUT           <<16)|( 0<<12)|(11<<4)|( 6<<0), //!<\n
      COMP1_OUT_PA6        = (COMP1_OUT           <<16)|( 0<<12)|( 6<<4)|( 6<<0), //!<\n
      COMP1_OUT_PB0        = (COMP1_OUT           <<16)|( 1<<12)|( 0<<4)|(12<<0), //!<\n
      COMP2_OUT_PA2        = (COMP2_OUT           <<16)|( 0<<12)|( 2<<4)|(12<<0), //!<\n
      COMP2_OUT_PA7        = (COMP2_OUT           <<16)|( 0<<12)|( 7<<4)|(12<<0), //!<\n
      COMP2_OUT_PB5        = (COMP2_OUT           <<16)|( 1<<12)|( 5<<4)|(12<<0), //!<\n
      DAC1_OUT1_PA4        = (DAC1_OUT1           <<16)|( 0<<12)|( 4<<4)|( 0<<0), //!<\n
      DAC1_OUT2_PA5        = (DAC1_OUT2           <<16)|( 0<<12)|( 5<<4)|( 0<<0), //!<\n
      I2C1_SCL_PA9         = (I2C1_SCL            <<16)|( 0<<12)|( 9<<4)|( 4<<0), //!<\n
      I2C1_SCL_PB6         = (I2C1_SCL            <<16)|( 1<<12)|( 6<<4)|( 4<<0), //!<\n
      I2C1_SDA_PA10        = (I2C1_SDA            <<16)|( 0<<12)|(10<<4)|( 4<<0), //!<\n
      I2C1_SDA_PB7         = (I2C1_SDA            <<16)|( 1<<12)|( 7<<4)|( 4<<0), //!<\n
      I2C1_SMBA_PA1        = (I2C1_SMBA           <<16)|( 0<<12)|( 1<<4)|( 4<<0), //!<\n
      I2C1_SMBA_PA14       = (I2C1_SMBA           <<16)|( 0<<12)|(14<<4)|( 4<<0), //!<\n
      I2C1_SMBA_PB5        = (I2C1_SMBA           <<16)|( 1<<12)|( 5<<4)|( 4<<0), //!<\n
      I2C3_SCL_PA7         = (I2C3_SCL            <<16)|( 0<<12)|( 7<<4)|( 4<<0), //!<\n
      I2C3_SDA_PB4         = (I2C3_SDA            <<16)|( 1<<12)|( 4<<4)|( 4<<0), //!<\n
      IR_OUT_PA13          = (IR_OUT              <<16)|( 0<<12)|(13<<4)|( 1<<0), //!<\n
      LPTIM1_ETR_PB6       = (LPTIM1_ETR          <<16)|( 1<<12)|( 6<<4)|( 1<<0), //!<\n
      LPTIM1_IN1_PB5       = (LPTIM1_IN1          <<16)|( 1<<12)|( 5<<4)|( 1<<0), //!<\n
      LPTIM1_IN2_PB7       = (LPTIM1_IN2          <<16)|( 1<<12)|( 7<<4)|( 1<<0), //!<\n
      LPTIM1_OUT_PA14      = (LPTIM1_OUT          <<16)|( 0<<12)|(14<<4)|( 1<<0), //!<\n
      LPTIM2_ETR_PA5       = (LPTIM2_ETR          <<16)|( 0<<12)|( 5<<4)|(14<<0), //!<\n
      LPTIM2_IN1_PB1       = (LPTIM2_IN1          <<16)|( 1<<12)|( 1<<4)|(14<<0), //!<\n
      LPTIM2_OUT_PA4       = (LPTIM2_OUT          <<16)|( 0<<12)|( 4<<4)|(14<<0), //!<\n
      LPTIM2_OUT_PA8       = (LPTIM2_OUT          <<16)|( 0<<12)|( 8<<4)|(14<<0), //!<\n
      LPUART1_CTS_PA6      = (LPUART1_CTS         <<16)|( 0<<12)|( 6<<4)|( 8<<0), //!<\n
      LPUART1_RTS_DE_PB1   = (LPUART1_RTS_DE      <<16)|( 1<<12)|( 1<<4)|( 8<<0), //!<\n
      LPUART1_RX_PA3       = (LPUART1_RX          <<16)|( 0<<12)|( 3<<4)|( 8<<0), //!<\n
      LPUART1_TX_PA2       = (LPUART1_TX          <<16)|( 0<<12)|( 2<<4)|( 8<<0), //!<\n
      QUADSPI_BK1_IO0_PB1  = (QUADSPI_BK1_IO0     <<16)|( 1<<12)|( 1<<4)|(10<<0), //!<\n
      QUADSPI_BK1_IO1_PB0  = (QUADSPI_BK1_IO1     <<16)|( 1<<12)|( 0<<4)|(10<<0), //!<\n
      QUADSPI_BK1_IO2_PA7  = (QUADSPI_BK1_IO2     <<16)|( 0<<12)|( 7<<4)|(10<<0), //!<\n
      QUADSPI_BK1_IO3_PA6  = (QUADSPI_BK1_IO3     <<16)|( 0<<12)|( 6<<4)|(10<<0), //!<\n
      QUADSPI_BK1_NCS_PA2  = (QUADSPI_BK1_NCS     <<16)|( 0<<12)|( 2<<4)|(10<<0), //!<\n
      QUADSPI_CLK_PA3      = (QUADSPI_CLK         <<16)|( 0<<12)|( 3<<4)|(10<<0), //!<\n
      RCC_OSC_IN_PH0       = (RCC_OSC_IN          <<16)|( 7<<12)|( 0<<4)|( 0<<0), //!<\n
      RCC_OSC_OUT_PH1      = (RCC_OSC_OUT         <<16)|( 7<<12)|( 1<<4)|( 0<<0), //!<\n
      SAI1_EXTCLK_PA0      = (SAI1_EXTCLK         <<16)|( 0<<12)|( 0<<4)|(13<<0), //!<\n
      SAI1_EXTCLK_PB0      = (SAI1_EXTCLK         <<16)|( 1<<12)|( 0<<4)|(13<<0), //!<\n
      SAI1_FS_A_PA9        = (SAI1_FS_A           <<16)|( 0<<12)|( 9<<4)|(13<<0), //!<\n
      SAI1_FS_B_PA14       = (SAI1_FS_B           <<16)|( 0<<12)|(14<<4)|(13<<0), //!<\n
      SAI1_FS_B_PA4        = (SAI1_FS_B           <<16)|( 0<<12)|( 4<<4)|(13<<0), //!<\n
      SAI1_FS_B_PB6        = (SAI1_FS_B           <<16)|( 1<<12)|( 6<<4)|(13<<0), //!<\n
      SAI1_MCLK_A_PA3      = (SAI1_MCLK_A         <<16)|( 0<<12)|( 3<<4)|(13<<0), //!<\n
      SAI1_MCLK_B_PB4      = (SAI1_MCLK_B         <<16)|( 1<<12)|( 4<<4)|(13<<0), //!<\n
      SAI1_SCK_A_PA8       = (SAI1_SCK_A          <<16)|( 0<<12)|( 8<<4)|(13<<0), //!<\n
      SAI1_SCK_B_PB3       = (SAI1_SCK_B          <<16)|( 1<<12)|( 3<<4)|(13<<0), //!<\n
      SAI1_SD_A_PA10       = (SAI1_SD_A           <<16)|( 0<<12)|(10<<4)|(13<<0), //!<\n
      SAI1_SD_B_PA13       = (SAI1_SD_B           <<16)|( 0<<12)|(13<<4)|(13<<0), //!<\n
      SAI1_SD_B_PB5        = (SAI1_SD_B           <<16)|( 1<<12)|( 5<<4)|(13<<0), //!<\n
      SPI1_MISO_PA11       = (SPI1_MISO           <<16)|( 0<<12)|(11<<4)|( 5<<0), //!<\n
      SPI1_MISO_PA6        = (SPI1_MISO           <<16)|( 0<<12)|( 6<<4)|( 5<<0), //!<\n
      SPI1_MISO_PB4        = (SPI1_MISO           <<16)|( 1<<12)|( 4<<4)|( 5<<0), //!<\n
      SPI1_MOSI_PA12       = (SPI1_MOSI           <<16)|( 0<<12)|(12<<4)|( 5<<0), //!<\n
      SPI1_MOSI_PA7        = (SPI1_MOSI           <<16)|( 0<<12)|( 7<<4)|( 5<<0), //!<\n
      SPI1_MOSI_PB5        = (SPI1_MOSI           <<16)|( 1<<12)|( 5<<4)|( 5<<0), //!<\n
      SPI1_NSS_PA15        = (SPI1_NSS            <<16)|( 0<<12)|(15<<4)|( 5<<0), //!<\n
      SPI1_NSS_PA4         = (SPI1_NSS            <<16)|( 0<<12)|( 4<<4)|( 5<<0), //!<\n
      SPI1_NSS_PB0         = (SPI1_NSS            <<16)|( 1<<12)|( 0<<4)|( 5<<0), //!<\n
      SPI1_SCK_PA1         = (SPI1_SCK            <<16)|( 0<<12)|( 1<<4)|( 5<<0), //!<\n
      SPI1_SCK_PA5         = (SPI1_SCK            <<16)|( 0<<12)|( 5<<4)|( 5<<0), //!<\n
      SPI1_SCK_PB3         = (SPI1_SCK            <<16)|( 1<<12)|( 3<<4)|( 5<<0), //!<\n
      SPI3_MISO_PB4        = (SPI3_MISO           <<16)|( 1<<12)|( 4<<4)|( 6<<0), //!<\n
      SPI3_MOSI_PB5        = (SPI3_MOSI           <<16)|( 1<<12)|( 5<<4)|( 6<<0), //!<\n
      SPI3_NSS_PA15        = (SPI3_NSS            <<16)|( 0<<12)|(15<<4)|( 6<<0), //!<\n
      SPI3_NSS_PA4         = (SPI3_NSS            <<16)|( 0<<12)|( 4<<4)|( 6<<0), //!<\n
      SPI3_SCK_PB3         = (SPI3_SCK            <<16)|( 1<<12)|( 3<<4)|( 6<<0), //!<\n
      SWPMI1_IO_PA8        = (SWPMI1_IO           <<16)|( 0<<12)|( 8<<4)|(12<<0), //!<\n
      SWPMI1_RX_PA14       = (SWPMI1_RX           <<16)|( 0<<12)|(14<<4)|(12<<0), //!<\n
      SWPMI1_SUSPEND_PA15  = (SWPMI1_SUSPEND      <<16)|( 0<<12)|(15<<4)|(12<<0), //!<\n
      SWPMI1_TX_PA13       = (SWPMI1_TX           <<16)|( 0<<12)|(13<<4)|(12<<0), //!<\n
      TIM15_BKIN_PA9       = (TIM15_BKIN          <<16)|( 0<<12)|( 9<<4)|(14<<0), //!<\n
      TIM15_CH1_PA2        = (TIM15_CH1           <<16)|( 0<<12)|( 2<<4)|(14<<0), //!<\n
      TIM15_CH1N_PA1       = (TIM15_CH1N          <<16)|( 0<<12)|( 1<<4)|(14<<0), //!<\n
      TIM15_CH2_PA3        = (TIM15_CH2           <<16)|( 0<<12)|( 3<<4)|(14<<0), //!<\n
      TIM16_BKIN_PB5       = (TIM16_BKIN          <<16)|( 1<<12)|( 5<<4)|(14<<0), //!<\n
      TIM16_CH1_PA6        = (TIM16_CH1           <<16)|( 0<<12)|( 6<<4)|(14<<0), //!<\n
      TIM16_CH1N_PB6       = (TIM16_CH1N          <<16)|( 1<<12)|( 6<<4)|(14<<0), //!<\n
      TIM1_BKIN_PA6        = (TIM1_BKIN           <<16)|( 0<<12)|( 6<<4)|( 1<<0), //!<\n
      TIM1_BKIN2_PA11      = (TIM1_BKIN2          <<16)|( 0<<12)|(11<<4)|( 2<<0), //!<\n
      TIM1_BKIN2_COMP1_PA11 = (TIM1_BKIN2_COMP1    <<16)|( 0<<12)|(11<<4)|(12<<0), //!<\n
      TIM1_BKIN_COMP2_PA6  = (TIM1_BKIN_COMP2     <<16)|( 0<<12)|( 6<<4)|(12<<0), //!<\n
      TIM1_CH1_PA8         = (TIM1_CH1            <<16)|( 0<<12)|( 8<<4)|( 1<<0), //!<\n
      TIM1_CH1N_PA7        = (TIM1_CH1N           <<16)|( 0<<12)|( 7<<4)|( 1<<0), //!<\n
      TIM1_CH2_PA9         = (TIM1_CH2            <<16)|( 0<<12)|( 9<<4)|( 1<<0), //!<\n
      TIM1_CH2N_PB0        = (TIM1_CH2N           <<16)|( 1<<12)|( 0<<4)|( 1<<0), //!<\n
      TIM1_CH3_PA10        = (TIM1_CH3            <<16)|( 0<<12)|(10<<4)|( 1<<0), //!<\n
      TIM1_CH3N_PB1        = (TIM1_CH3N           <<16)|( 1<<12)|( 1<<4)|( 1<<0), //!<\n
      TIM1_CH4_PA11        = (TIM1_CH4            <<16)|( 0<<12)|(11<<4)|( 1<<0), //!<\n
      TIM1_ETR_PA12        = (TIM1_ETR            <<16)|( 0<<12)|(12<<4)|( 1<<0), //!<\n
      TIM2_CH1_PA0         = (TIM2_CH1            <<16)|( 0<<12)|( 0<<4)|( 1<<0), //!<\n
      TIM2_CH1_PA15        = (TIM2_CH1            <<16)|( 0<<12)|(15<<4)|( 1<<0), //!<\n
      TIM2_CH1_PA5         = (TIM2_CH1            <<16)|( 0<<12)|( 5<<4)|( 1<<0), //!<\n
      TIM2_CH2_PA1         = (TIM2_CH2            <<16)|( 0<<12)|( 1<<4)|( 1<<0), //!<\n
      TIM2_CH2_PB3         = (TIM2_CH2            <<16)|( 1<<12)|( 3<<4)|( 1<<0), //!<\n
      TIM2_CH3_PA2         = (TIM2_CH3            <<16)|( 0<<12)|( 2<<4)|( 1<<0), //!<\n
      TIM2_CH4_PA3         = (TIM2_CH4            <<16)|( 0<<12)|( 3<<4)|( 1<<0), //!<\n
      TIM2_ETR_PA0         = (TIM2_ETR            <<16)|( 0<<12)|( 0<<4)|(14<<0), //!<\n
      TIM2_ETR_PA15        = (TIM2_ETR            <<16)|( 0<<12)|(15<<4)|( 2<<0), //!<\n
      TIM2_ETR_PA5         = (TIM2_ETR            <<16)|( 0<<12)|( 5<<4)|( 2<<0), //!<\n
      TSC_G2_IO1_PB4       = (TSC_G2_IO1          <<16)|( 1<<12)|( 4<<4)|( 9<<0), //!<\n
      TSC_G2_IO2_PB5       = (TSC_G2_IO2          <<16)|( 1<<12)|( 5<<4)|( 9<<0), //!<\n
      TSC_G2_IO3_PB6       = (TSC_G2_IO3          <<16)|( 1<<12)|( 6<<4)|( 9<<0), //!<\n
      TSC_G2_IO4_PB7       = (TSC_G2_IO4          <<16)|( 1<<12)|( 7<<4)|( 9<<0), //!<\n
      TSC_G3_IO1_PA15      = (TSC_G3_IO1          <<16)|( 0<<12)|(15<<4)|( 9<<0), //!<\n
      USART1_CK_PA8        = (USART1_CK           <<16)|( 0<<12)|( 8<<4)|( 7<<0), //!<\n
      USART1_CK_PB5        = (USART1_CK           <<16)|( 1<<12)|( 5<<4)|( 7<<0), //!<\n
      USART1_CTS_PA11      = (USART1_CTS          <<16)|( 0<<12)|(11<<4)|( 7<<0), //!<\n
      USART1_CTS_PB4       = (USART1_CTS          <<16)|( 1<<12)|( 4<<4)|( 7<<0), //!<\n
      USART1_RTS_DE_PA12   = (USART1_RTS_DE       <<16)|( 0<<12)|(12<<4)|( 7<<0), //!<\n
      USART1_RTS_DE_PB3    = (USART1_RTS_DE       <<16)|( 1<<12)|( 3<<4)|( 7<<0), //!<\n
      USART1_RX_PA10       = (USART1_RX           <<16)|( 0<<12)|(10<<4)|( 7<<0), //!<\n
      USART1_RX_PB7        = (USART1_RX           <<16)|( 1<<12)|( 7<<4)|( 7<<0), //!<\n
      USART1_TX_PA9        = (USART1_TX           <<16)|( 0<<12)|( 9<<4)|( 7<<0), //!<\n
      USART1_TX_PB6        = (USART1_TX           <<16)|( 1<<12)|( 6<<4)|( 7<<0), //!<\n
      USART2_CK_PA4        = (USART2_CK           <<16)|( 0<<12)|( 4<<4)|( 7<<0), //!<\n
      USART2_CTS_PA0       = (USART2_CTS          <<16)|( 0<<12)|( 0<<4)|( 7<<0), //!<\n
      USART2_RTS_DE_PA1    = (USART2_RTS_DE       <<16)|( 0<<12)|( 1<<4)|( 7<<0), //!<\n
      USART2_RX_PA15       = (USART2_RX           <<16)|( 0<<12)|(15<<4)|( 3<<0), //!<\n
      USART2_RX_PA3        = (USART2_RX           <<16)|( 0<<12)|( 3<<4)|( 7<<0), //!<\n
      USART2_TX_PA2        = (USART2_TX           <<16)|( 0<<12)|( 2<<4)|( 7<<0), //!<\n
      USART3_CK_PB0        = (USART3_CK           <<16)|( 1<<12)|( 0<<4)|( 7<<0), //!<\n
      USART3_CTS_PA6       = (USART3_CTS          <<16)|( 0<<12)|( 6<<4)|( 7<<0), //!<\n
      USART3_RTS_DE_PA15   = (USART3_RTS_DE       <<16)|( 0<<12)|(15<<4)|( 7<<0), //!<\n
      USART3_RTS_DE_PB1    = (USART3_RTS_DE       <<16)|( 1<<12)|( 1<<4)|( 7<<0), //!<\n
      USB_CRS_SYNC_PA10    = (USB_CRS_SYNC        <<16)|( 0<<12)|(10<<4)|(10<<0), //!<\n
      USB_DM_PA11          = (USB_DM              <<16)|( 0<<12)|(11<<4)|(10<<0), //!<\n
      USB_DP_PA12          = (USB_DP              <<16)|( 0<<12)|(12<<4)|(10<<0), //!<\n
      USB_NOE_PA13         = (USB_NOE             <<16)|( 0<<12)|(13<<4)|(10<<0), //!<\n
      END_OF_TABLE = -1
    } MAP;

  public:
    //---------------------------------------------------------------
    /*! Set configuration of a GPIO
        \param gpio  Pointer to function register.
                     Use predefined constant \a 'GPIOx'
        \param pinId GPIO pin number (0,...15)
        \param mode  Selected pin mode.
                     Use constants defined in \b MODE, the constants
                     can be combined using bit-or operations.
        \param af    Number of selected alternate function. This
                     parameter is used only, when \b ALTERNATE_FUNC
                     is selected.
    */
    static void set( GPIO_TypeDef *gpio,
                     BYTE          pinId,
                     DWORD         mode,
                     BYTE          af = 0 );

    //---------------------------------------------------------------
    /*! Set GPIO to alternate function

        A GPIO can be connected to a onchip peripheral module. The
        enum \b 'Function' lists all peripheral signals which can be
        connected to a GPIO. This method uses a application specific
        mapping \b 'table'. The application has to define
        this \b 'table' on global scope. The entries of this table
        define on which port pin a peripheral signal is mapped.
        \param func  Selected peripheral signal as alternate function
        \param mode  Selected pin mode.
                     Use constants defined in \b MODE, the constants
                     can be combined using bit-or operations.
        \return
                - true, if \b func is included in \b table
                - false otherwise
    */
    static bool set( Function func,
                     DWORD    mode = 0 );

  public:
    //---------------------------------------------------------------
    /*! This table defines how an alternate function is mapped to the
        port pins. It must be defined by application software. The
        last entry of this table has to be \b 'END_OF_TABLE'.
    */
    static MAP table[];
    static Std::Report report;

}; //PinConfig

}  } //namespace
