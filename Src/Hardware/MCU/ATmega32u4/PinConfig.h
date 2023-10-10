//*******************************************************************
/*!
\file   PinConfig.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   10.10.2023

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_PIN_CONFIG_H
#define _HW_PIN_CONFIG_H

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
      INPUT_PULLUP   = 0x0001, //!< Input mode
      OUTPUT         = 0x0002, //!< Output mode

    } MODE;

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
    static void set( volatile uint8_t *port,
                     BYTE  pinId,
                     DWORD mode,
                     BYTE  af = 0 );

  public:
    //---------------------------------------------------------------
    static Std::Report report;

}; //PinConfig

}  } //namespace

#endif
