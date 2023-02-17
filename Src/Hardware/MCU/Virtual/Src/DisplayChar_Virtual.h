//*******************************************************************
/*!
\file   DisplayChar_Virtual.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   10.03.2011

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_DISPLAY_CHAR_VIRTUAL_H
#define _HW_DISPLAY_CHAR_VIRTUAL_H

//*******************************************************************
#include "Hardware/Common/Display/DisplayChar.h"
#include "Sys/UDPclient.h"
#include "Sys/UDPserver.h"
#include "Sys/UDP_Types.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class DisplayChar_Virtual

\brief Virtual character display (Server)

\example HwDisplayChar.cpp
*/
class DisplayChar_Virtual : public DisplayChar
{
  private:
    //---------------------------------------------------------------
    class Data
    {
      public:
        enum
        {
          CMD_CLEAR       = 0,
          CMD_WRITE       = 1
        };

      public:
        UDP_MsgHeader header;
        char          c;
        UDP_WORD      line;
        UDP_WORD      column;

        Data()
        : header( UDP_MsgHeader::DISPLAY_CHAR, 0 )
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
        virtual void onClear( void ) = 0;

        //-----------------------------------------------------------
        // This method must be implemented by server
        virtual void onWrite( WORD column, WORD line, char c ) = 0;

        //-----------------------------------------------------------
        virtual bool update( BYTE *data, WORD plen );

    }; //class Handler

  public:
    //---------------------------------------------------------------
    /*! Initialize display
        \param numOfLines
        \param numOfColumns
        \param server
    */
    DisplayChar_Virtual( WORD        numOfLines,
                         WORD        numOfColumns,
                         const char *server );

    //---------------------------------------------------------------
    virtual void clear( void );

    //---------------------------------------------------------------
    virtual void refresh( void );

    //---------------------------------------------------------------
    virtual void gotoTextPos( BYTE line,
                              BYTE column );

    //---------------------------------------------------------------
    virtual void putChar( char c );

  private:
    //---------------------------------------------------------------
    Data       data;
    UDPclient  client;
    BYTE       line;
    BYTE       colum;

}; //class DisplayChar_Virtual

}  } //namespace

#endif
