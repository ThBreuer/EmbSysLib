//*******************************************************************
/*!
\file   ReportHandler.h
\author Thomas Breuer
\date   19.11.2021
\brief  Project specific ReportHandler
\todo add comment
*/

//*******************************************************************
#include <windows.h>

//*******************************************************************
using namespace EmbSysLib::Hw;

//*******************************************************************
class myReportHandler : public EmbSysLib::Std::Report::Handler
{
  void printMessage( const WORD code,const WORD module )
  {
    switch( module )
    {
      case ReportID_Hw::Module::SYSTEM_WSA:
           fprintf(stderr, "\n\rWSA-Error, VirtualServer not running?\n" );
      break;

     case ReportID_Hw::Module::UART_SERIAL:
       fprintf(stderr, "\n\rWin-Error (Uart_Serial): %u (0x%04X)\n",code,code);
       break;

     case ReportID_Hw::Module::USB_HOST_MCU:
       fprintf(stderr, "\n\rUSB-Error: Device not found (0x%04X)\n",code);
       break;

     case ReportID_Hw::Module::SYSTEM_LIB_USB:
       fprintf(stderr, "\n\rlibusb-Error: %u (0x%04X)\n",code,code);
       fprintf(stderr, "\n\r%s\n",libusb_error_name(-((int)code)) );
       fprintf(stderr, "\n\r%s\n",libusb_strerror(-((int)code)) );
       break;
    }
  }

  virtual void onReport(const WORD code,const WORD module,const bool isFatal)
  {
    // Set output mode to handle virtual terminal sequences
    HANDLE hOut = GetStdHandle(STD_ERROR_HANDLE);

    if( isFatal )
    {
      SetConsoleTextAttribute(hOut, 12); // red text color
      fprintf(stderr, "\n\rError: Module=0x%04X, Code=0x%04X(%d)\n",module,code,code);
      printMessage( code, module );
      SetConsoleTextAttribute(hOut, 7 ); // reset to white text color
      fprintf(stderr, "press key ...\n\r" );
      getchar();
      System::reset();
    }
    else
    {
      SetConsoleTextAttribute(hOut, 14 ); // yellow text color
      fprintf(stderr, "\n\rAlert: Module=0x%04X, Code=0x%04X(%d)\n",module,code,code);
      printMessage( code, module );
      SetConsoleTextAttribute(hOut, 7 ); // reset to white text color
    }
  }
} report;
