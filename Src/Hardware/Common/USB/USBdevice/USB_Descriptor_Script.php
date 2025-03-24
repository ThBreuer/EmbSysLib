<?php
//*******************************************************************
/*!
\file   USB_Descriptor_Script.php
\author Thomas Breuer ( Bonn-Rhein-Sieg University of Applied Sciences )
\date   13.12.2021

License: See file "LICENSE"

\brief PHP-Script to generate an USB Descriptor
*/

//*******************************************************************
$inputFileName = $argv[1];
$className     = $argv[2];

//*******************************************************************
//-------------------------------------------------------------------
function error( $str )
{
  echo "\r\n";
  echo "++++++++++++++++++++++++++++++++++++++\r\n";
  echo "#error \"".$str."\"\r\n";
  echo "++++++++++++++++++++++++++++++++++++++\r\n";
  exit(  );
}

//-------------------------------------------------------------------
function int2hexbytes( $arg )
{
  return sprintf( "(%s)&0xFF, (%s)>>8",$arg,$arg );
}

//-------------------------------------------------------------------
function asciiString2chars( $arg, $len )
{
  $str = "";
  for( $k=0;$k<$len;$k++ )
  {
    if( $k < strlen( $arg ) )
      $str = $str.sprintf( "'%s'",substr( $arg,$k,1 ) );
    else
      $str = $str.sprintf( "0" );
    if( $k < $len-1 )
      $str = $str.",";
  }
  return( $str );
}

//-------------------------------------------------------------------
function replace( $str, $arg )
{
  $token = explode(",",$arg);
  $ret = $str.$token[0];
  for( $n=1;$n<count($token);$n++ )
    $ret = $ret." | ".$str.$token[$n];
  return( $ret );
}

//-------------------------------------------------------------------
$String = array(  );

// First string descriptor is LANGID array
// The first language is en-us
$String[] = (object)array( "bLength"=>2+2,
						  "str"=>"en-us",
						  "id"=>0 );

function addString( $str,$id="" )
{
  $str = str_replace( array( "'" ),array( "" ),$str );
  global $String;
  if( strlen( $id ) == 0 )
    $String[] = (object)array( "bLength"=>2*strlen( $str )+2,
                              "str"=>$str,
                              "id"=>count( $String ) );
  else
   $String[] = (object)array( "bLength"=>2*strlen( $str )+2,
                             "str"=>$str,
                             "id"=>hexdec( $id ) );
  return( count( $String ) - 1 );
}

//-------------------------------------------------------------------
$Report = array();

//-------------------------------------------------------------------
$COMP = array();

//-------------------------------------------------------------------
$Offs = array();
function addOffset( $id, $OUTiff, $len, $name )
{
  global $Offs;
  if( count( $Offs ) == 0 )
    $Offs[] = (object)array( "off" => 0,
                             "id"  => $id,
                             "diff"=> $OUTiff,
                             "len" => $len,
                             "name"=> $name );
  else
    $Offs[] = (object)array( "off" => end( $Offs )->off + end( $Offs )->diff,
                             "id"  => $id,
                             "diff"=> $OUTiff,
                             "len" => $len,
                             "name"=> $name );
}

//-------------------------------------------------------------------
function getId( $typeId, $confId = 0, $interfId = 0, $epId = 0 )
{
  return( $typeId <<12 | $confId << 8 | $interfId << 4 | $epId <<0 );
}

//-------------------------------------------------------------------
function getStrId( $id )
{
  return( getId( 0x06, 0, 0, $id ) );
}

//*******************************************************************
// Start
//*******************************************************************
if( $inputFileName == NULL )
{
  error( "inputFileName not provided. Use >php -f scriptFile inputFileName className" );
}

