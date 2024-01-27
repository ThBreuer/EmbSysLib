//*******************************************************************
/*!
\file   Isc_USBhost.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   03.01.2024

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _MOD_ISC_USB_HOST_H
#define _MOD_ISC_USB_HOST_H

//*******************************************************************
#include "Module/Isc/Isc.h"
#include "Hardware/Common/USB/USBhost.h"

//*******************************************************************
namespace EmbSysLib {
namespace Mod {

//*******************************************************************
/*!
\class Isc_USBhost

\brief ...

*/
template <WORD maxLength> 
class Isc_USBhost : public Isc
{
  protected:
    //---------------------------------------------------------------
    #pragma pack(1)
    class DATA
    {
      public:
        Header header;
        BYTE data[ maxLength ];
    };
    #pragma pack()

  public:
    //---------------------------------------------------------------
    /*! ...
    */
    Isc_USBhost( USBhost &usb, BYTE interfId, Crc::MODE crcMode = Crc::FAST)

    : Isc( crcMode )
    , usb( usb )
    {
    }

    //---------------------------------------------------------------
    /*! ...
    */
    virtual bool isConnected()
    {
      return( true );
    }

    //---------------------------------------------------------------
    /*! ...
    */
    virtual void update( void )
    {
      DATA rec;

      if( usb.readInterrupt( rec )<0 )
      {
        return;
      }
      process( rec.header, rec.data, maxLength /*transfered - sizeof(Header)*/ );
      // todo use transfered(!) buffer size, not maxLength
      return; //( true );
    }

  protected:
    //---------------------------------------------------------------
    /*! ...
    */
    virtual bool writeStream( Header &header, BYTE *data, WORD size )
    {
      DATA tmp;

      tmp.header = header;
      memcpy( tmp.data, data, size );

      return( usb.writeInterrupt( (BYTE*)&tmp, size+sizeof(Header)) );
    }

    public:
    //---------------------------------------------------------------
    USBhost &usb;
    DATA     rec;

}; //Isc_USBhost

} } //namespace

#endif
