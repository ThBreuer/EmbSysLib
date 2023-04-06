@echo off
echo build "%cd%"
echo -----

set EXE="%EMBSYSLIB%\Tools\Bmp2Font.exe"

%EXE% -F Font_10x20   -w 10 -h 20 -f 32 -l 127
%EXE% -F Font_16x24   -w 16 -h 24 -f 32 -l 127
%EXE% -F Font_8x12    -w  8 -h 12 -f 32 -l 127
%EXE% -F Font_8x8     -w  8 -h 8 -f 32 -l 127

echo.
