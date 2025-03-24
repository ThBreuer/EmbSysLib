//*******************************************************************
/*!
\file   NetSocket.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   04.02.2024

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_NET_SOCKET_H
#define _HW_NET_SOCKET_H

//*******************************************************************
#include "NetStd.h"
#include "Net.h"
#include "Std/Std.h"
#include "Std/DataPointer.h"
#include "Std/Flag.h"

#include <stdarg.h>

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class NetSocket
\brief Socket to handle IP based protocolls like TCp and UDP

\example HwNet_TCP.cpp, HwNet_UDP.cpp
*/
class NetSocket : protected Net::Task
{
  public:
    //-----------------------------------------------------------
    /*! \enum State
        \brief Socket state
    */
    typedef enum
    {
      UNDEFINED = 0, //!< State undefined
      CLOSED,        //!< Socket closed
      LISTENING,     //!< Socket is listening (as server)
      CONNECTED,     //!< Socket is connected
      ERROR_STATE    //!< Socket error
    } State;

    //-----------------------------------------------------------
    /*! \enum Event
        \brief ...
    */
    typedef enum
    {
      SOCKET_STATE = 0, //!< Socket state changed
      LINK_STATE,       //!< Link (e.g. ethernet) state changed
    } Event;

  public:
    //***************************************************************
    /*!
    \class   Handler
    \brief   Base class (interface) for classes using a NetSocket.
    \example HwNet_TCP.cpp, HwNet_UDP.cpp
    */
    class Handler
    {
      public:
        //-----------------------------------------------------------
        /*! Method called by NetSocket, if a data package is received.
            The application has to override this method.
        */
        virtual void onReceive( NetSocket &socket ) = 0;

        //-----------------------------------------------------------
        /*! Method called by NetSocket, if a socket state is changed.
            The application has to override this method.
        */
        virtual void onEvent( NetSocket &socket, Event event, WORD param ) = 0;

    }; //class NetSocket::Handler

  public:
    //---------------------------------------------------------------
    /*! Initialize a NetSocket
        \param app A Handler object
    */
    NetSocket( Handler *app )
    {
      add( app );
      remoteAddr   = zeroAddr;
      remotePort   = 0;
      isServerFlag = false;
      state        = UNDEFINED;
    }

    //---------------------------------------------------------------
    /*! Set the Handler

        The NetSocket supports only one Handler object
        \param handler Pointer to an Handler object, could be NULL
    */
    void add( Handler *handler )
    {
      app = handler;
    }

    //---------------------------------------------------------------
    /*! Returns the address of the last connected station
        \return Station's IPv4 address or zeroAddr
    */
    virtual NetAddr<4> getRemoteAddr( void )
    {
      return( remoteAddr );
    }

    //---------------------------------------------------------------
    /*! Returns the port of the last connected station
        \return Station's port
    */
    virtual WORD getRemotePort( void )
    {
      return( (WORD)remotePort );
    }

    //---------------------------------------------------------------
    /*! Open a connection as client
        \param addr IPv4 address of remote station
        \param port Port of remote station
    */
    virtual void open( NetAddr<4> addr, WORD port ) = 0;

    //---------------------------------------------------------------
    /*! Close a connection
    */
    virtual void close( void ) = 0;

    //---------------------------------------------------------------
    /*! ...
    */
    virtual State getState( void )
    {
      return( state.get() );
    }

    //---------------------------------------------------------------
    /*! Clear I/O buffer

        Must be called \b after reading incomming data and
        \b before writing outgoing data
    */
    virtual void clear( void ) = 0;

  public:
    //---------------------------------------------------------------
    /*! Write a single byte
    */
    virtual void write( const char c )
    {
      fillBuffer( c );
    }

    //---------------------------------------------------------------
    /*! Write a string
    */
    virtual void write( const char *str )
    {
      write( str, strlen(str) );
    }

    //---------------------------------------------------------------
    /*! Write a buffer
    */
    virtual void write( const char *buf, WORD len )
    {
      for( WORD i = 0; i < len; i++ )
      {
        write( buf[i] );
      }
    }

    //---------------------------------------------------------------
    /*! Write a formated text string

        This method corresponds to the ANSI-C function of the same name
        \param format Formated string to print
    */
    template <WORD len = Std::DEFAUT_STRING_LEN>
    void printf( const char *format, ... )
    {
      char strOut[len+1];
      va_list argzeiger;

      va_start ( argzeiger, format );
      vsnprintf( strOut, len, format, argzeiger );
      va_end   ( argzeiger );
      strOut[len] = 0;
      // Set output
      write( strOut );
    }

    //---------------------------------------------------------------
    /*! Transmit the outgoing data buffer

        Must be called after writing has been completed
    */
    virtual void flush( void ) = 0;

    //---------------------------------------------------------------
    /*! Read a byte from
    */
    virtual bool get( BYTE &x ) = 0;

    //---------------------------------------------------------------
    /*! todo comment
        \return ...
    */
    virtual DataPointer get( WORD maxLen ) = 0;

    //---------------------------------------------------------------
    /*! Get the DataPointer of the outgoing data buffer
        \return DataPointer
    */
    virtual DataPointer getDataPointer( void ) = 0;

  protected:
    //---------------------------------------------------------------
    virtual bool onProcess( void )= 0;

    //---------------------------------------------------------------
    virtual void onTrigger( void ) = 0;

  private:
    //---------------------------------------------------------------
    virtual void fillBuffer( const char c ) = 0;

  protected:
    //---------------------------------------------------------------
    NetAddr<4>  remoteAddr;
    WORD        remotePort;
    bool        isServerFlag;
    Handler    *app;

  protected:
    //---------------------------------------------------------------
    Std::Flag<State> state; //!< Connection state

}; //class NetSocket

}  } //namespace

#endif
