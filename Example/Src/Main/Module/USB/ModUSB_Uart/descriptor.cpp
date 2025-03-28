//###################################################################"
//
// USB Descriptor File
//
// DON'T EDIT!
//
// This file is auto generated by
//    >php -f USB_Descriptor_Script.php descriptor.txt USBdeviceDescriptor_0
//
//###################################################################

//###################################################################
#include "Hardware/Common/USB/USBdevice/USBdescriptor.h"

//###################################################################
namespace EmbSysLib {
namespace Hw {

//###################################################################
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
class USBdeviceDescriptor_0 : public USBdeviceDescriptor
{
  private:
    //-----------------------------------------------------------------
    static WORD getId( BYTE typeID,
                       BYTE confID,
                       BYTE interfID,
                       BYTE ID )
    {
      return(   ( ( ID      &0xFF )<< 0 )
               | ( ( interfID&0x0F )<< 4 )
               | ( ( confID  &0x0F )<< 8 )
               | ( ( typeID  &0x0F )<<12 ) );
    }

  public:
    //---------------------------------------------------------------
    virtual DataPointer getDescriptor( BYTE typeID,
                                       BYTE confID,
                                       BYTE interfID,
                                       BYTE ID )
    {
      switch( getId( typeID, confID, interfID, ID ) )
      {
        case 0x0000: return( DataPointer( ( BYTE* )&descriptor[  0], 18 ) ); // Device
        case 0x1100: return( DataPointer( ( BYTE* )&descriptor[ 18], 48 ) ); // Configuration
        case 0x2100: return( DataPointer( ( BYTE* )&descriptor[ 27],  9 ) ); // Interface
        case 0x4100: return( DataPointer( ( BYTE* )&descriptor[ 36],  7 ) ); // Endpoint
        case 0x2110: return( DataPointer( ( BYTE* )&descriptor[ 43],  9 ) ); // Interface
        case 0x4110: return( DataPointer( ( BYTE* )&descriptor[ 52],  7 ) ); // Endpoint
        case 0x4111: return( DataPointer( ( BYTE* )&descriptor[ 59],  7 ) ); // Endpoint
        case 0x6000: return( DataPointer( ( BYTE* )&descriptor[ 66],  4 ) ); // String
        case 0x6001: return( DataPointer( ( BYTE* )&descriptor[ 70], 14 ) ); // String
        case 0x6002: return( DataPointer( ( BYTE* )&descriptor[ 84], 20 ) ); // String
        case 0x6003: return( DataPointer( ( BYTE* )&descriptor[104], 14 ) ); // String
        case 0x6004: return( DataPointer( ( BYTE* )&descriptor[118], 10 ) ); // String
        case 0x6005: return( DataPointer( ( BYTE* )&descriptor[128], 18 ) ); // String
        case 0x6006: return( DataPointer( ( BYTE* )&descriptor[146], 18 ) ); // String
      }
      return( DataPointer(  ) );
    }

    //---------------------------------------------------------------
    virtual void registerInterface( BYTE                confId,
                                    BYTE                interfId,
                                    USBdeviceInterface *interface )
    {
      switch( getId( INTERF,confId,interfId,0 ) )
      {
        case 0x2100: interfaces[0] = interface; break;
        case 0x2110: interfaces[1] = interface; break;
      }
    }

    //---------------------------------------------------------------
    virtual USBdeviceInterface *getInterface( BYTE confId, BYTE interfId )
    {
      switch( getId( INTERF,confId,interfId,0 ) )
      {
        case 0x2100: return( interfaces[0] );
        case 0x2110: return( interfaces[1] );
      }
      return( NULL );
    }

    //---------------------------------------------------------------
    virtual void registerEndpoint( BYTE epId, USBdeviceEndpoint *ep )
    {
      switch( epId )
      {
        case 0x81: epList[0] = ep; break;
        case 0x02: epList[1] = ep; break;
        case 0x82: epList[2] = ep; break;
        default: break;
      }
    }

    //---------------------------------------------------------------
    virtual USBdeviceEndpoint *getEndpoint( BYTE epId )
    {
      switch( epId )
      {
       case 0x81: return( epList[0] );
       case 0x02: return( epList[1] );
       case 0x82: return( epList[2] );
        default: break;
      }
      return( NULL );
    }

