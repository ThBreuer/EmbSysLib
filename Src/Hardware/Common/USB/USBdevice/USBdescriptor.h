//*******************************************************************
/*!
\file   USBdescriptor.h
\author Thomas Breuer (Bonn-Rhein-Sieg University of Applied Sciences)
\date   23.03.2016

License: See file "LICENSE"
*/

//*******************************************************************
#ifndef _HW_USB_DEVICE_DESCRIPTOR_H
#define _HW_USB_DEVICE_DESCRIPTOR_H

//*******************************************************************
#include "Mcu_Types.h"
#include "Std/DataPointer.h"

//*******************************************************************
namespace EmbSysLib {
namespace Hw {

//*******************************************************************
class USBdeviceInterface;
class USBdeviceEndpoint;

//*******************************************************************
/*!
\class USBdeviceDescriptor

\brief Abstract base class for USB descriptor
*/
#pragma pack(1)
class USBdeviceDescriptor
{
  public:
    //---------------------------------------------------------------
    // ID of requested descriptor type
    enum
    {
      DEVICE_DESCRIPTOR                  =  1,
      CONFIGURATION_DESCRIPTOR           =  2,
      STRING_DESCRIPTOR                  =  3,
      INTERFACE_DESCRIPTOR               =  4,
      ENDPOINT_DESCRIPTOR                =  5,
      DEVICE_QUALIFIER_DESCRIPTOR        =  6,
      OTHER_SPEED_CONFIG_DESCRIPTOR      =  7,
      INTERFACE_POWER_DESCRIPTOR         =  8,
      OTG_DESCRIPTOR                     =  9,
      DEBUG_DESCRIPTOR                   = 10,
      INTERFACE_ASSOCIATION_DESCRIPTOR   = 11
    };

  public:
    //---------------------------------------------------------------
    // Device descriptor
    //
    class Device
    {
      public:
        //-----------------------------------------------------------
        // DeviceClassType
        //
        enum
        {
          RESERVED_CLASS              = 0,
          AUDIO_CLASS                 = 1,
          COMMUNICATIONS_CLASS        = 2,
          HUMAN_INTERFACE_DEVICE      = 3,
          MONITOR_CLASS               = 4,
          PHYSICAL_INTERFACE_CLASS    = 5,
          POWER_CLASS                 = 6,
          PRINTER_CLASS               = 7,
          STORAGE_CLASS               = 8,
          HUB_CLASS                   = 9,
          MISCELLANEOUS               = 0xEF,
          VENDOR_SPECIFIC_CLASS       = 0xFF
        };

      public:
        enum
        {
          // Bits returned by getConfigDeviceStatus
          SELF_POWERED_MASK  = 0x01,
          REMOTE_WAKEUP_MASK = 0x02
        };

      public:
        BYTE  bLength;
        BYTE  bDescriptorType;
        WORD  bcdUSB;
        BYTE  bDeviceClass;
        BYTE  bDeviceSubClass;
        BYTE  bDeviceProtocol;
        BYTE  bMaxPacketSize0;
        WORD  idVendor;
        WORD  idProduct;
        WORD  bcdDevice;
        BYTE  iManufacturer;
        BYTE  iProduct;
        BYTE  iSerialNumber;
        BYTE  bNumConfigurations;
    };

  public:
    //---------------------------------------------------------------
    // Configuration descriptor
    //
    class Configuration
    {
      public:
        //-----------------------------------------------------------
        // ConfigurationId
        //
        enum
        {
          // Bits in field bmAttributes of Configuration Descriptor
          SELF_POWERED   = 0x40,
          REMOTE_WAKEUP  = 0x20,
        };

      public:
        BYTE  bLength;
        BYTE  bDescriptorType;
        WORD  wTotalLength;
        BYTE  bNumInterfaces;
        BYTE  bConfigurationValue;
        BYTE  iConfiguration;
        BYTE  bmAttributes;
        BYTE  bMaxPower;
    };

  public:
    //---------------------------------------------------------------
    // Interface descriptor
    //
    class Interface
    {
      public:
        //---------------------------------------------------------------
        // DeviceClassType
        //
        enum
        {
          CDC_COMMUNICATION           = 0x02,
          CDC_DATA                    = 0x0A,
          HUMAN_INTERFACE_DEVICE      = 0x03,
          VENDOR_SPECIFIC_CLASS       = 0xFF
        };

      protected:
        BYTE  bLength;
        BYTE  bDescriptorType;
        BYTE  bInterfaceNumber;
        BYTE  bAlternateSetting;
        BYTE  bNumEndpoints;
        BYTE  bInterfaceClass;
        BYTE  bInterfaceSubClass;
        BYTE  bInterfaceProtocol;
        BYTE  iInterface;
    };

