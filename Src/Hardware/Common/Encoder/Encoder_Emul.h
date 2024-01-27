//*******************************************************************
/*!
\file   Encoder_Emul.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   26.01.2024

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_ENCODER_EMUL_H
#define _HW_ENCODER_EMUL_H

//*******************************************************************
#include "Hardware/Common/Encoder/Encoder.h"
#include "Hardware/Common/Port/Port.h"
#include "Hardware/Common/Timer/Timer.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class Encoder_Emul

\brief ...

\example HwEncoder.cpp

\todo Add documentation
\todo Pin configuration w/o Pull-up
*/
class Encoder_Emul : public Encoder, Timer::Task
{
  public:
    //---------------------------------------------------------------
    Encoder_Emul( Port::Pin &pinAIn,
                  Port::Pin &pinBIn,
                  Timer     &timer,
                  Mode       modeIn = NORMAL )
    : Encoder( modeIn ),
      pinA( pinAIn ),
      pinB( pinBIn )
    {
      timer.add( this );
      A_last = pinA.get();
      B_last = pinB.get();
      cnt    = 0;
    }

    //---------------------------------------------------------------
    virtual void update( void )
    {
      BYTE A_new = pinA.get();
      BYTE B_new = pinB.get();

      System::disableInterrupt();

      if( A_new != A_last )
      {
        if( A_new == B_new )
        {
          cnt++;
        }
        else
        {
          cnt--;
        }
      }
      else if( B_new != B_last )
      {
        if( B_new != A_new )
        {
          cnt++;
        }
        else
        {
          cnt--;
        }
      }
      System::enableInterrupt();

      A_last = A_new;
      B_last = B_new;
    }

    //---------------------------------------------------------------
    virtual int get( void )
    {
      System::disableInterrupt();
      int ret = cnt;
      cnt = 0;
      System::enableInterrupt();
      return( (mode&REVERSE)?ret:-ret );
    }

  private:
    //---------------------------------------------------------------
    Port::Pin &pinA;
    Port::Pin &pinB;
    BYTE A_last;
    BYTE B_last;

    int cnt;

}; //class Encoder_Emul

}  } //namespace

#endif
