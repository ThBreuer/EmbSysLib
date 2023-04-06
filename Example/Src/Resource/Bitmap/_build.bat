@echo off
echo build "%cd%"
echo -----

"%EMBSYSLIB%\Tools\bmp2cpp.exe" -F Bitmap_320x240
"%EMBSYSLIB%\Tools\bmp2cpp.exe" -F Bitmap_32x32

echo.