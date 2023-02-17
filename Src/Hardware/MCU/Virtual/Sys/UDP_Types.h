//*******************************************************************
/*!
\file   UDP_Tyxpes.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   18.11.2021

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _UDP_TYPES_H
#define _UDP_TYPES_H

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
class UDP_MsgHeader
{
  public:
    static const BYTE PORT            = 0;
    static const BYTE ADC             = 1;
    static const BYTE DAC             = 2;
    static const BYTE DISPLAY_CHAR    = 3;
    static const BYTE DISPLAY_GRAPHIC = 4;
    static const BYTE TOUCH           = 5;

    BYTE id;
    BYTE instance;
    BYTE cmd;
    BYTE reserved;

    UDP_MsgHeader( BYTE id, BYTE instance )
    {
      this->id       = id;
      this->instance = instance;
      this->cmd      = 0;
      this->reserved = 0;
    }
};

//---------------------------------------------------------------
class UDP_BYTE
{
  private:
    BYTE data;

  public:
    void operator=(BYTE x)
    {
      data = x;
    }

    operator BYTE()
    {
      return(data );
    }
};

//---------------------------------------------------------------
class UDP_WORD
{
  private:
    WORD data;

  public:
    void operator=(WORD x)
    {
      data = htons( x );
    }

    operator WORD()
    {
      return( ntohs(data) );
    }
};

//---------------------------------------------------------------
class UDP_DWORD
{
  private:
    DWORD data;

  public:
    void operator=(DWORD x)
    {
      data = htonl( x );
    }

    operator DWORD()
    {
      return( ntohl(data) );
    }
};

}  } //namespace

#endif // _UDP_TYPES_H
