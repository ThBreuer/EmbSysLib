//*******************************************************************
/*!
\file   NetWin.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   04.02.2024

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_NET_WIN_H
#define _HW_NET_WIN_H

//*******************************************************************
#include "Hardware/Common/Net/Net.h"
#include "Hardware/Common/Net/NetSocket.h"

#include <windows.h>
#include <winsock.h>
#include <stdio.h>

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class NetWin

\brief ...

\example ...
*/
class NetWin : public Net
{
  public:
    //***************************************************************
    /*!
    \class   Socket
    \brief   ...
    */
    class Socket : public NetSocket
    {
      public:
        //-----------------------------------------------------------
        /*! ...
        */
        Socket( NetWin &net, NetSocket::Handler *app )

        : NetSocket( app )
        , net( net )
        {
          isServerFlag = (remoteAddr == broadcastAddr);
        }

        //-----------------------------------------------------------
        /*! ...
        */
        virtual void clear( void )
        {
          net.bufIdx = 0;
          net.bufLen = 0;
        }

        //-----------------------------------------------------------
        /*! ...
        */
        virtual bool get( BYTE &x )
        {
          if( net.bufIdx < net.bufLen )
          {
            x = net.buf[net.bufIdx++];
            return( true );
          }
          return( false );
        }

        //-------------------------------------------------------------------
        virtual DataPointer get( WORD maxLen )
        {
          DataPointer dp;
          if( net.bufIdx < net.bufLen )
          {
            WORD minLen = MIN( (WORD)(net.bufLen-net.bufIdx), maxLen );
            dp = DataPointer( (BYTE*)&net.buf[net.bufIdx], minLen);
            net.bufIdx += minLen;
          }
          return( dp );
        }

        //-----------------------------------------------------------
        virtual DataPointer getDataPointer( void )
        {
          DataPointer dp( net.buf, net.bufSize );
          return( dp );
        }

      private:
        //-----------------------------------------------------------
        virtual void fillBuffer( const char c )
        {
          if( net.bufIdx >= net.bufSize )
          {
            flush();
          }
          net.buf[net.bufIdx] = c;
          net.bufIdx++;
          net.bufLen++;
        }

      protected:
        //-----------------------------------------------------------
        NetWin &net;

    }; //class NetWin::Sockt
    //***************************************************************

  public:
    //---------------------------------------------------------------
    /*!
    */
    NetWin( Timer &timer );

    //---------------------------------------------------------------
    /*!
    */
    virtual void process( void );

    //---------------------------------------------------------------
    /*!
    */
    NetAddr<4> &getIP( void );

  public:
    //---------------------------------------------------------------
    static const WORD bufSize = 2000;

    BYTE buf[bufSize];
    WORD bufLen;
    WORD bufIdx;

   private:
    //---------------------------------------------------------------
    Std::Report  report;
    WSADATA      w;

}; //class NetWin

}  } //namespace

#endif
