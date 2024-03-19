//*******************************************************************
/*!
\file   NetStd.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   04.02.2024

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _NET_STD_H
#define _NET_STD_H

//*******************************************************************
#include "string.h"
#include <stdio.h>

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class NetAddr
\brief Static array for storing an IP or MAC address
*/
template <BYTE size> class NetAddr
{
  public:
    //---------------------------------------------------------------
    /*! Compare operator
        \param x Address
        \return true, if addresses are equal\n
                false otherwise
    */
    bool operator==( const NetAddr &x ) const
    {
      return( memcmp(addr, x.addr, size) == 0 );
    }

    //---------------------------------------------------------------
    /*! Assignment operator
        \param addrIn IPv4 address (normaly used by Virtual only)
    */
    void operator=( DWORD addrIn )
    {
      BYTE i=0;
      while(i < size)
      {
        addr[i] = (addrIn>>(8*i))&0xFF;
        i++;
       }
    }

    //---------------------------------------------------------------
    /*! Assignment operator
        \param addrIn String containing an address (e.g. "127.0.0.1")
    */
    void operator=( char *addrIn )
    {
      char *pos = (char*)addrIn;
      BYTE cnt = 0;

      while( *pos && cnt < size )
      {
        if( sscanf(pos,"%hhu.", &addr[cnt]) == 1 )
        {
          cnt++;
          do
          {
            pos++;
          } while( *pos != '.' && *pos != 0 );
          if( *pos == 0 )
          {
            break;
          }
          else
          {
            pos++;
          }
        }
        else
        {
          break;
        }
      }
      for( ; cnt<size; cnt ++)
      {
        addr[cnt] = 0;
      }
    }

    //---------------------------------------------------------------
    /*! Converts an address to a string
        \return String formated address
    */
    operator char *( void )
    {
      static char tmp[3*(size+1)] = "";

      strcpy( tmp, "" );
      for( BYTE i=0; i<size; i++ )
      {
        char str[5];
        sprintf( str, "%hu", addr[i] );
        strcat( tmp, str );
        if(i<size-1)
        {
          strcat( tmp, "." );
        }
      }
      return( tmp );
    }

    //---------------------------------------------------------------
    /*! Converts an address to a DWORD, normaly used by Virtual only
        \return DWORD formated address
    */
    operator DWORD( void )
    {
      return( (DWORD)(*((DWORD*)addr)) );
    }

  public:
    //-----------------------------------------------------------
    BYTE addr[size];

}; //class NetAddr

//*******************************************************************
/*!
\class NetType
\brief Template for data types with automatic host <-> network byte
       order conversion
*/
#pragma pack(1)
template <class T> class NetType
{
  public:
    //---------------------------------------------------------------
    T data;

    //---------------------------------------------------------------
    /*! Assignement operator
        \brief Converts input \b x into network byte order
        \param x Data in host byte order
    */
    void operator=( T x )
    {
      data = hton( x );
    }

    //---------------------------------------------------------------
    /*! Converts object into host byte order
        \return Data in host byte order
    */
    operator T( void )
    {
      return( ntoh(data) );
    }
};
#pragma pack(8)

//*******************************************************************
typedef BYTE           NetBYTE;  //!< A network byte oder BYTE, defined for completeness.
typedef NetType<WORD>  NetWORD;  //!< A network byteorder WORD (2 bytes).
typedef NetType<DWORD> NetDWORD; //!< A network byteorder DWORD (4 bytes).

//*******************************************************************
const NetAddr<4> zeroAddr         = {{  0,  0,  0,  0}};         //!< Zero IPv4 address, indicates an undefined address.
const NetAddr<4> broadcastAddr    = {{255,255,255,255}};         //!< Broadcast IPv4 address.
const NetAddr<6> zeroAddrPhy      = {{  0,  0,  0,  0,  0,  0}}; //!< Zero MAC address, indicates an undefined address.
const NetAddr<6> broadcastAddrPhy = {{255,255,255,255,255,255}}; //!< Broad MAC address.

}  } //namespace

#endif
