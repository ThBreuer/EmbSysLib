//*******************************************************************
/*!
\file   ReportHandler.h
\author Thomas Breuer
\date   19.11.2021
\brief  Project specific ReportHandler
\todo add comment
*/

//*******************************************************************
class myReportHandler : public EmbSysLib::Std::Report::Handler
{
  virtual void onReport( const WORD code, const WORD module, const bool isFatal )
  {
    if( isFatal )
    {
      while( 1 );
    }
    else
    {
    }
  }
} report;
