//*******************************************************************
/*!
\file   ReportID_Hw.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   19.11.2021

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_REPORT_ID_H
#define _HW_REPORT_ID_H

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class ReportID_Hw
\brief Definition of hardware related Module IDs and Error-Codes

\see Std::Report
*/
class ReportID_Hw
{
  public:
    //---------------------------------------------------------------
    /*!
    \class Module
    \brief Definition of module ID used by hardware modules
    */
    class Module
    {
      public:
        //-----------------------------------------------------------
        static const WORD  PORT_MCU                 = 0x0010; //!< Port_Mcu
        static const WORD  PORT_VIRTUAL             = 0x0011; //!< Port_Virtual
        static const WORD  PORT_PERIPHERAL          = 0x0012; //!< Port_Peripheral
        static const WORD  UART_MCU                 = 0x0020; //!< Uart_Mcu
        static const WORD  UART_STDIO               = 0x0021; //!< Uart_Stdio
        static const WORD  UART_SERIAL              = 0x0022; //!< Uart_Serial, original Windows error codes
        static const WORD  UART_FILE                = 0x0023; //!< Uart_File
        static const WORD  TIMER_MCU                = 0x0030; //!< Timer_Mcu
        static const WORD  ADC_MCU                  = 0x0040; //!< Adc_Mcu
        static const WORD  ADC_VIRTUAL              = 0x0041; //!< Adc_Virtual
        static const WORD  DAC_MCU                  = 0x0050; //!< Dac_Mcu
        static const WORD  DAC_VIRTUAL              = 0x0051; //!< Dac_Virtual
        static const WORD  USB_DEVICE_MCU           = 0x0060; //!< USBdevice_Mcu
        static const WORD  USB_HOST_MCU             = 0x0070; //!< USBhost_Mcu
        static const WORD  MEMORY_MCU               = 0x0080; //!< Memory_Mcu
        static const WORD  MEMORY_FLASH             = 0x0081; //!< Memory_Flash
        static const WORD  MEMORY_EEPROM            = 0x0082; //!< Memory_EEPROM
        static const WORD  MEMORY_BKRAM             = 0x0083; //!< Memory_BKRAM
        static const WORD  MEMORY_PERIPHERAL        = 0x0084; //!< Memory_Peripheral
        static const WORD  MEMORY_IMAGE             = 0x008F; //!< MemoryImage
        
        static const WORD  RTC_MCU                  = 0x0090; //!< Rtc_Mcu
        static const WORD  RTC_PERIPHERAL           = 0x0091; //!< Rtc_Peripheral
        static const WORD  DISPLAY_CHAR_VIRTUAL     = 0x00A0; //!< DisplayChar_Virtual
        static const WORD  DISPLAY_CHAR_TERMINAL    = 0x00A1; //!< DisplayChar_Terminal
        static const WORD  DISPLAY_GRAPHIC_VIRTUAL  = 0x00B0; //!< DisplayGraphic_Virtual
        static const WORD  DISPLAY_GRAPHIC_SSD2119  = 0x00B1; //!< DisplayGraphic SSD2119
        static const WORD  DISPLAY_GRAPHIC_OTM8009A = 0x00B2; //!< DisplayGraphic OTM8009A
        static const WORD  DISPLAY_GRAPHIC_NT35510  = 0x00B3; //!< DisplayGraphic NT35510
        static const WORD  TOUCH_VIRTUAL            = 0x00C0; //!< Touch_Virtual
        static const WORD  TOUCH_STMPE811           = 0x00C1; //!< Touch_STMPE811
        static const WORD  TOUCH_FT6206             = 0x00C2; //!< Touch_FT6206    
        static const WORD  TOUCH_FT5336             = 0x00C3; //!< Touch_FT5336    
        static const WORD  I2C_MASTER_MCU           = 0x00D0; //!< I2C_Mcu
        static const WORD  I2C_MASTER_EMUL          = 0x00D2; //!< I2C_Mcu
        
        static const WORD  NET_WIN_UDP              = 0x00E1; //!< NetWinUDP
        static const WORD  NET_WIN_TCP              = 0x00E2; //!< NetWinTCP
        
        static const WORD  SYSTEM_PIN_CONFIG        = 0x0F00; //!< PinConfig
        static const WORD  SYSTEM_WSA               = 0x0F01; //!< WSA, original WSA error codes
        static const WORD  SYSTEM_UDP_CLIENT        = 0x0F02; //!< UDPclient
        static const WORD  SYSTEM_UDP_SERVER        = 0x0F03; //!< UDPserver
        static const WORD  SYSTEM_LIB_USB           = 0x0F04; //!< libusb, original libusb error codes (but positiv)
    };

  public:
    //---------------------------------------------------------------
    /*!
    \class Event
    \brief Definition of event ID used by hardware modules
    */
    class Event
    {
      public:
        //-----------------------------------------------------------
        static const WORD  WRONG_VERSION           = 0x0001; //!< Version mismatch
        static const WORD  FUNCTION_NOT_SUPPORTED  = 0x0002; //!< Function not implemented
        static const WORD  FUNCTION_NOT_EXECUTED   = 0x0003; //!< The Function cannot be executed
        static const WORD  CONFIG_NOT_SUPPORTED    = 0x0004; //!< The configuration is not valid
        static const WORD  MEMORY_ALLOCTION_ERROR  = 0x0005; //!< The requested memory could not be allocated
        static const WORD  VALUE_OUT_OF_RANGE      = 0x0006; //!< A value is out of the valid range
        static const WORD  WRONG_ID                = 0x0007; //!< The given ID is not valid
        static const WORD  WRONG_CHANNEL           = 0x0008; //!< The given channel is not valid
        static const WORD  OPEN_ERROR              = 0x0009; //!< Device could not be opened
        static const WORD  READ_ERROR              = 0x000A; //!< Cannot be read from the device (not opened or not readable)
        static const WORD  WRITE_ERROR             = 0x000B; //!< The device can not writen to (not opened or not writeable)
        static const WORD  SIZE_ERROR              = 0x000C; //!< Wrong data size in transmission
        static const WORD  HARDWARE_NOT_SUPPORTED  = 0x000D; //!< The requested hardware component is not supported
        static const WORD  THREAD_NOT_CREATED      = 0x000E; //!< The thread could not be created
        static const WORD  NO_DEVICE_HANDLE        = 0x000F; //!< Device handler not valid
        static const WORD  POINTER_NOT_AVAILABLE   = 0x0010; //!< Pointer is not available
        static const WORD  STARTUP_FAILED          = 0x0011; //!< Startup failed
        static const WORD  INVALID_HANDLE          = 0x0012; //!< Invalid handle
    };

}; //class ReportID_Hw

}  } //namespace

#endif
