//*******************************************************************
/*!
\file   ReportID_Mod.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   21.02.2022

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _MOD_REPORT_ID_H
#define _MOD_REPORT_ID_H

//*******************************************************************
namespace EmbSysLib {
namespace Mod {

//*******************************************************************
/*!
\class ReportID_Mod
\brief Definition of Module IDs and Error-Codes used in "Module"

\see Std::Report
*/
class ReportID_Mod
{
  public:
    //---------------------------------------------------------------
    /*!
    \class Module
    \brief Definition of module ID
    */
    class Module
    {
      public:
        //-----------------------------------------------------------
        static const WORD  USB_Uart = 0x2010; //!< USB in virtual com port mode
    };

  public:
    //---------------------------------------------------------------
    /*!
    \class Event
    \brief Definition of event ID
    */
    class Event
    {
      public:
        //-----------------------------------------------------------
    };

}; //class _ReportID_Mod

}  } //namespace

#endif
