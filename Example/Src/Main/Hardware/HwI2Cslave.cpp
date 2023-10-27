//*******************************************************************
/*!
\file   HwI2Cslave.cpp
\author Thomas Breuer
\date   31.01.2013
\brief  Sample of using hardware related classes
*/

/*******************************************************************
/*
Usage:    ...
*/

//*******************************************************************
#include <stdio.h>
#include "EmbSysLib.h"

//-------------------------------------------------------------------
#include "ReportHandler.h"
#include "configHw.h"

//-------------------------------------------------------------------
#define HWADDR 0xB0   // I2C address for I2Cmaster/-slave test

//*******************************************************************
using namespace EmbSysLib::Hw;

//*******************************************************************
class DataHandler : public I2Cslave::DataHandler
{
  public:
    //---------------------------------------------------------------
    virtual void receive(BYTE dataIn, BYTE id)
    {
      if(id==0)
        adr = dataIn;
      else
        data[(adr+id-1)%256] = dataIn;
    }

    //---------------------------------------------------------------
    virtual BYTE transmit(BYTE id)
    {
      return(data[(adr+id)%256]);
    }

    //---------------------------------------------------------------
    BYTE data[256];
    BYTE adr;
};

//*******************************************************************
DataHandler handler;
I2Cslave_0  i2c( handler, HWADDR );

//*******************************************************************
int main( void )
{
  uart.set( "\r\n\nHwI2Cslave," __DATE__ "," __TIME__ "\r\n\n" );

  while( 1 )
  {
  }
}