if( $className == NULL )
{
  error( "className not provided. Use >php -f scriptFile inputFile className" );
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Read input file and translate to JSON format
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// read input file
$lines  = file( $inputFileName ); 

// start JSON
$JSON = "{\n";

$pending = false;
$isArr   = false;
foreach( $lines AS $line )
{
  // skip comment, carrage return and new line
  $line  = explode( "//",str_replace( array( "\r","\n" ),array( "","" ),$line ) );
  
  // get key or go to next line
  $str   = explode( "=", str_replace( array( " " ),array( "" ),$line[0] ) );
  if( strlen( $str[0] )==0 )
    continue;
  else
    $key = $str[0];
  
  // get value
  $str = explode( "=", $line[0] );
  if( count( $str ) > 1 )
    $value = trim( str_replace( array( "\"" ),array( "" ),$str[1] ) );
  else
    $value = "";

  // parse $key and write braces or $key or $key $value pair to JSON
  // <BEGIN> ... <NEXT> ...<END>                     is translated to :[{...},{...}]
  // <BEGIN_ARRAY> ... <NEXT_ARRAY> ... <END_ARRAY>  is translated to [{"arr":[...]},{"arr":[...]}]
  switch( $key )
  {
    case "<BEGIN>"      : $out= ":[{\n";               $pending=false;              break;
    case "<NEXT>"       : $out= "\n},\n{\n";           $pending=false;              break;
    case "<END>"        : $out= "}\n]\n";              $pending=true;               break;
    case "<BEGIN_ARRAY>": $out= ":[{\"arr\":[\n";      $pending=false; $isArr=true; break;
    case "<NEXT_ARRAY>" : $out= "\n]},\n{\"arr\":[\n"; $pending=false;              break;
    case "<END_ARRAY>"  : $out= "]}]\n";               $pending=true; $isArr=false; break;
    default: 
       if( $pending )
         $out=",";
       else
         $out="";
       $pending = true;
       if( $isArr )
       {
         if( strlen( $value ) > 0 )
           $out= $out."[\"".$key."\",\"".$value."\"]\n";  // ["$key","value"]
         else
           $out= $out."[\"".$key."\"]\n";                 // ["key"]
       }
       else
       {
         if( strlen( $value ) > 0 )
           $out= $out."\"".$key."\":\"".$value."\"\n";    // "$key":"$value"
         else
           $out= $out."\"".$key."\"\n";                   // "$key"
       }
       break;
  }
  
  // add to JSON
  $JSON = $JSON . $out;
}

// finalize JSON
$JSON = $JSON . "}\n";

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Convert JSON to output structure
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// parse JSON
$OUT = json_decode( $JSON );
if( $OUT == NULL )
{
  echo "Debug JSON :\n";
  echo "**********************\n";
  echo $JSON;
  echo "**********************\n";
  error( "No valid JSON format" );
}

$I = array();
$E = array();

//-------------------------------------------------------------------
// DEVICE
//-------------------------------------------------------------------
$OUT->bLength            = 18;
$OUT->bDeviceClass       = "Device::".$OUT->Class;
$OUT->iManufacturer      = addString( $OUT->ManufacturerStr );
$OUT->iProduct           = addString( $OUT->ProductStr );
$OUT->iSerialNumber      = addString( $OUT->SerialNumberStr );
$OUT->bNumConfigurations = 0;

foreach( $OUT->CONFIGURATION AS &$c )
{
  //-------------------------------------------------------------------
  // CONFIGURATION
  //-------------------------------------------------------------------
  $OUT->bNumConfigurations++;

  $c->bLength             = 9;
  $c->bConfigurationValue = $OUT->bNumConfigurations;
  $c->iConfiguration      = addString( $c->Name );
  
  $c->bmAttributes        = "0x80";
  if( isset( $c->SelfPowered ) && strtolower( $c->SelfPowered ) == "yes" )
    $c->bmAttributes =  $c->bmAttributes . "| Configuration::SELF_POWERED";
  if( isset( $c->RemoteWakeup ) && strtolower( $c->RemoteWakeup ) == "yes" )
    $c->bmAttributes =  $c->bmAttributes . "| Configuration::REMOTE_WAKEUP";

  $c->bNumInterfaces      = 0;
  $c->wTotalLength        = $c->bLength;

  foreach( $c->INTERFACE as &$I )
  {
    //-------------------------------------------------------------------
    // INTERFACE
    //-------------------------------------------------------------------
    $I->bLength          = 9;
    $I->bInterfaceNumber = $c->bNumInterfaces++;
    $I->iInterface       = addString( $I->Name );
    $c->wTotalLength    += $I->bLength;

    $IA[] = ( object )array( "id"=>getId( 0x02, $c->bConfigurationValue, $I->bInterfaceNumber ) );

    //-------------------------------------------------------------------
    // CLASS
    //-------------------------------------------------------------------
    if( $I->Class == "VENDOR_SPECIFIC_CLASS" )
    {
      $I->bInterfaceClass  = "Interface::".$I->Class;
      $c->wTotalLength    += 0; // no additional length
    }
    else if( $I->Class == "CDC_COMMUNICATION" )
    {
      $I->bInterfaceClass  = "Interface::".$I->Class;
      $c->wTotalLength    += 0; // no additional length
    }
    else if( $I->Class == "CDC_DATA" )
    {
      $I->bInterfaceClass  = "Interface::".$I->Class;
      $c->wTotalLength    += 0; // no additional length
    }
    else if( $I->Class == "HUMAN_INTERFACE_DEVICE" && $I->HID[0] != NULL )
    {
      $I->bInterfaceClass      = "Interface::".$I->Class;
      
      $I->HID[0]->bCountryCode = "HID::Country::".$I->HID[0]->Country;
      $I->HID[0]->bDescriptorType = 0x21;
      $I->HID[0]->bNumDescriptors = count( $I->HID[0]->REPORT );  // number of report descriptors ?
      $I->HID[0]->bLength         = 6 + 3*$I->HID[0]->bNumDescriptors;

      foreach( $I->HID[0]->REPORT as &$r )
      {
        //-------------------------------------------------------------------
        // REPORT
        //-------------------------------------------------------------------
        $r->Type     = 0x22;
        
        $bytes = array(  );
        $usagePage = "";
        
        foreach( $r->arr as &$arr )
        {
          $hd  = sprintf( "\r\n    /* %-15s */ ",$arr[0] );
          $hd2 = "(BYTE)";
          switch( $arr[0] )
          {
             // Main item
             case "Input"        : $bytes[] = $hd."0x81"; $bytes[] = replace( "Report::DataType::",$arr[1] ); break;
             case "Output"       : $bytes[] = $hd."0x91"; $bytes[] = replace( "Report::DataType::",$arr[1] ); break;
             case "Feature"      : $bytes[] = $hd."0xB1"; $bytes[] = replace( "Report::DataType::",$arr[1] ); break;

             case "Collection"   : $bytes[] = $hd."0xA1"; $bytes[] = "Report::Collection::".$arr[1]; break;
             case "EndCollection": $bytes[] = $hd."0xC0"; break;

             // Global item
             case "UsagePage"    : $usagePage = "Report::UsagePage::".$arr[1];
                                   $bytes[] = $hd."0x05"; $bytes[] = $usagePage."::PAGE_ID"; break;
             case "LogicalMin"   : $bytes[] = $hd."0x15"; $bytes[] = $hd2.$arr[1]; break;
             case "LogicalMinS"  : $bytes[] = $hd."0x16"; $bytes[] = $hd2."( ( ".$arr[1]." ) )&0xFF"; 
                                                          $bytes[] = $hd2."( ( ".$arr[1]." )>>8 )&0xFF"; break;
             case "LogicalMax"   : $bytes[] = $hd."0x25"; $bytes[] = $hd2.$arr[1]; break;
             case "LogicalMaxS"  : $bytes[] = $hd."0x26"; $bytes[] = $hd2."( ( ".$arr[1]." ) )&0xFF"; 
                                                          $bytes[] = $hd2."( ( ".$arr[1]." )>>8 )&0xFF"; break;
             case "PhysicalMin"  : $bytes[] = $hd."0x35"; $bytes[] = $arr[1]; break;
             case "PhysicalMinS" : $bytes[] = $hd."0x36"; $bytes[] = $hd2."( ( ".$arr[1]." ) )&0xFF"; 
                                                          $bytes[] = $hd2."( ( ".$arr[1]." )>>8 )&0xFF"; break;
             case "PhysicalMax"  : $bytes[] = $hd."0x45"; $bytes[] = $hd2.$arr[1]; break;
             case "PhysicalMaxS" : $bytes[] = $hd."0x46"; $bytes[] = $hd2."( ( ".$arr[1]." ) )&0xFF"; 
                                                          $bytes[] = $hd2."( ( ".$arr[1]." )>>8 )&0xFF"; break;
             case "UnitExponent" : $bytes[] = $hd."0x55"; $bytes[] = $hd2.$arr[1]; break;
             case "Unit"         : $bytes[] = $hd."0x65"; $bytes[] = $hd2.$arr[1]; break;
             case "UnitS"        : $bytes[] = $hd."0x66"; $bytes[] = $hd2."( ( ".$arr[1]." ) )&0xFF"; 
                                                          $bytes[] = $hd2."( ( ".$arr[1]." )>>8 )&0xFF"; break;
             case "ReportSize"   : $bytes[] = $hd."0x75"; $bytes[] = $arr[1]; break;
             case "ReportID"     : $bytes[] = $hd."0x85"; $bytes[] = $arr[1]; break;
             case "ReportCount"  : $bytes[] = $hd."0x95"; $bytes[] = $arr[1]; break;

             case "Push"         : $bytes[] = $hd."0xA0"; break;
             case "Pop"          : $bytes[] = $hd."0xB0"; break;

             // Local item
             case "Usage"        : $bytes[] = $hd."0x09"; $bytes[] = $usagePage."::".$arr[1]; break;
             case "UsageMin"     : $bytes[] = $hd."0x19"; $bytes[] = $hd2.$arr[1]; break;
             case "UsageMax"     : $bytes[] = $hd."0x29"; $bytes[] = $hd2.$arr[1]; break;

             default: error( "unkown REPORT item:".$arr[0] );
          }
        }
        $r->wDescriptorLength = count( $bytes );
        $Report[] = (object)array( "ConfigID" => $c->bConfigurationValue,
                                   "InterfID" => $I->bInterfaceNumber,
                                   "bytes"    => (object)$bytes );
      }
      $c->wTotalLength += $I->HID[0]->bLength;
    }
    else
    {
      $I->bInterfaceClass  = "#pragma error: class not defined!";
      $c->wTotalLength    += 0; // no additional length
    }
    
    //-------------------------------------------------------------------
    // OS_Compatibily Decriptor
    //-------------------------------------------------------------------
    if( isset( $I->COMPAT ) )
    {
      $COMP[] = (object)array( "interf"=>$I->bInterfaceNumber,
                               "ID"    => str_replace( array("'"),array(""),$I->COMPAT[0]->ID),
                               "SubID" => str_replace( array("'"),array(""),$I->COMPAT[0]->SubID) );
    }

    //-------------------------------------------------------------------
    // IAD
    //-------------------------------------------------------------------
    if( isset( $I->IAD ) )
    {
      $I->IAD[0]->bLength          = 8;
      $I->IAD[0]->bFirstInterface  = $I->bInterfaceNumber;
      $I->IAD[0]->Count            = max( 1,$I->IAD[0]->Count );
      $I->IAD[0]->bInterfaceClass  = $I->bInterfaceClass;
      $I->IAD[0]->SubClass         = $I->SubClass;
      $I->IAD[0]->Protocol         = $I->Protocol;
      $I->IAD[0]->iFunction        = $I->iInterface;

      $c->wTotalLength += $I->IAD[0]->bLength;
    }
    
    //-------------------------------------------------------------------
    // ENDPOINT
    //-------------------------------------------------------------------
    $I->bNumEndpoints = 0;
    if( isset( $I->ENDPOINT ) )
      foreach( $I->ENDPOINT as &$e )
      {
        $e->bLength         = 7;
        $e->bmAttributes    = "Endpoint::".$e->Attributes;
        $e->bEndpointNumber = $I->bNumEndpoints++;

        $c->wTotalLength   += $e->bLength;

        switch( $e->Direction )
        {
          case "IN": $e->bEndpointAddress = $e->Address | 0x80; break;
          case "OUT":$e->bEndpointAddress = $e->Address;        break;
          default:   $e->bEndpointAddress = 0;                  break;
        }
        $E[] = ( object )array( "id"=>$e->bEndpointAddress );
      }
  }
}

//-------------------------------------------------------------------
// STRING
//-------------------------------------------------------------------
if( isset( $OUT->STRING ) )
  foreach( $OUT->STRING AS $s )
  {
    addString( $s->String,$s->Address );
  }

//-------------------------------------------------------------------
// Add Microsoft compatibility string
//-------------------------------------------------------------------
if( count( $COMP ) )
{
  addString( "MSFT100 ","0xEE" );
}

//-------------------------------------------------------------------
// Calculate offsets
//-------------------------------------------------------------------
addOffset( getId( 0x00 ), $OUT->bLength, $OUT->bLength, "Device" );
foreach( $OUT->CONFIGURATION AS &$C )
{
  addOffset( getId( 0x01,$C->bConfigurationValue ), $C->bLength, $C->wTotalLength, "Configuration" );
  foreach( $C->INTERFACE as &$I )
  {
    $length = $I->bLength;
    if( isset( $I->IAD ) )
    {
      addOffset( -1, $I->IAD[0]->bLength, $I->IAD[0]->bLength, "IAD" );
    }
    addOffset( getId( 0x02,$C->bConfigurationValue,$I->bInterfaceNumber ),  $I->bLength, $I->bLength, "Interface" );
    switch( $I->bInterfaceClass )
    {
      case "Interface::VENDOR_SPECIFIC_CLASS":
        break;
      case "Interface::CDC_COMMUNICATION":
        break;
      case "Interface::CDC_DATA":
        break;
      case "Interface::HUMAN_INTERFACE_DEVICE":
        addOffset( getId( 0x03,$C->bConfigurationValue,$I->bInterfaceNumber ), $I->HID[0]->bLength, $I->HID[0]->bLength, "HID" );
        break;
      default:
        error( "InterfaceClass not defined" );
    }
    if( isset( $I->ENDPOINT ) )
      foreach( $I->ENDPOINT as &$e )
      {
        addOffset( getId( 0x04,$C->bConfigurationValue,$I->bInterfaceNumber,$e->bEndpointNumber ), $e->bLength, $e->bLength, "Endpoint" );
      }
  }
}

// Report Descriptor offsets
for( $n=0;$n<count( $Report );$n++ )
{
  addOffset( getId( 0x05, $Report[$n]->ConfigID, $Report[$n]->InterfID, $n ), count( ( array )$Report[$n]->bytes ), count( ( array )$Report[$n]->bytes ), "Report" );
}

// String Descriptor offsets
foreach( $String as $s )
{
  addOffset( getStrId( $s->id ), $s->bLength, $s->bLength, "String" );
}

if( count( $COMP ) )
{
  addOffset( getId( 0x07, 0, 0, 0 ), 16+count( $COMP )*24,  16+count( $COMP )*24 , "Compat" );
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Output to C++ file
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
?>
//###################################################################"
//
// USB Descriptor File
//
// DON'T EDIT!
//
// This file is auto generated by
<?php echo "//    >php -f ".basename($argv[0])." ".$inputFileName." ".$className."\r\n";?>
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
class <?php echo $className ?> : public USBdeviceDescriptor
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
<?php
foreach( $Offs AS $o )
{
  if( $o->id != -1 )
    printf( "        case 0x%04X: return( DataPointer( ( BYTE* )&descriptor[%3d], %2d ) ); // %s\r\n", $o->id, $o->off, $o->len, $o->name );
}
?>
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
<?php
for( $n=0;$n<count( $IA );$n++ )
  printf( "        case 0x%04x: interfaces[%d] = interface; break;\r\n", $IA[$n]->id, $n );
?>
      }
    }

    //---------------------------------------------------------------
    virtual USBdeviceInterface *getInterface( BYTE confId, BYTE interfId )
    {
      switch( getId( INTERF,confId,interfId,0 ) )
      {
<?php
for( $n=0;$n<count( $IA );$n++ )
  printf( "        case 0x%04x: return( interfaces[%d] );\r\n", $IA[$n]->id, $n );
?>
      }
      return( NULL );
    }

    //---------------------------------------------------------------
    virtual void registerEndpoint( BYTE epId, USBdeviceEndpoint *ep )
    {
      switch( epId )
      {
<?php
for( $n=0;$n<count( $E );$n++ )
  printf( "        case 0x%02x: epList[%d] = ep; break;\r\n", $E[$n]->id, $n );
?>
        default: break;
      }
    }

    //---------------------------------------------------------------
    virtual USBdeviceEndpoint *getEndpoint( BYTE epId )
    {
      switch( epId )
      {
<?php
for( $n=0;$n<count( $E );$n++ )
  printf( "       case 0x%02x: return( epList[%d] );\r\n", $E[$n]->id, $n );
?>
        default: break;
      }
      return( NULL );
    }

  private:
    //---------------------------------------------------------------
    static const BYTE          descriptor[<?php printf( "%d",end( $Offs )->off+end( $Offs )->len+1 );?>];
    static USBdeviceInterface *interfaces[<?php printf( "%d",count( $IA ) );?>];
    static USBdeviceEndpoint  *epList[<?php printf( "%d",max( 1,count( $E ) ) );?>];
};


