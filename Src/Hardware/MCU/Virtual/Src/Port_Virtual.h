  //*******************************************************************
/*!
\file   Port_Virtual.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   10.11.2016

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_PORT_VIRTUAL_H
#define _HW_PORT_VIRTUAL_H

//*******************************************************************
#include "Hardware/Common/Port/Port.h"
#include "Sys/UDPclient.h"
#include "Sys/UDPserver.h"
#include "Sys/UDP_Types.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class Port_Virtual

\brief Implementation of a digital I/O interface

Supports 8-Bit interfaces only
*/
class Port_Virtual : public Port
{
  private:
    //---------------------------------------------------------------
    class Data
    {
      public:
        UDP_MsgHeader  header;
        UDP_DWORD      dir;
        UDP_DWORD      value;

        Data()
        : header( UDP_MsgHeader::PORT, 0 )
        {
        }
    };

  public:
    //***************************************************************
    /*!
    \class Handler

    \brief Interface to handle a request, must be derived by server
    */
    class Handler : public UDPserver::Handler
    {
      public:
        //-----------------------------------------------------------
        Handler( UDPserver &server );

        //-----------------------------------------------------------
        // This method must be implemented by server
        //<! \todo Add servers responsibilities
        virtual MTYPE onValueRequest( MTYPE dir, MTYPE value ) = 0;

        //-----------------------------------------------------------
        virtual bool update( BYTE *data, WORD plen );

    }; //class Handler

  public:
    //---------------------------------------------------------------
    /*! Initialize hardware\n
        \param server Reference to a server
    */
    Port_Virtual( const char *server );

    //---------------------------------------------------------------
    virtual void setMode( MTYPE mask, Mode mode );

    //---------------------------------------------------------------
    virtual void setPinMode( BYTE pinId, Mode modeIn );

    //---------------------------------------------------------------
    virtual void set( MTYPE mask );

    //---------------------------------------------------------------
    virtual void set( MTYPE mask, MTYPE value );

    //---------------------------------------------------------------
    virtual void clr( MTYPE mask );

    //---------------------------------------------------------------
    virtual MTYPE get( void );

  private:
    //-------------------------------------------------------------------
    void transceive();

  private:
    //----------------------------------------
    Data       data;
    UDPclient  client;
    MTYPE      valueDir;
    MTYPE      valueOut;
    MTYPE      valueIn;

}; //class Port_Virtual

}  } //namespace

#endif
