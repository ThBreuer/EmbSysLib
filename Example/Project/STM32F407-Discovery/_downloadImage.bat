@echo off
"%EMBSYSLIB%\Tools\Image.exe"    -out image.bin -in ../../Src/Resource/Bitmap/Bitmap_320x240.bin
"%EMBSYSLIB%\Tools\Image.exe" -A -out image.bin -in ../../Src/Resource/Bitmap/Bitmap_32x32.bin
"%EMBSYSLIB%\Tools\Image.exe" -A -out image.bin -in ../../Src/Resource/Font/Font_16x24.bin
"%EMBSYSLIB%\Tools\Image.exe" -A -out image.bin -in ../../Src/Resource/Font/Font_10x20.bin
"%EMBSYSLIB%\Tools\Image.exe" -A -out image.bin -in ../../Src/Resource/Font/Font_8x12.bin
"%EMBSYSLIB%\Tools\Image.exe" -A -out image.bin -in ../../Src/Resource/Font/Font_8x8.bin

objcopy.exe --change-addresses 0x08080000 -I binary -O ihex image.bin image.hex

call %EMBSYSLIB%\Tools\flash.bat image.hex

pause