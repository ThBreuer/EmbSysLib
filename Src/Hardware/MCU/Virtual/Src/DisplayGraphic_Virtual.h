//*******************************************************************
/*!
\file   DisplayGraphic_Virtual.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   14.02.2012

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_DISPLAY_GRAPHIC_VIRTUAL_H
#define _HW_DISPLAY_GRAPHIC_VIRTUAL_H

//********************************************
#include "Hardware/Common/Display/DisplayGraphic.h"
#include "Sys/UDPclient.h"
#include "Sys/UDPserver.h"
#include "Sys/UDP_Types.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class DisplayGraphic_Virtual

\brief Virtual graphic display (Server)

\example HwDisplayGraphic.cpp
*/
class DisplayGraphic_Virtual : public DisplayGraphic
{
  private:
    //---------------------------------------------------------------
    class Data
    {
      public:
        enum
        {
          CMD_CLEAR      = 2,
          CMD_PIXEL      = 3,
          CMD_RELATIVE   = 4,
          CMD_AREA       = 5,
          CMD_FILL_AREA  = 6
        };

      public:
        UDP_MsgHeader header;
        UDP_WORD      x;
        UDP_WORD      y;
        UDP_WORD      c;
        UDP_WORD      d;
        UDP_WORD      color;

        Data()
        : header( UDP_MsgHeader::DISPLAY_GRAPHIC, 0 )
        {
        }
    };

  private:
    //---------------------------------------------------------------
    class AreaData
    {
      public:
        static const WORD maxNumOfPixel = 512;

        UDP_MsgHeader header;
        UDP_WORD      len;
        UDP_WORD      color[maxNumOfPixel];

        AreaData()
        : header( UDP_MsgHeader::DISPLAY_GRAPHIC, 0 )
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
        virtual void onClear( WORD color ) = 0;

        //-----------------------------------------------------------
        // This method must be implemented by server
        virtual void onSetPixel( WORD xPos, WORD yPos, WORD color ) = 0;

        //-----------------------------------------------------------
        // This method must be implemented by server
        virtual void onSetPixel( WORD color ) = 0;

        //-----------------------------------------------------------
        // This method must be implemented by server
        virtual void onSetArea( WORD xPos, WORD yPos, WORD width, WORD height ) = 0;

        //-----------------------------------------------------------
        // This method must be implemented by server
        virtual void onSetRectangle( WORD xPos, WORD yPos, WORD width, WORD heigth, WORD color ) = 0;

        //-----------------------------------------------------------
        virtual bool update( BYTE *data, WORD plen );

    }; //class Handler

  public:
    //---------------------------------------------------------------
    /*! Initialize display
        \param width
        \param height
        \param server Reference to a client
        \param font   Display font
        \param zoom   Display font zoom size (1,2,3,...)
    */
    DisplayGraphic_Virtual( WORD        width,
                            WORD        height,
                            const char *server,
                            Font      font,
                            BYTE        zoom = 1 );

    //---------------------------------------------------------------
    virtual void clear( void );

    //---------------------------------------------------------------
    virtual void setPixel( WORD color,
                           WORD x,
                           WORD y );

    //---------------------------------------------------------------
    virtual void setPixel( WORD color );

    //---------------------------------------------------------------
    virtual void setArea( WORD x,
                          WORD y,
                          WORD w,
                          WORD h );

    //---------------------------------------------------------------
    virtual void putRectangle( WORD x,
                               WORD y,
                               WORD w,
                               WORD h );

  private:
    //-------------------------------------------------------------------
    void transceive( BYTE cmd, WORD x, WORD y, WORD c = 0, WORD d = 0, WORD color = 0);

  private:
    //---------------------------------------------------------------
    Data        data;
    AreaData    area;
    UDPclient   client;
    DWORD       areaCount;
    DWORD       areaSize;
    WORD        areaIndex;

}; //class DisplayGraphic_Virtual

}  } //namespace

#endif
