
"%EMBSYSLIB%\Tools\Image.exe"    -out image.bin -in ../../Src/Resource/Bitmap/Bitmap_320x240.bin
"%EMBSYSLIB%\Tools\Image.exe" -A -out image.bin -in ../../Src/Resource/Bitmap/Bitmap_32x32.bin
"%EMBSYSLIB%\Tools\Image.exe" -A -out image.bin -in ../../Src/Resource/Font/Font_16x24.bin
"%EMBSYSLIB%\Tools\Image.exe" -A -out image.bin -in ../../Src/Resource/Font/Font_10x20.bin
"%EMBSYSLIB%\Tools\Image.exe" -A -out image.bin -in ../../Src/Resource/Font/Font_8x12.bin
"%EMBSYSLIB%\Tools\Image.exe" -A -out image.bin -in ../../Src/Resource/Font/Font_8x8.bin

STM32_Programmer_CLI.exe -c port=SWD -w image.bin 0x80C0000 -v

pause