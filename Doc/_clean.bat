@echo off

echo cleaning Doc
echo ------------

set pfad=%cd%

cd %EMBSYSLIB%\Doc

rmdir /S /Q .\_Log       2>> nul
rmdir /S /Q .\Main       2>> nul
rmdir /S /Q .\STM32L1xx  2>> nul
rmdir /S /Q .\STM32L4xx  2>> nul
rmdir /S /Q .\Virtual    2>> nul

cd %pfad%

echo.