//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
USBdeviceInterface *<?php echo $className ?>::interfaces[<?php printf( "%d",count( $IA ) );?>] =
{
<?php
for( $n=1;$n<count( $IA );$n++ )
  printf( "  NULL,\r\n" );
?>
  NULL
};

//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
USBdeviceEndpoint *<?php echo $className ?>::epList[<?php printf( "%d",max( 1,count( $E ) ) );?>] =
{
<?php
for( $n=1;$n<count( $E );$n++ )
  printf( "  NULL,\r\n" );
?>
  NULL
};

//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
const BYTE <?php echo $className ?>::descriptor[<?php printf( "%d",end( $Offs )->off+end( $Offs )->len+1 );?>] =
{
<?php
printf( "\r\n  // DEVICE" );
printf( "\r\n    /* bLength            */ %d,",     $OUT->bLength                 );
printf( "\r\n    /* bDescriptorType    */ 0x01,"                                );
printf( "\r\n    /* bcdUSB             */ %s,",     int2hexbytes( $OUT->Version )   );
printf( "\r\n    /* bDeviceClass       */ %s,",     $OUT->bDeviceClass            );
printf( "\r\n    /* bDeviceSubClass    */ %s,",     $OUT->SubClass                );
printf( "\r\n    /* bDeviceProtocol    */ %s,",     $OUT->Protocol                );
printf( "\r\n    /* bMaxPacketSize0    */ %s,",     $OUT->MaxPacketSize           );
printf( "\r\n    /* idVendor           */ %s,",     int2hexbytes( $OUT->VendorID )  );
printf( "\r\n    /* idProduct          */ %s,",     int2hexbytes( $OUT->ProductID ) );
printf( "\r\n    /* bcdDevice          */ %s,",     int2hexbytes( $OUT->Device )    );
printf( "\r\n    /* iManufacturer      */ 0x%02x,", $OUT->iManufacturer           );
printf( "\r\n    /* iProduct           */ 0x%02x,", $OUT->iProduct                );
printf( "\r\n    /* iSerialNumber      */ 0x%02x,", $OUT->iSerialNumber           );
printf( "\r\n    /* bNumConfigurations */ %d,",     $OUT->bNumConfigurations      );


foreach( $OUT->CONFIGURATION AS &$C )
{
  printf( "\r\n\r\n  // CONFIGURATION" );
  printf( "\r\n    /* bLength             */ %d,",     $C->bLength                    );
  printf( "\r\n    /* bDescriptorType     */ 0x02,"                                   );
  printf( "\r\n    /* wTotalLength        */ %s,",     int2hexbytes( $C->wTotalLength ) );
  printf( "\r\n    /* bNumInterfaces      */ %d,",     $C->bNumInterfaces             );
  printf( "\r\n    /* bConfigurationValue */ %d,",     $C->bConfigurationValue        );
  printf( "\r\n    /* iConfiguration      */ 0x%02x,", $C->iConfiguration             );
  printf( "\r\n    /* bmAttributes        */ %s,",     $C->bmAttributes               );
  printf( "\r\n    /* bMaxPower           */ %s,",     $C->MaxPower                   );

  foreach( $C->INTERFACE as &$I )
  {
    if( isset( $I->IAD ) )
    {
      printf( "\r\n\r\n  // Interface Association Descriptor" );
      printf( "\r\n    /* bLength           */ %d,",     $I->IAD[0]->bLength         );
      printf( "\r\n    /* bDescriptorType   */ 0x0B,"                                );
      printf( "\r\n    /* bFirstInterface   */ %d," ,    $I->IAD[0]->bFirstInterface );
      printf( "\r\n    /* bInterfaceCount   */ %s,",     $I->IAD[0]->Count           );
      printf( "\r\n    /* bFunctionClass    */ %s,",     $I->IAD[0]->bInterfaceClass );
      printf( "\r\n    /* bFunctionSubClass */ %s,",     $I->IAD[0]->SubClass        );
      printf( "\r\n    /* bFunctionProtocol */ %s,",     $I->IAD[0]->Protocol        );
      printf( "\r\n    /* iFunction         */ 0x%02x,", $I->IAD[0]->iFunction       );
    }
    printf( "\r\n\r\n  // INTERFACE" );
    printf( "\r\n    /* bLength            */ %d,",     $I->bLength          );
    printf( "\r\n    /* bDescriptorType    */ 0x04,"                         );
    printf( "\r\n    /* bInterfaceNumber   */ %d,",     $I->bInterfaceNumber );
    printf( "\r\n    /* bAlternateSetting  */ 0,"                            );
    printf( "\r\n    /* bNumEndpoints      */ %d,",     $I->bNumEndpoints    );
    printf( "\r\n    /* bInterfaceClass    */ %s,",     $I->bInterfaceClass  );
    printf( "\r\n    /* bInterfaceSubClass */ %s,",     $I->SubClass         );
    printf( "\r\n    /* bInterfaceProtocol */ %s,",     $I->Protocol         );
    printf( "\r\n    /* iInterface         */ 0x%02x,", $I->iInterface       );

    switch( $I->bInterfaceClass )
    {
      case "Interface::VENDOR_SPECIFIC_CLASS":
        break;
      case "Interface::CDC_COMMUNICATION":
        break;
      case "Interface::CDC_DATA":
        break;
      case "Interface::HUMAN_INTERFACE_DEVICE":
      {
        printf( "\r\n\r\n  // HID" );
        printf( "\r\n    /* bLength           */ %d,",  $I->HID[0]->bLength               );
        printf( "\r\n    /* bDescriptorType   */ 0x21,"                                   );
        printf( "\r\n    /* bcdHID            */ %s,",  int2hexbytes( $I->HID[0]->Version ) );
        printf( "\r\n    /* bCountryCode      */ %s,",  $I->HID[0]->bCountryCode          );
        printf( "\r\n    /* bNumDescriptors   */ %d,",  $I->HID[0]->bNumDescriptors       );

        foreach( $I->HID[0]->REPORT as &$r )
        {
            printf( "\r\n    /* bDescriptorType   */ 0x%02x,", $r->Type                            );
            printf( "\r\n    /* wDescriptorLength */ %s,",     int2hexbytes( $r->wDescriptorLength ) );
        }
        break;
      }
      default:
        error( "InterfaceClass not defined" );
    }

    if( isset( $I->ENDPOINT ) ) 
      foreach( $I->ENDPOINT as &$e )
      {
        printf( "\r\n\r\n  // ENDPOINT */ " );
        printf( "\r\n    /* bLength          */ %d,",     $e->bLength                     );
        printf( "\r\n    /* bDescriptorType  */ 0x05,"                                    );
        printf( "\r\n    /* bEndpointAddress */ 0x%02x,", $e->bEndpointAddress            );
        printf( "\r\n    /* bmAttributes     */ %s,",     $e->bmAttributes                  );
        printf( "\r\n    /* wMaxPacketSize   */ %s,",     int2hexbytes( $e->MaxPacketSize ) );
        printf( "\r\n    /* bInterval        */ %s,",     $e->Interval                    );
      }
  }
}

// Report Descriptor
  foreach( $Report as $r )
  {
    printf( "\r\n\r\n  // REPORT" );
    foreach( $r->bytes as $b )
    {
      printf( "%s, ",$b );
    }
  }

// String Descriptor
foreach( $String as $s )
{
  printf( "\r\n\r\n  // STRING 0x%02x",          $s->id );
  printf( "\r\n    /* bLength         */ %d,", $s->bLength );
  printf( "\r\n    /* bDescriptorType */ 0x03,"            );
  
  if( $s->id == 0 )
  {
	switch( $s->str )
	{
		case "en-us": 
		default:	  $langid = 0x0409;	break;
	}
	printf( "\r\n    (0x%04x)&0xFF, (0x%04x)>>8,", $langid, $langid );
  }
  else
  {
	  for( $i=0;$i<strlen( $s->str );$i++ )
	  {
		printf( "\r\n    '%s',0,",substr( $s->str,$i,1 ) );
	  }
  }
}

// OS Compatibility ID Feature
if( count( $COMP ) )
{
  printf( "\r\n\r\n  // OS Compatibility ID Feature" );
  printf( "  \r\n  // Header" );
  printf( "\r\n    /* dwLength         */ %s,0,0,", int2hexbytes( 16+count( $COMP )*24 ) );
  printf( "\r\n    /* bcdVersion       */ %s,",     int2hexbytes( 0x0100 ) );
  printf( "\r\n    /* wIndex           */ %s,",     int2hexbytes( 0x004 ) );
  printf( "\r\n    /* bCount           */ %d,",     count( $COMP ) );
  printf( "\r\n    /* 7 BYTES reserved */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00," );

  for( $i=0;$i<count( $COMP );$i++ )
  {
    printf( "  \r\n  // Function" );
    printf( "\r\n    /* bFirstInterfaceNumber     */ %d,", $COMP[$i]->interf );
    printf( "\r\n    /* 1 BYTE reserved           */ 0x00,"                  );

    printf( "\r\n    /* 8 BYTES Compatible ID     */ %s,", asciiString2chars($COMP[$i]->ID,   8) );
    printf( "\r\n    /* 8 BYTES Compatible ID     */ %s,", asciiString2chars($COMP[$i]->SubID,8) );

    printf( "\r\n    /* 6 BYTES reserved          */ 0,0,0,0,0,0," );
  }
}
?>


    0 // final byte
};

//*******************************************************************
}  } //namespace
//EOF
