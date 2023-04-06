
"%EMBSYSLIB%\Tools\Image.exe"    -out image.bin -in ../../Src/Resource/Bitmap/Bitmap_320x240.bin
"%EMBSYSLIB%\Tools\Image.exe" -A -out image.bin -in ../../Src/Resource/Bitmap/Bitmap_32x32.bin
"%EMBSYSLIB%\Tools\Image.exe" -A -out image.bin -in ../../Src/Resource/Font/Font_16x24.bin
"%EMBSYSLIB%\Tools\Image.exe" -A -out image.bin -in ../../Src/Resource/Font/Font_10x20.bin
"%EMBSYSLIB%\Tools\Image.exe" -A -out image.bin -in ../../Src/Resource/Font/Font_8x12.bin
"%EMBSYSLIB%\Tools\Image.exe" -A -out image.bin -in ../../Src/Resource/Font/Font_8x8.bin

ST-Link_CLI -P image.bin 0x8180000 -V while_programming
ST-Link_CLI -Rst

pause