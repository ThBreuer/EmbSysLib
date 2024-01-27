//*******************************************************************
/*!
\file   Isc.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   03.01.2024

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _MOD_ISC_H
#define _MOD_ISC_H

//*******************************************************************
#include "Std/CRC.h"

//*******************************************************************
namespace EmbSysLib {
namespace Mod {

using namespace Std;

//*******************************************************************
/*!
\class Isc

\brief Inter-Systems-Communication (Isc), base class

This class is the base class for Isc objects. Derived
classes must provide a communication port to transport Isc data objects.
A 16-bit CRC is used.

To create an Isc data object:
- Define an application specific class with POD subclass named \b
  'Data'
- This class may have an \b update() method, which is called on
  data receive
- Instantiate an object from the template class \b Isc::Data.
  The first template parameter is the name of the aplication
  specific data class.
  The second parameter must be an unique ID to identify data objects

\example ModIsc.cpp
*/
class Isc
{
  protected:
    //---------------------------------------------------------------
    class DataInterface : protected Sequence<DataInterface>::Item
    {
      protected:
        //-----------------------------------------------------------
        DataInterface( Isc &isc )
        : isc( isc )
        {
        }

        //-----------------------------------------------------------
        virtual void update( void ) = 0;

      public:
        //-----------------------------------------------------------
        BYTE *dataRef;
        WORD  dataLength;
        BYTE  id;

      protected:
        //-----------------------------------------------------------
        Isc        &isc;
        Flag<bool>  flag;

      public:
        //-----------------------------------------------------------
        /*! Write an object

            The object is written with the Isc transport object
            defined by Constructor
            \return
                    - true, if data are sentry
                    - false otherwise
        */
        bool write( void )
        {
          return( isc.write( id, dataRef, dataLength ) );
        }

        //-----------------------------------------------------------
        /*! Ceck, if new data are received
            \return
                    - true, if new data are received
                    - false otherwise
        */
        bool isNew( void )
        {
          return( flag.getEvent() );
        }

      friend class Isc;
      friend class Sequence<DataInterface>;

    }; //class Isc::DataInterface
    //---------------------------------------------------------------

  public:
    //---------------------------------------------------------------
    /*! Template class for Isc data objects
        \param T Used data class
        \param ID Unique identifier
    */
    template <class T, BYTE ID> class Data : public DataInterface
    {
      public:
        //-----------------------------------------------------------
        /*! Instantiate an Isc data object
            \param IscIn Reference to an Isc. The instantiated object
                         is bound to this Isc transport object
        */
        Data( Isc &isc )
        : DataInterface( isc )
        , data( prop.data )
        {
          dataRef    = (BYTE*)&prop.data;
          dataLength = sizeof( typename T::Data );
        //  if( isc.getMaxLength() < dataLength )
        //  {
        //    // report fatal error
        //  }
          id         = ID;
          flag       = false;
          isc.list.add( this );
        }

      protected:
        //-----------------------------------------------------------
        virtual void update( void )
        {
          flag = true;
          prop.onReceive();
        }

      public:
        //-----------------------------------------------------------
        typename T::Data &data;
        T                 prop;

    }; //class Isc::Data
    //---------------------------------------------------------------

  protected:
    //---------------------------------------------------------------
    class Header
    {
      private:
        BYTE crcLow;
        BYTE crcHigh;
        BYTE id;
        BYTE reserved = 0;

      friend class Isc;
    }; //class Isc::Header
    //---------------------------------------------------------------

  protected:
    //---------------------------------------------------------------
    Isc( Crc::MODE crcMode )
    : crc( crcMode )
    {
    }

  public:
    //---------------------------------------------------------------
    /*! Check, if underlaying communication port is connected
        \return
                - true, if connected
                - false otherwise
    */
    virtual bool isConnected() = 0;

    //---------------------------------------------------------------
    /*! Receive and process incomming data. This method must be
        called periodically by the application
    */
    virtual void update( void ) = 0;

  protected:
    //---------------------------------------------------------------
    // todo How to handle output error (device error, buffer/fifo full etc)?
    virtual bool write( BYTE id, BYTE *data, BYTE size )
    {
      Header h;

      crc.reset();
      crc(id);
      crc(0); // reserved
      crc( data, size );
      h.id = id;
      h.crcLow  = ( crc.get()      ) & 0xFF;
      h.crcHigh = ( crc.get() >> 8 ) & 0xFF;
      return( writeStream( h, data, size ) );
    }

    //---------------------------------------------------------------
    virtual bool process( Header &header, BYTE *data, WORD size )
    {
      // look up correct data object: identifier and length OK?
      DataInterface *ptr = (DataInterface*)list.getFirst();

      while( ptr && ptr->id != header.id )
      {
        ptr = (DataInterface*)ptr->getNext();
      }

      if( ptr && size >= ptr->dataLength ) // todo get transfered size, then check size == ptr->dataLength
      {
        crc.reset();
        crc( header.id );
        crc(  0 );// reserved
        crc( data, ptr->dataLength );
        if( crc.get() == (header.crcLow | (header.crcHigh<<8)) )
        {
          memcpy( ptr->dataRef, data, ptr->dataLength );
          ptr->update(); // we found a matching data object
          return( true );
        }
      }
      return( false );
     }

    protected:
      //-------------------------------------------------------------
      virtual bool writeStream( Header &header, BYTE *data, WORD size ) = 0;

  protected:
    //---------------------------------------------------------------
    Sequence<DataInterface>  list;
    Crc                      crc;

}; //class Isc

} } //namespace

#endif
