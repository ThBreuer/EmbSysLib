//*******************************************************************
/*!
\file   EmbSysLib.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"

\brief  Library main include file
*/

//*******************************************************************
#ifndef _EMB_SYS_LIB_H
#define _EMB_SYS_LIB_H

//*******************************************************************
//*******************************************************************
//! global namespace for everything
namespace EmbSysLib {

  //! Standard
  namespace Std {}

  //! Hardware related stuff
  namespace Hw {
    using namespace EmbSysLib::Std;
  }

  //! Device related stuff
  namespace Dev {
    using namespace EmbSysLib::Std;
    using namespace EmbSysLib::Hw;
  }

  //! Controls
  namespace Ctrl {
    using namespace EmbSysLib::Dev;
  }

  //! Modules
  namespace Mod {
    using namespace EmbSysLib::Std;
  }
}

//*******************************************************************
//
//*******************************************************************
#include "Mcu_Types.h"
#include "Hardware/MCU/System.h"

//===================================================================
// MCU
//===================================================================
#include "Mcu.h"

//===================================================================
// Std
//===================================================================
#include "Std/Std.h"
#include "Std/DataPointer.h"
#include "Std/Fifo.h"
#include "Std/Sequence.h"
#include "Std/Report.h"
#include "Std/Flag.h"

//===================================================================
// Hardware/Common
//===================================================================
#include "Hardware/Common/Analog/Adc.h"
#include "Hardware/Common/Analog/Dac.h"
#include "Hardware/Common/Display/Type/Font.h"
#include "Hardware/Common/Display/Type/Bitmap.h"
#include "Hardware/Common/Display/DisplayChar.h"
#include "Hardware/Common/Display/DisplayGraphic.h"
#include "Hardware/Common/I2C/I2Cmaster.h"
#include "Hardware/Common/I2C/I2Cmaster_Emul.h"
#include "Hardware/Common/I2C/I2Cslave.h"
#include "Hardware/Common/Memory/Memory.h"
#include "Hardware/Common/Memory/MemoryImage.h"
#include "Hardware/Common/Port/Port.h"
#include "Hardware/Common/Rtc/Rtc.h"
#include "Hardware/Common/SPI/SPImaster.h"
#include "Hardware/Common/Timer/Timer.h"
#include "Hardware/Common/Display/Touch.h"
#include "Hardware/Common/Uart/Uart.h"
#include "Hardware/Common/USB/USBdevice.h"
#include "Hardware/Common/USB/USBdevice/USBcontrol.h"
#include "Hardware/Common/USB/USBdevice/USBdescriptor.h"
#include "Hardware/Common/USB/USBdevice/USBendpoint.h"
#include "Hardware/Common/USB/USBdevice/USBinterface.h"
#include "Hardware/Common/USB/USBhost.h"

//===================================================================
// Device/Digital
//===================================================================
#include "Device/Digital/Digital.h"
#include "Device/Screen/ScreenChar.h"
#include "Device/Screen/ScreenGraphic.h"
#include "Device/Pointer/Pointer.h"
#include "Device/Task/TaskManager.h"
#include "Device/Terminal/Terminal.h"

//===================================================================
// Control
//===================================================================
#include "Control/DigitalButton.h"
#include "Control/DigitalEncoder.h"
#include "Control/DigitalEncoderJoystick.h"
#include "Control/DigitalEncoderRotaryknob.h"
#include "Control/DigitalIndicator.h"

//===================================================================
#endif
