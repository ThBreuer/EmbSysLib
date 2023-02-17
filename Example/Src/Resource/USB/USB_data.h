static const WORD vid = 0x0023;
static const WORD pid = 0x1000;

//*******************************************************************
class __attribute__ ((__packed__)) cData
{
  public:
    cData()
    {
      cnt               = 0;
      onTransmit        = 0;
      onTransmitCtrl    = 0;
      onReceive         = 0;
      onReceiveCtrl     = 0;
    }

    char *toString()
    {
      static char str[255];
      sprintf(str, "cnt=%d | interrupt: IN=%u OUT=%u | ctrl: IN=%u OUT=%u",
                  cnt,
                  onTransmit,
                  onReceive,
                  onTransmitCtrl,
                  onReceiveCtrl );
      return( str );
    }

    int      cnt;
    unsigned onTransmit;
    unsigned onTransmitCtrl;
    unsigned onReceive;
    unsigned onReceiveCtrl;
};
