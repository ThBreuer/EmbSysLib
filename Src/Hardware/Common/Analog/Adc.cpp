//*******************************************************************
/*!
\file   Adc.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"
*/

//*******************************************************************
#include "Adc.h"
#include "Hardware/MCU/System.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// Adc
//
//*******************************************************************
//-------------------------------------------------------------------
Adc::Adc( WORD moduleId, BYTE num, Timer &timer )
: report( moduleId ) 
{
  currentCh      = 0;
  currentId      = 0;
  numActivatedCh = 0;
  numOfChannels  = num;
  result         = new WORD[num];
  chList         = new BYTE[num];
  isRunning      = false;

  if( !result || !chList )
  {
    report.error( ReportID_Hw::Event::MEMORY_ALLOCTION_ERROR );
  }

  timer.add( this ); // add to update list
}

//-------------------------------------------------------------------
void Adc::enable( BYTE ch, BYTE para )
{
  if( ch >= numOfChannels )  // is requested channel supported?
  {
    report.error( ReportID_Hw::Event::WRONG_CHANNEL );
    return;
  }

  for( int i = 0; i < numActivatedCh; i++ )  // check, if ch in chList
  {
    if( chList[i]  == ch )
    {
      configCh( ch, para );
      return;
    }
  }

  if( numActivatedCh < numOfChannels )  // add ch to chList if possible
  {
    result[ch] = 0;
    chList[numActivatedCh++] = ch;
    configCh( ch, para );               // call hardware configuration
  }
}

//-------------------------------------------------------------------
inline WORD Adc::get( BYTE ch )
{
  return( result[ch] ); // get result of this ch
}

//-------------------------------------------------------------------
inline void Adc::update( void )
{
  if( numActivatedCh && !isRunning ) // while sampling is running, we
                                     // must not update. Otherwise it
                                     // stops actual scan
  {
    currentId = 0;                   // start with first one
    currentCh = chList[currentId];   // get channel from list
    isRunning = true;                // set flag ...
    startCh( currentCh );            // ... and start with sampling
  }
}

//-------------------------------------------------------------------
inline void Adc::isr( void )
{
  result[currentCh] = getResult(); // get Result from HW and
                                   // store it in result buffer
  currentId++;                     // next channel

  if( currentId < numActivatedCh ) // is there a next one?
  {
    currentCh = chList[currentId]; // get next ch from list
    startCh( currentCh );          // start with sampling
  }
  else                             // ready, all channels are sampled
  {
    currentId = 0;
    currentCh = chList[currentId];
    isRunning = false;
  }
}

} } //namespace

//EOF
