//*******************************************************************
/*!
\file   I2Cslave.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   02.03.2011

License: See file "LICENSE"
*/

//*******************************************************************
#include "I2Cslave.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
// 
// I2Cslave
//
//*******************************************************************
//-------------------------------------------------------------------
I2Cslave::I2Cslave( DataHandler &handlerIn,
                          BYTE                    hwAddrIn  )
: handler(handlerIn)

{
  hwAddr = hwAddrIn;
}


} } //namespace

//EOF
