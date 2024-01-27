//*******************************************************************
/*!
\file   Encoder.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_ENCODER_H
#define _HW_ENCODER_H

//*******************************************************************

#include "Hardware/Common/Port/Port.h"
#include "Hardware/Common/Timer/Timer.h"


//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*! 
\class Encoder

\brief Abstract class supporting a quadratur encoder

\example HwEncoder.cpp
*/
class Encoder
{
  public:
    //---------------------------------------------------------------
    /*! \enum Mode
        \brief Define encoder operating modes
    */
    typedef enum
    {
      NORMAL  = 0x00, //!< Normal count direction
      REVERSE = 0x01  //!< Reverse count direction
    } Mode;

  protected:
    //---------------------------------------------------------------
    // Standard constructor
    // 
    Encoder( Mode modeIn )
    {
      mode = modeIn;
    }

  public:
    //---------------------------------------------------------------
    /*! Get and reset counter
        \return Steps since last call of this function
    */
    virtual int get( void ) = 0;
  

  protected:
    //---------------------------------------------------------------
    Mode mode;

}; //class Encoder

}  } //namespace

#endif