  public:
    //---------------------------------------------------------------
    // Endpoint descriptor
    //
    class Endpoint
    {
      public:
        //-----------------------------------------------------------
        // EndpointType
        //
        enum
        {
          CONTROL     = 0,
          ISOCHRONOUS = 1,
          BULK        = 2,
          INTERRUPT   = 3
        };

      public:
        BYTE  bLength;
        BYTE  bDescriptorType;
        BYTE  bEndpointAddress;
        BYTE  bmAttributes;
        WORD  wMaxPacketSize;
        BYTE  bInterval;
    };

  public:
    //---------------------------------------------------------------
    // HID descriptor
    //
    class HID
    {
      public:
        //-----------------------------------------------------------
        // Country
        //
        class Country
        {
          public:
            //-------------------------------------------------------
            // CountryId
            //
            enum
            {
              NOT_SUPPORTED    = 0,
              GERMAN           = 9,
              INTERNATIONAL    = 13
            };
        };

      protected:
        BYTE  bLength;
        BYTE  bDescriptorType;
        BYTE  bcdHID;
        BYTE  bCountryCode;
        BYTE  bNumDescriptors;
    };


  public:
    //---------------------------------------------------------------
    // Report descriptor
    // \see "Universal Serial Bus HID Usage Tables",
    //      \n https://www.usb.org/document-library/hid-usage-tables-122
    //
    class Report
    {
      public:
        //-----------------------------------------------------------
        class UsagePage
        {
          public:
            //-------------------------------------------------------
            class GenericDesktop
            {
              public:
                static const BYTE  PAGE_ID = 0x01;

                //---------------------------------------------------
                // Usage_Id
                //
                enum
                {
                  UNDEFINED               = 0x00,
                  POINTER                 = 0x01,
                  MOUSE                   = 0x02,
                  JOYSTICK                = 0x04,
                  GAME_PAD                = 0x05,
                  KEYBOARD                = 0x06,
                  KEYPAD                  = 0x07,
                  MULTI_AXIS              = 0x08,
                  TABLET_PC               = 0x09,
                  X                       = 0x30,
                  Y                       = 0x31,
                  Z                       = 0x32,
                  RX                      = 0x33,
                  RY                      = 0x34,
                  RZ                      = 0x35,
                  SLIDER                  = 0x36,
                  DIAL                    = 0x37,
                  WHEEL                   = 0x38,
                  HAT_SWITCH              = 0x39,
                  COUNTED_BUFFER          = 0x3A,
                  BYTE_COUNT              = 0x3B,
                  MOTION_WAKEUP           = 0x3C,
                  START                   = 0x3D,
                  SELECT                  = 0x3E,
                  VX                      = 0x40,
                  VY                      = 0x41,
                  VZ                      = 0x42,
                  VBRX                    = 0x43,
                  VBRY                    = 0x44,
                  VBRZ                    = 0x45,
                  VNO                     = 0x46,
                  FEATURE_NOTIFICATION    = 0x47,
                  RESOLUTION_MULTIPLIER   = 0x48,
                  SYSTEM_CTL              = 0x80,
                  SYSCTL_POWER            = 0x81,
                  SYSCTL_SLEEP            = 0x82,
                  SYSCTL_WAKE             = 0x83,
                  SYSCTL_CONTEXT_MENU     = 0x84,
                  SYSCTL_MAIN_MENU        = 0x85,
                  SYSCTL_APP_MENU         = 0x86,
                  SYSCTL_HELP_MENU        = 0x87,
                  SYSCTL_MENU_EXIT        = 0x88,
                  SYSCTL_MENU_SELECT      = 0x89,
                  SYSCTL_MENU_RIGHT       = 0x8A,
                  SYSCTL_MENU_LEFT        = 0x8B,
                  SYSCTL_MENU_UP          = 0x8C,
                  SYSCTL_MENU_DOWN        = 0x8D,
                  COLD_RESTART            = 0x8E,
                  WARM_RESTART            = 0x8F,
                  D_PAD_UP                = 0x90,
                  D_PAD_DOWN              = 0x91,
                  D_PAD_RIGHT             = 0x92,
                  D_PAD_LEFT              = 0x93,
                  SYSTEM_DOCK             = 0xA0,
                  SYSTEM_UNDOCK           = 0xA1,
                  SYSTEM_SETUP            = 0xA2,
                  SYSTEM_BREAK            = 0xA3,
                  SYSTEM_DEGUGGER_BREAK   = 0xA4,
                  APPLICATION_BREAK       = 0xA5,
                  APPLICATION_DEBUG_BREAK = 0xA6,
                  SYSTEM_SPEAKER_MUTE     = 0xA7,
                  SYSTEM_HIBERNATE        = 0xA8,
                  SYSTEM_DISPLAY_INNVERT  = 0xB0,
                  SYSTEM_DISPLAY_INTERNAL = 0xB1,
                  SYSTEM_DISPLAY_EXTERNAL = 0xB2,
                  SYSTEM_DISPLAY_BOTH     = 0xB3,
                  SYSTEM_DISPLAY_DUAL     = 0xB4,
                  SYSTEM_DISPLAY_TOGGLE   = 0xB5 
                };
              }; //GenericDesktop

