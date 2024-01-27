@echo off
echo build "%cd%"
echo -----

echo Generate USB descriptor file

set out=descriptor.cpp

php -f "%EMBSYSLIB%\Src\Hardware\Common\USB\USBdevice\USB_Descriptor_Script.php" descriptor.txt USBdeviceDescriptor_0 > %out%

if /I "%ERRORLEVEL%" EQU "0" (
	echo.
) else (
	echo Error: File %out% not generated correctly
	rem del %out%
	pause
)

echo.
