//*******************************************************************
/*!
\file   Dac_Virtual.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   21.01.2013

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_DAC_VIRTUAL_H
#define _HW_DAC_VIRTUAL_H

//*******************************************************************
#include "Hardware/Common/Analog/Dac.h"
#include "Sys/UDPclient.h"
#include "Sys/UDPserver.h"
#include "Sys/UDP_Types.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class Dac_Virtual

\brief Implementation of the digital to analog converter (DAC)

\example HwDac.cpp
*/
class Dac_Virtual : public Dac
{
  private:
    //***************************************************************
    class Data
    {
      public:
        UDP_MsgHeader header;
        BYTE          ch;
        UDP_WORD      value;

        Data()
        : header( UDP_MsgHeader::DAC, 0 )
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
        virtual void onSetValue( BYTE ch, WORD value ) = 0;

        //-----------------------------------------------------------
        virtual bool update( BYTE *data, WORD plen );

    }; //class Handler

  public:
    //---------------------------------------------------------------
    /*! Initialize the DAC hardware
        \param server Reference to a client
    */
    Dac_Virtual( const char *server );

  public:
    //---------------------------------------------------------------
    virtual void set( BYTE ch,
                      WORD value );

    //---------------------------------------------------------------
    virtual void enable( BYTE ch ) { /* nothing to do */ }

  private:
    //---------------------------------------------------------------
    Data        data;
    UDPclient   client;

}; //class Dac_Virtual

}  } //namespace

#endif
