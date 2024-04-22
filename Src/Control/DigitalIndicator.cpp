//*******************************************************************
/*!
\file   DigitalIndicator.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   26.01.2023

License: See file "LICENSE"
*/

//*******************************************************************
#include "DigitalIndicator.h"

//*******************************************************************
namespace EmbSysLib {
namespace Ctrl {

//*******************************************************************
//
// DigitalIndicator
//
//*******************************************************************
//-------------------------------------------------------------------
DigitalIndicator::DigitalIndicator( Digital     &digital,
                                    TaskManager &taskManager,
                                    BYTE         brightness  )
: digital( digital )
{
  clr();
  cycleTime = taskManager.getCycleTime();
  G = 25E3/cycleTime + 1; // Tics in 25ms (@40Hz), at least one
  setBrightness( brightness );
  taskManager.add(this);
}

//-------------------------------------------------------------------
void DigitalIndicator::update( void )
{
  if( runUpdate )
  {
    if( t >= maximum )
    {
      if( repeat && maximum )
      {
        t = 0;
      }
      else
      {
        clr();
      }
    }
    else
    {
      if( t == 0 )
      {
        //digital.set();
        P = pwr;
        M = G-pwr;
      }
      else if( t >= limit )
      {
        //digital.clr();
        P = 0;
        M = G;
      }
      t++;
    }
  }
  if( mean <= 0 )
  {
    digital.set(0);
    mean += P;
  }
  else
  {
    digital.set(1);
    mean -= M;
  }
}

//-------------------------------------------------------------------
void DigitalIndicator::setBrightness( BYTE b )
{
  if( b > 100 )
  {
    b = 100;
  }
  pwr = (short)b * G / 100;
}

//-------------------------------------------------------------------
void DigitalIndicator::clr( void )
{
  runUpdate = false;
  maximum   = 0;
  repeat    = false;
  t         = 0;
  //digital.clr();
  P = 0;
  M = G;
}

//-------------------------------------------------------------------
void DigitalIndicator::trigger( WORD time )
{
  runUpdate = false;
  repeat    = false;
  maximum   = ((DWORD)time*1000)/cycleTime;
  limit     = maximum;
  t         = 0;
  runUpdate = true;
}

//-------------------------------------------------------------------
void DigitalIndicator::blink( WORD time, WORD duty )
{
  runUpdate = false;
  repeat    = true;
  maximum   = ((DWORD)time*1000)/cycleTime;
  limit     = ((DWORD)MIN(duty,(WORD)100))*maximum/100;
  runUpdate = true;
}

//-------------------------------------------------------------------
void DigitalIndicator::set( void )
{
  runUpdate = false;
  repeat    = true;
  maximum   = G;
  limit     = G;
  runUpdate = true;
}

} } //namespace

//EOF
