//*******************************************************************
/*!
\file   Adc_Virtual.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   15.10.2013

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_ADC_VIRTUAL_H
#define _HW_ADC_VIRTUAL_H

//*******************************************************************
#include "Hardware/Common/Analog/Adc.h"
#include "Sys/UDPclient.h"
#include "Sys/UDPserver.h"
#include "Sys/UDP_Types.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class Adc_Virtual

\brief Implementation of the analog to digital converter (ADC)

\example HwAdc.cpp
*/
class Adc_Virtual : public Adc
{
  private:
    //---------------------------------------------------------------
    class Data
    {
      public:
        UDP_MsgHeader header;
        BYTE          ch;
        UDP_WORD      value;

        Data()
        : header( UDP_MsgHeader::ADC, 0 )
        {
        }
    };

  public:
    //*************************************************************
    /*!
    \class Handler

    \brief Interface to handle a request, must be derived by server
    */
    class Handler : public UDPserver::Handler
    {
      public:
        //---------------------------------------------------------
        Handler( UDPserver &server );

        //---------------------------------------------------------
        // This method must be implemented by server
        virtual WORD onGetValue( BYTE ch ) = 0;

        //---------------------------------------------------------
        virtual bool update( BYTE *data, WORD plen );

    }; //class Handler

  public:
    //---------------------------------------------------------------
    /*! Initialize the ADC hardware
        \param server Name of the server
        \param timer Reference to a timer object
    */
    Adc_Virtual( const char *server,
                 Timer      &timer );

  private:
    //----------------------------------------
    virtual WORD getResult( void );

    //----------------------------------------
    virtual void startCh( BYTE ch );

    //----------------------------------------
    virtual void configCh( BYTE ch, BYTE para = 0 );

  private:
    //----------------------------------------
    Data       data;
    UDPclient  client;
    WORD       currentValue;

}; //class Adc_Virtual

}  } //namespace

#endif
