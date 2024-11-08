@echo off

echo cleaning Doc
echo ------------

set pfad=%cd%

cd %EMBSYSLIB%\Doc

rmdir /S /Q .\_Log       2>> nul
rmdir /S /Q .\HTML            2>> nul

cd %pfad%

echo.