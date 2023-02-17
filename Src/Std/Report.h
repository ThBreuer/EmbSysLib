//*******************************************************************
/*!
\file   Report.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   10.09.2020

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _Report_H
#define _Report_H

//*******************************************************************
#include "Mcu_Types.h"

//*******************************************************************
namespace EmbSysLib {
namespace Std {

//*******************************************************************
/*!
\class Report

\brief Store and report an error event

This class can be used to store and report an error event.
By calling the method error() or alert():
- the code and module ID are stored in static attributes
  (first event on global scope only)
- the code is stored in the Report object (first event on object scope only)
- the method Handler::onReport() is called

Derive the subclass Handler to define an own Handler::onReport() method.

This class is only intended to report \b exceptional errors.

\todo Klären, ob Code in dieser Klasse gespeichert werden muss (global bzw. local).
      Wenn ja: dann ersten oder letzten Code? Nur errors (nicht alert) global speichern?
      Könnte der Handler das nicht besser?

\example StdReport.cpp
*/
class Report
{
  public:
    //*******************************************************************
    /*!
    \class Handler

    \brief Handle a Report
    */
    class Handler
    {
      public:
        //---------------------------------------------------------------
        /*! Instantiate an object to handle the report

            The object will be registered automatically
        */
        Handler();

        //---------------------------------------------------------------
        /*! Destructor, dereference the registered object
        */
        ~Handler();

        //---------------------------------------------------------------
        /*! The onReport() method is called by any Report object.
            \param code Error code
            \param module Module ID
            \param isFatal true, if Report::error() is called\n
                           false, if Report::alert() is called
        */
        virtual void onReport( const WORD code,
                               const WORD module,
                               const bool isFatal ) = 0;

    }; // Report::Handler

  public:
    //---------------------------------------------------------------
    /*! Standard constructor
        \param moduleId
    */
    Report( WORD moduleId );

    //---------------------------------------------------------------
    /*! Store an event, but don't call the onReport() method
        \param code
        \todo private only?
    */
    void store( const WORD code );

    //---------------------------------------------------------------
    /*! Store an event and call onReport().
        If no handler is registered, a reset is triggered.
        \param code
    */
    void error( const WORD code );

    //---------------------------------------------------------------
    /*! Store an event and call onReport().
        \param code
    */
    void alert( const WORD code );

    //---------------------------------------------------------------
    /*! Return the code of the first event that occured
        \return Error code
    */
    WORD getCode( void );

    //---------------------------------------------------------------
    /*! Return the code of the first event that occured
        and reset code
        \return Error code
    */
    WORD getCodeEvent( void );

    //---------------------------------------------------------------
    /*! Return own moduleId
        \return Error code
    */
    WORD getModuleId( void );

  public:
    //---------------------------------------------------------------
    static WORD globalCode;    //!< Code of the first reported event (global)
    static WORD globalModule;  //!< Module ID of the first reported event (global)

  private:
    //---------------------------------------------------------------
    WORD localCode;
    WORD moduleId;

    static Report::Handler *msgHandler;

}; //Report

}  } //namespace

#endif
