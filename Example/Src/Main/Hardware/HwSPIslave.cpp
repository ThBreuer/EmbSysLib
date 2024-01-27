//*******************************************************************
/*!
\file   HwSPIslave.cpp
\author Thomas Breuer
\date   31.01.2013
\brief  Sample of using hardware related classes
*/

//*******************************************************************
/*
Usage:    ...
          // todo documentation
*/

//*******************************************************************
#include <stdio.h>
#include "EmbSysLib.h"

//-------------------------------------------------------------------
#include "ReportHandler.h"
#include "configHw.h"

//*******************************************************************
using namespace EmbSysLib::Hw;

//*******************************************************************
class DataHandler : public SPIslave::DataHandler
{
  public:
    //---------------------------------------------------------------
    DataHandler()
    {
    }

    //---------------------------------------------------------------
    virtual BYTE transceive( BYTE dataIn )
    {
      return( dataIn ^ 0x55 );
    }
};

//*******************************************************************
DataHandler handler;
SPIslave_0  spi( handler );

//*******************************************************************
int main(void) 
{
  uart.set( "\r\n\nHwSPIslave," __DATE__ "," __TIME__ "\r\n\n" );

  while(1)
  {
  }
}
