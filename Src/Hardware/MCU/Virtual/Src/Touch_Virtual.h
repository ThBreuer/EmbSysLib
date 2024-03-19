//*******************************************************************
/*!
\file   Touch_Virtual.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   10.03.2013

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_TOUCH_VIRTUAL_H
#define _HW_TOUCH_VIRTUAL_H

//*******************************************************************
#include "Hardware/Common/Touch/Touch.h"
#include "Sys/UDPclient.h"
#include "Sys/UDPserver.h"
#include "Sys/UDP_Types.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class Touch_Virtual

\brief Implementation of a touch sensor

\example HwTouch.cpp
*/
class Touch_Virtual : public Touch
{
  private:
    //---------------------------------------------------------------
    class Data
    {
      public:
        UDP_MsgHeader  header;
        BYTE           isTouched;
        UDP_WORD       x;
        UDP_WORD       y;

        Data()
        : header( UDP_MsgHeader::TOUCH, 0 )
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
      protected:
        //---------------------------------------------------------------
        class Data
        {
          public:
            BYTE       isTouched;
            WORD       x;
            WORD       y;
        };

      public:
        //-----------------------------------------------------------
        Handler( UDPserver &server );

        //-----------------------------------------------------------
        // This method must be implemented by server
        virtual Data onGetTouch( void ) = 0;

        //-----------------------------------------------------------
        virtual bool update( BYTE *data, WORD plen );

    }; //class Handler


  public:
    //---------------------------------------------------------------
    /*! Initialize virtual touch sensor
        \param server Reference to a client
        \param width  Touch area width
        \param height Touch area height
    */
    Touch_Virtual( const char *server,
                   WORD        width,
                   WORD        height );

  public:
    /*! Update coordinates and flags. This method must be called to
        read new data from the touch controller
    */
    virtual void update( void );

  private:
    //---------------------------------------------------------------
    Data        data;
    UDPclient   client;

}; //class Touch_Virtual;

}  } //namespace

#endif
