//*******************************************************************
/*!
\file   USBdevice.cpp
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"
*/

//*******************************************************************
#include "USBdevice.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
//
// USBdevice
//
//*******************************************************************
//-------------------------------------------------------------------
USBdevice::USBdevice( WORD moduleId, USBdeviceDescriptor &desc )

: desc  ( desc     ) // desc must be instantiated first
, epCtrl( *this    )
, report( moduleId )

{
}

//-------------------------------------------------------------------
void USBdevice::eventHandler( BYTE epAddr, BYTE isSetup )
{
  USBdeviceEndpoint *ptr = NULL;

  switch( epAddr )
  {
    case 0x00: // Control EP OUT
      if( isSetup )
      {
        epCtrl.setup();
      }
      else
      {
        epCtrl.out();
      }
      break;

    case 0x80: // Control-EP IN
      epCtrl.in();
      break;

    default:
      if( (ptr = desc.getEndpoint( epAddr )) != NULL )
      {
        if( epAddr & 0x80 ) // non-control EP IN
        {
          ptr->in();
        }
        else                // non-control EP OUT
        {
          ptr->out();
        }
      }
      break;
  }
}

//-------------------------------------------------------------------
void USBdevice::suspend( void )
{
  // Iteration over all configurations
  for( BYTE c = 1; (BYTE *)desc.getDescriptorConfig( c ) != NULL; c++ )
  {
    // Iteration over all interfaces, defined within a configuration
    for( WORD i = 0; (BYTE *)desc.getDescriptorInterface( c, i ) != NULL; i++ )
    {
      USBdeviceInterface *interf = desc.getInterface( c, i );
      if( interf )
        interf->onStop();
    }
  }
}

}  } //namespace

//EOF
