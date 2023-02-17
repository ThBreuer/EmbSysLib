//*******************************************************************
/*!
\file   Dac.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_DAC_H
#define _HW_DAC_H

//*******************************************************************
#include "Mcu_Types.h"
#include "Std/Report.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class Dac

\brief Abstract class supporting the digital to analog converter (DAC)

This class is an interface to the DAC hardware. The given value is
immediately converted to a analog output. Multiple channels are
supported (limited by hardware). Independently of the resolution of
the used DAC, a 16 bit value is expected

Derived classes should at least:
 - initialize the DAC hardware
 - implement the set() method to set a sample value

\example HwDac.cpp
*/
class Dac
{
  protected:
    //---------------------------------------------------------------
    // Initialize the DAC
    // moduleId:     Id of this module, used for report
    //
    Dac( WORD moduleId );

  public:
    //---------------------------------------------------------------
    /*! Select a channel and set the value to convert
        \param ch    Channel ID
        \param value Converted value (16 bit)
    */
    virtual void set( BYTE ch, WORD value ) = 0;

    //---------------------------------------------------------------
    /*! Enables a DAC-Channel
        \param ch Channel ID (0,...)
    */
    virtual void enable( BYTE ch ) = 0;

  public:
    //---------------------------------------------------------------
    Std::Report report;

}; //Dac

} } //namespace

#endif
