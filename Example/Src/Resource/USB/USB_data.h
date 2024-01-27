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
      sprintf(str, "cnt=%ld | interrupt: IN=%lu OUT=%lu | ctrl: IN=%lu OUT=%lu",
                  (unsigned long)cnt,
                  (unsigned long)onTransmit,
                  (unsigned long)onReceive,
                  (unsigned long)onTransmitCtrl,
                  (unsigned long)onReceiveCtrl );
      return( str );
    }

    DWORD  cnt;
    DWORD  onTransmit;
    DWORD  onTransmitCtrl;
    DWORD  onReceive;
    DWORD  onReceiveCtrl;
};
