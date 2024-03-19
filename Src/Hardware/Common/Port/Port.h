//*******************************************************************
/*!
\file   Port.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_PORT_H
#define _HW_PORT_H

//*******************************************************************
#include "Mcu_Types.h"
#include "Std/Report.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
/*!
\class Port
\brief Abstract class supporting general purpose I/O hardware

This base class is an interface to different kinds of
digital I/O ports.

\example HwPort.cpp
*/
class Port
{
  public:
    //---------------------------------------------------------------
    /*! \enum  Mode
        \brief Define the operating modes of a GPIO port pin
    */
    typedef enum
    {
      // Direction
      In  = 0x00,         //!< Input, floating
      Out = 0x01,         //!< Output, push-pull

      // Options
      OD = 0x02,          //!< Open drain mode
      PU = 0x04,          //!< use pull up
      PD = 0x08,          //!< use pull down

      // Shortcuts
      InFL  = In,         //!< Input, floating (same as \ref In)
      InPU  = In|PU,      //!< Input, pull-up
      InPD  = In|PD,      //!< Input, pull-down
      OutPP = Out,        //!< Output, push-pull (same as \ref Out)
      OutOD = Out  |OD,   //!< Output, open drain
      OutPU = OutOD|PU,   //!< Output, open drain & pull-up
      OutPD = OutOD|PD    //!< Output, open drain & pull-down
    } Mode;

  public:
    //***************************************************************
    /*!
    \class   Pin
    \brief   Handle a single pin of a GPIO port
    \example HwPortPin.cpp
    */
    class Pin
    {
      public:
        //-----------------------------------------------------------
        /*! Instantiate an object, but don't configure the operating mode
          \param port  Reference to a Port object
          \param pinId Port pin number
        */
        Pin( Port &port, BYTE pinId );

        //-----------------------------------------------------------
        /*! Instantiate an object and configure the desired operating mode
          \param port  Reference to a Port object
          \param pinId Port pin number
          \param mode  Operating mode
        */
        Pin( Port &port, BYTE pinId, Mode mode );

        //-----------------------------------------------------------
        /*! Configure the operating mode
          \param mode New operating mode
        */
        virtual void setMode( Mode mode );

        //-----------------------------------------------------------
        /*! Set port pin to high
        */
        virtual void set( void );

        //-----------------------------------------------------------
        /*! Set port pin to value
          \param value
        */
        virtual void set( bool value );

        //-----------------------------------------------------------
        /*! Clear port pin to low
        */
        virtual void clr( void );

        //-----------------------------------------------------------
        /*! Get input value of the port pin
          \return Input value
        */
        virtual bool get( void );

      private:
        //-----------------------------------------------------------
        const BYTE  pinId;
        const MTYPE pinMask;

      private:
        //-----------------------------------------------------------
        Port  &port;

    }; //class Port::Pin
    //***************************************************************

  protected:
    //---------------------------------------------------------------
    // Standard constructor
    //
    Port( WORD moduleId );

  public:
    //---------------------------------------------------------------
    /*! Set operating mode of serveral port pins
      \param mask Defines affected port pins
      \param mode New operating mode
    */
    virtual void setMode( const MTYPE mask, Mode mode ) = 0;

    //---------------------------------------------------------------
    /*! Set operating mode of one port pin
      \param pinId Port pin number
      \param mode  Operating mode
    */
    virtual void setPinMode( const BYTE pinId, Mode mode ) = 0;

    //---------------------------------------------------------------
    /*! Set serveral port pin to high
      \param mask Defines affected port pins
    */
    virtual void set( const MTYPE mask ) = 0;

    //---------------------------------------------------------------
    /*! Set output value of serveral port pins
     \param mask  Defines affected port pins
     \param value Value in bit form, corresponding to \b mask
    */
    virtual void set( const MTYPE mask, MTYPE value ) = 0;

    //---------------------------------------------------------------
    /*! Clear several port pins to low
      \param mask Defines affected port pins
    */
    virtual void clr( const MTYPE mask ) = 0;

    //---------------------------------------------------------------
    /*! Get input value of port pins
      \return Input value of port pins in bit form
    */
    virtual MTYPE get( void ) = 0;

  public:
    //---------------------------------------------------------------
    Std::Report report;

}; //class Port

}  } //namespace

#endif