  private:
    //---------------------------------------------------------------
    static const BYTE          descriptor[165];
    static USBdeviceInterface *interfaces[2];
    static USBdeviceEndpoint  *epList[3];
};


//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
USBdeviceInterface *USBdeviceDescriptor_0::interfaces[2] =
{
  NULL,
  NULL
};

//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
USBdeviceEndpoint *USBdeviceDescriptor_0::epList[3] =
{
  NULL,
  NULL,
  NULL
};

//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
const BYTE USBdeviceDescriptor_0::descriptor[165] =
{

  // DEVICE
    /* bLength            */ 18,
    /* bDescriptorType    */ 0x01,
    /* bcdUSB             */ (0x0200)&0xFF, (0x0200)>>8,
    /* bDeviceClass       */ Device::COMMUNICATIONS_CLASS,
    /* bDeviceSubClass    */ 0,
    /* bDeviceProtocol    */ 0,
    /* bMaxPacketSize0    */ 64,
    /* idVendor           */ (0x0025)&0xFF, (0x0025)>>8,
    /* idProduct          */ (0x3007)&0xFF, (0x3007)>>8,
    /* bcdDevice          */ (0x0100)&0xFF, (0x0100)>>8,
    /* iManufacturer      */ 0x01,
    /* iProduct           */ 0x02,
    /* iSerialNumber      */ 0x03,
    /* bNumConfigurations */ 1,

  // CONFIGURATION
    /* bLength             */ 9,
    /* bDescriptorType     */ 0x02,
    /* wTotalLength        */ (48)&0xFF, (48)>>8,
    /* bNumInterfaces      */ 2,
    /* bConfigurationValue */ 1,
    /* iConfiguration      */ 0x04,
    /* bmAttributes        */ 0x80| Configuration::SELF_POWERED,
    /* bMaxPower           */ 100,

  // INTERFACE
    /* bLength            */ 9,
    /* bDescriptorType    */ 0x04,
    /* bInterfaceNumber   */ 0,
    /* bAlternateSetting  */ 0,
    /* bNumEndpoints      */ 1,
    /* bInterfaceClass    */ Interface::CDC_COMMUNICATION,
    /* bInterfaceSubClass */ 0x02,
    /* bInterfaceProtocol */ 0,
    /* iInterface         */ 0x05,

  // ENDPOINT */ 
    /* bLength          */ 7,
    /* bDescriptorType  */ 0x05,
    /* bEndpointAddress */ 0x81,
    /* bmAttributes     */ Endpoint::INTERRUPT,
    /* wMaxPacketSize   */ (8)&0xFF, (8)>>8,
    /* bInterval        */ 10,

  // INTERFACE
    /* bLength            */ 9,
    /* bDescriptorType    */ 0x04,
    /* bInterfaceNumber   */ 1,
    /* bAlternateSetting  */ 0,
    /* bNumEndpoints      */ 2,
    /* bInterfaceClass    */ Interface::CDC_DATA,
    /* bInterfaceSubClass */ 0,
    /* bInterfaceProtocol */ 0,
    /* iInterface         */ 0x06,

  // ENDPOINT */ 
    /* bLength          */ 7,
    /* bDescriptorType  */ 0x05,
    /* bEndpointAddress */ 0x02,
    /* bmAttributes     */ Endpoint::BULK,
    /* wMaxPacketSize   */ (Mod::USB_Uart::packetSize)&0xFF, (Mod::USB_Uart::packetSize)>>8,
    /* bInterval        */ 10,

  // ENDPOINT */ 
    /* bLength          */ 7,
    /* bDescriptorType  */ 0x05,
    /* bEndpointAddress */ 0x82,
    /* bmAttributes     */ Endpoint::BULK,
    /* wMaxPacketSize   */ (Mod::USB_Uart::packetSize)&0xFF, (Mod::USB_Uart::packetSize)>>8,
    /* bInterval        */ 10,

  // STRING 0x00
    /* bLength         */ 4,
    /* bDescriptorType */ 0x03,
    (0x0409)&0xFF, (0x0409)>>8,

  // STRING 0x01
    /* bLength         */ 14,
    /* bDescriptorType */ 0x03,
    'H',0,
    '-',0,
    'B',0,
    'R',0,
    'S',0,
    ' ',0,

  // STRING 0x02
    /* bLength         */ 20,
    /* bDescriptorType */ 0x03,
    'E',0,
    'm',0,
    'b',0,
    'S',0,
    'y',0,
    's',0,
    'L',0,
    'i',0,
    'b',0,

  // STRING 0x03
    /* bLength         */ 14,
    /* bDescriptorType */ 0x03,
    'S',0,
    'N',0,
    '0',0,
    '0',0,
    '0',0,
    '0',0,

  // STRING 0x04
    /* bLength         */ 10,
    /* bDescriptorType */ 0x03,
    'D',0,
    'E',0,
    'M',0,
    'O',0,

  // STRING 0x05
    /* bLength         */ 18,
    /* bDescriptorType */ 0x03,
    'D',0,
    'e',0,
    'v',0,
    'i',0,
    'c',0,
    'e',0,
    ' ',0,
    '0',0,

  // STRING 0x06
    /* bLength         */ 18,
    /* bDescriptorType */ 0x03,
    'D',0,
    'e',0,
    'v',0,
    'i',0,
    'c',0,
    'e',0,
    ' ',0,
    '0',0,

    0 // final byte
};

//*******************************************************************
}  } //namespace
//EOF
