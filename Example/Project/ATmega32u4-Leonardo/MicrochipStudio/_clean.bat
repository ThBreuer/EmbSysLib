@echo off
echo cleaning Example (%cd%)
echo ----------------

rmdir /S /Q .\Debug  2>>nul
rmdir /S /Q .\.vs    2>>nul



echo.