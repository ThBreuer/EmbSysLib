//*******************************************************************
/*!
\file   Analog.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   18.12.2023

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _DEV_ANALOG_H
#define _DEV_ANALOG_H

//*******************************************************************
namespace EmbSysLib {
namespace Dev {

//*******************************************************************
/*!
\class Analog

\brief Access to analog I/O devices

This class is an interface to analog I/O devices.

It supports also a linear conversion between raw value and physical
quantity

\example DevAnalog.cpp
*/
class Analog
{
  protected:
    //---------------------------------------------------------------
    // Instatiate an analog device
    // channel: Device input or output channel
    // gain:    Conversion factor
    // offset:  Conversion offset
    Analog( BYTE  channel,
            float gain,
            float offset );

  protected:
    //---------------------------------------------------------------
    BYTE  channel;
    float gain;
    float offs;

}; //class Analog

} } //namespace

#endif
