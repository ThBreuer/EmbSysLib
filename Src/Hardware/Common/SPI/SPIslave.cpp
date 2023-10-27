//*******************************************************************
/*!
\file   SPIslave.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   20.03.2023

License: See file "LICENSE"
*/

//*******************************************************************
#include "SPIslave.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
// 
// SPIslave
//
//*******************************************************************
//-------------------------------------------------------------------
SPIslave::SPIslave( SPIslave::DataHandler &handlerIn )

: handler( handlerIn )

{
}

} } //namespace

//EOF