            //-------------------------------------------------------
            class KeyCodes
            {
              public:
                static const BYTE PAGE_ID  = 0x07;
            };

            //-------------------------------------------------------
            class Consumer
            {
              public:
                static const BYTE PAGE_ID  = 0x0C;
              
                enum
                {
                   CONSUMER_CONTROL = 0x01,
                   VOLUME           = 0xE0,
                   VOLUME_MUTE      = 0xE2,
                   VOLUME_INCREMENT = 0xE9,
                   VOLUME_DECREMENT = 0xEA
                };
            };

            //-------------------------------------------------------
            class LED
            {
              public:
                static const BYTE PAGE_ID = 0x08; 
            };

            //-------------------------------------------------------
            class Button
            {
              public:
                static const BYTE PAGE_ID = 0x09; 
            };

        }; // UsagePage


        //-----------------------------------------------------------
        class DataType
        {
          public:
            //---------------------------------------------------------------
            // DataTypeId
            //
            enum
            {
              DATA         = 0x00,
              CONSTANT     = 0x01,

              ARRAY        = 0x00,
              VARIABLE     = 0x02,
              ABSOLUTE     = 0x00,
              RELATIVE     = 0x04,
              WRAP         = 0x08,
              NON_LINEAR   = 0x10,
              NON_PREFERED = 0x20,
              NULL_STATE   = 0x40,
              VOLATILE     = 0x80
            };
        };

        //---------------------------------------------------------------
        // Collection
        //
        class Collection
        {
          public:
            //---------------------------------------------------------------
            // CollectionId
            //
            enum
            {
              PHYSICAL       = 0x00,
              APPLICATION    = 0x01,
              LOGICAL        = 0x02,
              REPORT         = 0x03,
              NAMED_ARRAY    = 0x04,
              USAGE_SWITCH   = 0x05,
              USAGE_MODIFIER = 0x06
            };
        };
      };

#pragma pack()

  protected:
    //-----------------------------------------------------------------
    enum
    {
       DEVICE        = 0,
       CONFIG        = 1,
       INTERF        = 2,
       CLASS         = 3,
       ENDPOINT      = 4,
       REPORT        = 5,
       STRING        = 6,
       DEVICE_VENDOR = 7
    };

  public:
    //-----------------------------------------------------------------
    virtual Std::DataPointer getDescriptor( BYTE typeID,
                                            BYTE confID,
                                            BYTE interfID,
                                            BYTE ID        ) = 0;

    //-----------------------------------------------------------------
    virtual void registerInterface( BYTE                confId,
                                    BYTE                interfId,
                                    USBdeviceInterface *interf ) = 0;

    //-----------------------------------------------------------------
    virtual USBdeviceInterface *getInterface( BYTE confId,
                                              BYTE interfId ) = 0;

    //-----------------------------------------------------------------
    virtual void registerEndpoint( BYTE               epId,
                                   USBdeviceEndpoint *endpoint ) = 0;

    //-----------------------------------------------------------------
    virtual USBdeviceEndpoint *getEndpoint( BYTE epId ) = 0;

    //-----------------------------------------------------------------
     virtual Std::DataPointer getDescriptorDevice( void );

    //-----------------------------------------------------------------
    virtual Std::DataPointer getDescriptorConfig( BYTE confId );

    //-----------------------------------------------------------------
    virtual Std::DataPointer getDescriptorInterface( BYTE confId,
                                                     BYTE interfId );

    //-----------------------------------------------------------------
    virtual Std::DataPointer getDescriptorEndpoint( BYTE confId,
                                                    BYTE interfId,
                                                    BYTE epId );

    //-----------------------------------------------------------------
    virtual Std::DataPointer getDescriptorHID( BYTE confId,
                                               BYTE interfId,
                                               BYTE hidId );

    //-----------------------------------------------------------------
    virtual Std::DataPointer getDescriptorHIDreport( BYTE confId,
                                                     BYTE interfId,
                                                     BYTE hidId );

    //-----------------------------------------------------------------
    virtual Std::DataPointer getDescriptorString( BYTE strId );

    //-----------------------------------------------------------------
    virtual Std::DataPointer getDescriptorDeviceVendor( void );

    //-----------------------------------------------------------------
    virtual BYTE getMaxPacketSize0( void );

  protected:
    //-----------------------------------------------------------------
    virtual BYTE getConfigDeviceStatus( BYTE confId );

  //-------------------------------------------------------------------
  friend class USBdevice;
  friend class USBdeviceControl;
  friend class USBdeviceInterface;
  friend class USBdeviceEndpoint;

  friend class USBinterfClassHID;

}; //class USBdeviceDescriptor

}  } //namespace

#endif
