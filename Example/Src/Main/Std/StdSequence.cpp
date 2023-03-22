//*******************************************************************
/*!
\file   StdSequence.cpp
\author Thomas Breuer
\date   13.09.2022
\brief  Sample
*/

//*******************************************************************
/*
Configuration:
          Uart

Usage:    The class TestItem is instatiated 3 times.
          itemA is added to the list within its constructor
          itemB is added during runtime
          itemC is never added

          The list is processed periodically, the result is printed each time.
*/

//*******************************************************************
#include <stdio.h>
#include "EmbSysLib.h"

//-------------------------------------------------------------------
#include "ReportHandler.h"
#include "config.h"

//*******************************************************************
class TestItem : public Sequence<TestItem>::Item
{
  public:
    //---------------------------------------------------------------
    TestItem( Sequence<TestItem> *list = 0 )
    : Item( list )
    {
      cnt = 0;
    }

    //---------------------------------------------------------------
    virtual void update(void)
    {
      cnt++;
    }

  public:
    //---------------------------------------------------------------
    WORD cnt;

};

//*******************************************************************
Sequence<TestItem> list;

TestItem itemA( &list ); // add to list
TestItem itemB;          // this item will added to the list later
TestItem itemC;

//*******************************************************************
int main(void)
{
  uart.set( "\r\n\nStdSequence," __DATE__ "," __TIME__ "\r\n\n" );
  uart.set( "running ...\r\n" );

  while( 1 )
  {
    // processing a list
    TestItem *ptr = list.getFirst();
    while( ptr != NULL )
    {
      ptr->update();
      ptr = ptr->getNext();
    }

    // print counter
    char str[40];
    sprintf( str, "A=%3d, B=%3d C=%3d\r", itemA.cnt,
                                          itemB.cnt,
                                          itemC.cnt );
    uart.set( str );

    // Add itemB to list
    if( list.add( &itemB ) )
    {
      uart.set( "\nitemB added to list\r\n" );;
    }
  }
}
